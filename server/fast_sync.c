/*
 * Fast synchronization primitives
 *
 * Copyright (C) 2021-2022 Zebediah Figura for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "winternl.h"

#include "file.h"
#include "handle.h"
#include "request.h"
#include "thread.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/winesync.h>

struct linux_device
{
    struct object obj;      /* object header */
    struct fd *fd;          /* fd for unix fd */
};

static struct linux_device *linux_device_object;

static void linux_device_dump( struct object *obj, int verbose );
static struct fd *linux_device_get_fd( struct object *obj );
static void linux_device_destroy( struct object *obj );
static enum server_fd_type linux_device_get_fd_type( struct fd *fd );

static const struct object_ops linux_device_ops =
{
    sizeof(struct linux_device),        /* size */
    &no_type,                           /* type */
    linux_device_dump,                  /* dump */
    no_add_queue,                       /* add_queue */
    NULL,                               /* remove_queue */
    NULL,                               /* signaled */
    NULL,                               /* get_esync_fd */
    NULL,                               /* get_fsync_idx */
    NULL,                               /* satisfied */
    no_signal,                          /* signal */
    linux_device_get_fd,                /* get_fd */
    default_map_access,                 /* map_access */
    default_get_sd,                     /* get_sd */
    default_set_sd,                     /* set_sd */
    no_get_full_name,                   /* get_full_name */
    no_lookup_name,                     /* lookup_name */
    no_link_name,                       /* link_name */
    NULL,                               /* unlink_name */
    no_open_file,                       /* open_file */
    no_kernel_obj_list,                 /* get_kernel_obj_list */
    no_get_fast_sync,                   /* get_fast_sync */
    no_close_handle,                    /* close_handle */
    linux_device_destroy                /* destroy */
};

static const struct fd_ops linux_device_fd_ops =
{
    default_fd_get_poll_events,     /* get_poll_events */
    default_poll_event,             /* poll_event */
    linux_device_get_fd_type,       /* get_fd_type */
    no_fd_read,                     /* read */
    no_fd_write,                    /* write */
    no_fd_flush,                    /* flush */
    no_fd_get_file_info,            /* get_file_info */
    no_fd_get_volume_info,          /* get_volume_info */
    no_fd_ioctl,                    /* ioctl */
    default_fd_cancel_async,        /* cancel_async */
    no_fd_queue_async,              /* queue_async */
    default_fd_reselect_async       /* reselect_async */
};

static void linux_device_dump( struct object *obj, int verbose )
{
    struct linux_device *device = (struct linux_device *)obj;
    assert( obj->ops == &linux_device_ops );
    fprintf( stderr, "Fast synchronization device fd=%p\n", device->fd );
}

static struct fd *linux_device_get_fd( struct object *obj )
{
    struct linux_device *device = (struct linux_device *)obj;
    return (struct fd *)grab_object( device->fd );
}

static void linux_device_destroy( struct object *obj )
{
    struct linux_device *device = (struct linux_device *)obj;
    assert( obj->ops == &linux_device_ops );
    if (device->fd) release_object( device->fd );
    linux_device_object = NULL;
}

static enum server_fd_type linux_device_get_fd_type( struct fd *fd )
{
    return FD_TYPE_FILE;
}

static struct linux_device *get_linux_device(void)
{
    struct linux_device *device;
    int unix_fd;
    static int wineserver_once;

    if (getenv( "WINE_DISABLE_FAST_SYNC" ) && atoi( getenv( "WINE_DISABLE_FAST_SYNC" ) ))
    {
        set_error( STATUS_NOT_IMPLEMENTED );
        return NULL;
    }

    if (linux_device_object)
        return (struct linux_device *)grab_object( linux_device_object );

    unix_fd = open( "/dev/winesync", O_CLOEXEC | O_RDONLY );
    if (unix_fd == -1)
    {
        file_set_error();
        if (!wineserver_once++) {
            fprintf( stderr, "fastsync: /dev/winesync not available. (Please check winesync module)\n" );
            fprintf( stderr, "wineserver: using server-side synchronization.\n" );
        }
        return NULL;
    }

    if (!(device = alloc_object( &linux_device_ops )))
    {
        close( unix_fd );
        set_error( STATUS_NO_MEMORY );
        if (!wineserver_once++) {
            fprintf( stderr, "fastsync: failed to allocate object.\n" );
            fprintf( stderr, "wineserver: using server-side synchronization.\n" );
        }
        return NULL;
    }

    if (!(device->fd = create_anonymous_fd( &linux_device_fd_ops, unix_fd, &device->obj, 0 )))
    {
        release_object( device );
        set_error( STATUS_NO_MEMORY );
        if (!wineserver_once++) {
            fprintf( stderr, "fastsync: failed to allocate anonymous fd.\n" );
            fprintf( stderr, "wineserver: using server-side synchronization.\n" );
        }
        return NULL;
    }

    linux_device_object = device;
    return device;
}

struct fast_sync
{
    struct object obj;
    struct linux_device *device;
    enum fast_sync_type type;
    unsigned int linux_obj;
};

static void linux_obj_dump( struct object *obj, int verbose );
static void linux_obj_destroy( struct object *obj );

static const struct object_ops fast_sync_ops =
{
    sizeof(struct fast_sync),   /* size */
    &no_type,                   /* type */
    linux_obj_dump,             /* dump */
    no_add_queue,               /* add_queue */
    NULL,                       /* remove_queue */
    NULL,                       /* signaled */
    NULL,                       /* get_esync_fd */
    NULL,                       /* get_fsync_idx */
    NULL,                       /* satisfied */
    no_signal,                  /* signal */
    no_get_fd,                  /* get_fd */
    default_map_access,         /* map_access */
    default_get_sd,             /* get_sd */
    default_set_sd,             /* set_sd */
    no_get_full_name,           /* get_full_name */
    no_lookup_name,             /* lookup_name */
    no_link_name,               /* link_name */
    NULL,                       /* unlink_name */
    no_open_file,               /* open_file */
    no_kernel_obj_list,         /* get_kernel_obj_list */
    no_get_fast_sync,           /* get_fast_sync */
    no_close_handle,            /* close_handle */
    linux_obj_destroy           /* destroy */
};

static void linux_obj_dump( struct object *obj, int verbose )
{
    struct fast_sync *fast_sync = (struct fast_sync *)obj;
    assert( obj->ops == &fast_sync_ops );
    fprintf( stderr, "Fast synchronization object type=%u linux_obj=%u\n",
             fast_sync->type, fast_sync->linux_obj );
}

static void linux_obj_destroy( struct object *obj )
{
    struct fast_sync *fast_sync = (struct fast_sync *)obj;

    ioctl( get_unix_fd( fast_sync->device->fd ), WINESYNC_IOC_DELETE, &fast_sync->linux_obj );
    release_object( fast_sync->device );
}

struct fast_sync *fast_create_event( enum fast_sync_type type, int signaled )
{
    struct winesync_event_args args = {0};
    struct linux_device *device;
    struct fast_sync *fast_sync;

    if (!(device = get_linux_device())) return NULL;

    args.signaled = signaled;
    switch (type)
    {
        case FAST_SYNC_AUTO_EVENT:
        case FAST_SYNC_AUTO_SERVER:
            args.manual = 0;
            break;

        case FAST_SYNC_MANUAL_EVENT:
        case FAST_SYNC_MANUAL_SERVER:
        case FAST_SYNC_QUEUE:
            args.manual = 1;
            break;

        case FAST_SYNC_MUTEX:
        case FAST_SYNC_SEMAPHORE:
            assert(0);
            break;
    }
    if (ioctl( get_unix_fd( device->fd ), WINESYNC_IOC_CREATE_EVENT, &args ) < 0)
    {
        file_set_error();
        release_object( device );
        return NULL;
    }

    if (!(fast_sync = alloc_object( &fast_sync_ops ))) return NULL;

    /* transfer our device reference to the fast sync object */
    fast_sync->device = device;
    fast_sync->type = type;
    fast_sync->linux_obj = args.event;

    return fast_sync;
}

struct fast_sync *fast_create_semaphore( unsigned int count, unsigned int max )
{
    struct winesync_sem_args args = {0};
    struct linux_device *device;
    struct fast_sync *fast_sync;

    if (!(device = get_linux_device())) return NULL;

    args.count = count;
    args.max = max;
    if (ioctl( get_unix_fd( device->fd ), WINESYNC_IOC_CREATE_SEM, &args ) < 0)
    {
        file_set_error();
        release_object( device );
        return NULL;
    }

    if (!(fast_sync = alloc_object( &fast_sync_ops ))) return NULL;

    /* transfer our device reference to the fast sync object */
    fast_sync->device = device;
    fast_sync->type = FAST_SYNC_SEMAPHORE;
    fast_sync->linux_obj = args.sem;

    return fast_sync;
}

struct fast_sync *fast_create_mutex( thread_id_t owner, unsigned int count )
{
    struct winesync_mutex_args args = {0};
    struct linux_device *device;
    struct fast_sync *fast_sync;

    if (!(device = get_linux_device())) return NULL;

    args.owner = owner;
    args.count = count;
    if (ioctl( get_unix_fd( device->fd ), WINESYNC_IOC_CREATE_MUTEX, &args ) < 0)
    {
        file_set_error();
        release_object( device );
        return NULL;
    }

    if (!(fast_sync = alloc_object( &fast_sync_ops ))) return NULL;

    /* transfer our device reference to the fast sync object */
    fast_sync->device = device;
    fast_sync->type = FAST_SYNC_MUTEX;
    fast_sync->linux_obj = args.mutex;

    return fast_sync;
}

void fast_set_event( struct fast_sync *fast_sync )
{
    struct winesync_event_args args = {0};

    if (!fast_sync) return;

    if (debug_level) fprintf( stderr, "fast_set_event %u\n", fast_sync->linux_obj );

    args.event = fast_sync->linux_obj;
    ioctl( get_unix_fd( fast_sync->device->fd ), WINESYNC_IOC_SET_EVENT, &args );
}

struct timespec64
{
    long long tv_sec;
    long long tv_nsec;
};

void fast_reset_event( struct fast_sync *fast_sync )
{
    struct winesync_event_args args = {0};

    if (!fast_sync) return;

    if (debug_level) fprintf( stderr, "fast_reset_event %u\n", fast_sync->linux_obj );

    args.event = fast_sync->linux_obj;
    ioctl( get_unix_fd( fast_sync->device->fd ), WINESYNC_IOC_RESET_EVENT, &args );
}

void fast_abandon_mutexes( thread_id_t tid )
{
    struct linux_device *device;

    if (!(device = get_linux_device()))
    {
        clear_error();
        return;
    }

    ioctl( get_unix_fd( device->fd ), WINESYNC_IOC_KILL_OWNER, &tid );
    release_object( device );
}

DECL_HANDLER(get_linux_sync_device)
{
    struct linux_device *device;

    if ((device = get_linux_device()))
    {
        reply->handle = alloc_handle( current->process, device, 0, 0 );
        release_object( device );
    }
}

DECL_HANDLER(get_linux_sync_obj)
{
    struct object *obj;
    static int once;

    if (!once++)
        fprintf( stderr, "wine: using fast synchronization.\n" );

    if ((obj = get_handle_obj( current->process, req->handle, 0, NULL )))
    {
        struct fast_sync *fast_sync;

        if ((fast_sync = obj->ops->get_fast_sync( obj )))
        {
            reply->handle = alloc_handle( current->process, fast_sync, 0, 0 );
            reply->obj = fast_sync->linux_obj;
            reply->type = fast_sync->type;
            reply->access = get_handle_access( current->process, req->handle );
            release_object( fast_sync );
        }
        release_object( obj );
    }
}
