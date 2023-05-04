/*
 * Audio capture filter unit tests
 *
 * Copyright 2018 Zebediah Figura
 * Copyright 2023 Zebediah Figura for CodeWeavers
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

#define COBJMACROS
#include <stdbool.h>
#include "dshow.h"
#include "mmreg.h"
#include "wine/strmbase.h"
#include "wine/test.h"

static bool compare_media_types(const AM_MEDIA_TYPE *a, const AM_MEDIA_TYPE *b)
{
    return !memcmp(a, b, offsetof(AM_MEDIA_TYPE, pbFormat))
            && !memcmp(a->pbFormat, b->pbFormat, a->cbFormat);
}

static void init_pcm_mt(AM_MEDIA_TYPE *mt, WAVEFORMATEX *format,
        WORD channels, DWORD sample_rate, WORD depth)
{
    memset(format, 0, sizeof(WAVEFORMATEX));
    format->wFormatTag = WAVE_FORMAT_PCM;
    format->nChannels = channels;
    format->nSamplesPerSec = sample_rate;
    format->wBitsPerSample = depth;
    format->nBlockAlign = channels * depth / 8;
    format->nAvgBytesPerSec = format->nBlockAlign * format->nSamplesPerSec;
    memset(mt, 0, sizeof(AM_MEDIA_TYPE));
    mt->majortype = MEDIATYPE_Audio;
    mt->subtype = MEDIASUBTYPE_PCM;
    mt->bFixedSizeSamples = TRUE;
    mt->lSampleSize = format->nBlockAlign;
    mt->formattype = FORMAT_WaveFormatEx;
    mt->cbFormat = sizeof(WAVEFORMATEX);
    mt->pbFormat = (BYTE *)format;
}

static ULONG get_refcount(void *iface)
{
    IUnknown *unknown = iface;
    IUnknown_AddRef(unknown);
    return IUnknown_Release(unknown);
}

#define check_interface(a, b, c) check_interface_(__LINE__, a, b, c)
static void check_interface_(unsigned int line, void *iface_ptr, REFIID iid, BOOL supported)
{
    IUnknown *iface = iface_ptr;
    HRESULT hr, expected_hr;
    IUnknown *unk;

    expected_hr = supported ? S_OK : E_NOINTERFACE;

    hr = IUnknown_QueryInterface(iface, iid, (void **)&unk);
    ok_(__FILE__, line)(hr == expected_hr, "Got hr %#lx, expected %#lx.\n", hr, expected_hr);
    if (SUCCEEDED(hr))
        IUnknown_Release(unk);
}

static void test_interfaces(IBaseFilter *filter)
{
    HRESULT hr;
    IPin *pin;

    todo_wine check_interface(filter, &IID_IAMFilterMiscFlags, TRUE);
    check_interface(filter, &IID_IBaseFilter, TRUE);
    check_interface(filter, &IID_IMediaFilter, TRUE);
    check_interface(filter, &IID_IPersist, TRUE);
    check_interface(filter, &IID_IPersistPropertyBag, TRUE);
    check_interface(filter, &IID_IUnknown, TRUE);

    check_interface(filter, &IID_IBasicAudio, FALSE);
    check_interface(filter, &IID_IBasicVideo, FALSE);
    check_interface(filter, &IID_IKsPropertySet, FALSE);
    check_interface(filter, &IID_IMediaPosition, FALSE);
    check_interface(filter, &IID_IMediaSeeking, FALSE);
    check_interface(filter, &IID_IPin, FALSE);
    check_interface(filter, &IID_IQualityControl, FALSE);
    check_interface(filter, &IID_IQualProp, FALSE);
    check_interface(filter, &IID_IReferenceClock, FALSE);
    check_interface(filter, &IID_IVideoWindow, FALSE);

    hr = IBaseFilter_FindPin(filter, L"Capture", &pin);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    check_interface(pin, &IID_IAMStreamConfig, TRUE);
    todo_wine check_interface(pin, &IID_IKsPropertySet, TRUE);
    check_interface(pin, &IID_IPin, TRUE);
    todo_wine check_interface(pin, &IID_IQualityControl, TRUE);
    check_interface(pin, &IID_IUnknown, TRUE);

    check_interface(pin, &IID_IAsyncReader, FALSE);
    check_interface(pin, &IID_IMediaPosition, FALSE);
    check_interface(pin, &IID_IMediaSeeking, FALSE);

    IPin_Release(pin);
}

static const GUID test_iid = {0x33333333};
static LONG outer_ref = 1;

static HRESULT WINAPI outer_QueryInterface(IUnknown *iface, REFIID iid, void **out)
{
    if (IsEqualGUID(iid, &IID_IUnknown)
            || IsEqualGUID(iid, &IID_IBaseFilter)
            || IsEqualGUID(iid, &test_iid))
    {
        *out = (IUnknown *)0xdeadbeef;
        return S_OK;
    }
    ok(0, "unexpected call %s\n", wine_dbgstr_guid(iid));
    return E_NOINTERFACE;
}

static ULONG WINAPI outer_AddRef(IUnknown *iface)
{
    return InterlockedIncrement(&outer_ref);
}

static ULONG WINAPI outer_Release(IUnknown *iface)
{
    return InterlockedDecrement(&outer_ref);
}

static const IUnknownVtbl outer_vtbl =
{
    outer_QueryInterface,
    outer_AddRef,
    outer_Release,
};

static IUnknown test_outer = {&outer_vtbl};

static void test_aggregation(void)
{
    IBaseFilter *filter, *filter2;
    IUnknown *unk, *unk2;
    HRESULT hr;
    ULONG ref;

    filter = (IBaseFilter *)0xdeadbeef;
    hr = CoCreateInstance(&CLSID_AudioRecord, &test_outer, CLSCTX_INPROC_SERVER,
            &IID_IBaseFilter, (void **)&filter);
    ok(hr == E_NOINTERFACE, "Got hr %#lx.\n", hr);
    ok(!filter, "Got interface %p.\n", filter);

    hr = CoCreateInstance(&CLSID_AudioRecord, &test_outer, CLSCTX_INPROC_SERVER,
            &IID_IUnknown, (void **)&unk);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(outer_ref == 1, "Got unexpected refcount %ld.\n", outer_ref);
    ok(unk != &test_outer, "Returned IUnknown should not be outer IUnknown.\n");
    ref = get_refcount(unk);
    ok(ref == 1, "Got unexpected refcount %ld.\n", ref);

    ref = IUnknown_AddRef(unk);
    ok(ref == 2, "Got unexpected refcount %ld.\n", ref);
    ok(outer_ref == 1, "Got unexpected refcount %ld.\n", outer_ref);

    ref = IUnknown_Release(unk);
    ok(ref == 1, "Got unexpected refcount %ld.\n", ref);
    ok(outer_ref == 1, "Got unexpected refcount %ld.\n", outer_ref);

    hr = IUnknown_QueryInterface(unk, &IID_IUnknown, (void **)&unk2);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(unk2 == unk, "Got unexpected IUnknown %p.\n", unk2);
    IUnknown_Release(unk2);

    hr = IUnknown_QueryInterface(unk, &IID_IBaseFilter, (void **)&filter);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_QueryInterface(filter, &IID_IUnknown, (void **)&unk2);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(unk2 == (IUnknown *)0xdeadbeef, "Got unexpected IUnknown %p.\n", unk2);

    hr = IBaseFilter_QueryInterface(filter, &IID_IBaseFilter, (void **)&filter2);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(filter2 == (IBaseFilter *)0xdeadbeef, "Got unexpected IBaseFilter %p.\n", filter2);

    hr = IUnknown_QueryInterface(unk, &test_iid, (void **)&unk2);
    ok(hr == E_NOINTERFACE, "Got hr %#lx.\n", hr);
    ok(!unk2, "Got unexpected IUnknown %p.\n", unk2);

    hr = IBaseFilter_QueryInterface(filter, &test_iid, (void **)&unk2);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(unk2 == (IUnknown *)0xdeadbeef, "Got unexpected IUnknown %p.\n", unk2);

    IBaseFilter_Release(filter);
    ref = IUnknown_Release(unk);
    ok(!ref, "Got unexpected refcount %ld.\n", ref);
    ok(outer_ref == 1, "Got unexpected refcount %ld.\n", outer_ref);
}

static HRESULT WINAPI property_bag_QueryInterface(IPropertyBag *iface, REFIID iid, void **out)
{
    ok(0, "Unexpected call (iid %s).\n", wine_dbgstr_guid(iid));
    return E_NOINTERFACE;
}

static ULONG WINAPI property_bag_AddRef(IPropertyBag *iface)
{
    ok(0, "Unexpected call.\n");
    return 2;
}

static ULONG WINAPI property_bag_Release(IPropertyBag *iface)
{
    ok(0, "Unexpected call.\n");
    return 1;
}
static int ppb_id;
static unsigned int ppb_got_read;

static HRESULT WINAPI property_bag_Read(IPropertyBag *iface, const WCHAR *name, VARIANT *var, IErrorLog *log)
{
    if (!lstrcmpW(name, L"UseMixer"))
        return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

    todo_wine ok(!lstrcmpW(name, L"WaveInId"), "Got unexpected name %s.\n", wine_dbgstr_w(name));
    ok(V_VT(var) == VT_I4, "Got unexpected type %u.\n", V_VT(var));
    ok(!log, "Got unexpected error log %p.\n", log);
    ppb_got_read++;
    return S_OK;
}

static HRESULT WINAPI property_bag_Write(IPropertyBag *iface, const WCHAR *name, VARIANT *var)
{
    ok(0, "Unexpected call (name %s).\n", wine_dbgstr_w(name));
    return E_FAIL;
}

static const IPropertyBagVtbl property_bag_vtbl =
{
    property_bag_QueryInterface,
    property_bag_AddRef,
    property_bag_Release,
    property_bag_Read,
    property_bag_Write,
};

static void test_property_bag(IMoniker *mon)
{
    IPropertyBag property_bag = {&property_bag_vtbl};
    IPropertyBag *devenum_bag;
    IPersistPropertyBag *ppb;
    VARIANT var;
    HRESULT hr;
    ULONG ref;

    hr = IMoniker_BindToStorage(mon, NULL, NULL, &IID_IPropertyBag, (void **)&devenum_bag);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    VariantInit(&var);
    hr = IPropertyBag_Read(devenum_bag, L"WaveInId", &var, NULL);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ppb_id = V_I4(&var);

    hr = CoCreateInstance(&CLSID_AudioRecord, NULL, CLSCTX_INPROC_SERVER,
            &IID_IPersistPropertyBag, (void **)&ppb);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IPersistPropertyBag_InitNew(ppb);
    todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);

    ppb_got_read = 0;
    hr = IPersistPropertyBag_Load(ppb, &property_bag, NULL);
    ok(hr == S_OK || broken(hr == E_FAIL) /* 8+, intermittent */, "Got hr %#lx.\n", hr);
    ok(ppb_got_read == 1, "Got %u calls to Read().\n", ppb_got_read);

    ref = IPersistPropertyBag_Release(ppb);
    ok(!ref, "Got unexpected refcount %ld.\n", ref);

    VariantClear(&var);
    IPropertyBag_Release(devenum_bag);
}

static void test_unconnected_filter_state(IBaseFilter *filter)
{
    FILTER_STATE state;
    HRESULT hr;

    hr = IBaseFilter_GetState(filter, 0, &state);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(state == State_Stopped, "Got state %u.\n", state);

    hr = IBaseFilter_Pause(filter);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    todo_wine ok(hr == VFW_S_CANT_CUE, "Got hr %#lx.\n", hr);
    ok(state == State_Paused, "Got state %u.\n", state);

    hr = IBaseFilter_Run(filter, 0);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(state == State_Running, "Got state %u.\n", state);

    hr = IBaseFilter_Pause(filter);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    todo_wine ok(hr == VFW_S_CANT_CUE, "Got hr %#lx.\n", hr);
    ok(state == State_Paused, "Got state %u.\n", state);

    hr = IBaseFilter_Stop(filter);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(state == State_Stopped, "Got state %u.\n", state);

    hr = IBaseFilter_Run(filter, 0);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(state == State_Running, "Got state %u.\n", state);

    hr = IBaseFilter_Stop(filter);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IBaseFilter_GetState(filter, 0, &state);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(state == State_Stopped, "Got state %u.\n", state);
}

static void test_pin_info(IBaseFilter *filter)
{
    PIN_DIRECTION dir;
    PIN_INFO info;
    HRESULT hr;
    WCHAR *id;
    IPin *pin;

    hr = IBaseFilter_FindPin(filter, L"Capture", &pin);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IPin_QueryPinInfo(pin, &info);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(info.pFilter == filter, "Expected filter %p, got %p.\n", filter, info.pFilter);
    ok(info.dir == PINDIR_OUTPUT, "Got direction %d.\n", info.dir);
    ok(!wcscmp(info.achName, L"Capture"), "Got name %s.\n", debugstr_w(info.achName));
    IBaseFilter_Release(info.pFilter);

    hr = IPin_QueryDirection(pin, &dir);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(dir == PINDIR_OUTPUT, "Got direction %d.\n", dir);

    hr = IPin_QueryId(pin, &id);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    ok(!wcscmp(id, L"Capture"), "Got id %s.\n", debugstr_w(id));
    CoTaskMemFree(id);

    IPin_Release(pin);
}

static void test_media_types(IBaseFilter *filter)
{
    WAVEFORMATEX format, *pformat;
    IEnumMediaTypes *enummt;
    AM_MEDIA_TYPE mt, *pmt;
    HRESULT hr;
    IPin *pin;

    IBaseFilter_FindPin(filter, L"Capture", &pin);

    hr = IPin_EnumMediaTypes(pin, &enummt);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    hr = IEnumMediaTypes_Next(enummt, 1, &pmt, NULL);
    todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
    if (hr == S_OK)
    {
        pformat = (void *)pmt->pbFormat;
        ok(pformat->nChannels == 2, "Got %u channels.\n", pformat->nChannels);
        ok(pformat->wBitsPerSample == 16, "Got depth %u.\n", pformat->wBitsPerSample);
        ok(pformat->nSamplesPerSec == 44100, "Got sample rate %lu.\n", pformat->nSamplesPerSec);

        do
        {
            pformat = (void *)pmt->pbFormat;

            ok(IsEqualGUID(&pmt->majortype, &MEDIATYPE_Audio), "Got major type %s.\n",
                    debugstr_guid(&pmt->majortype));
            ok(IsEqualGUID(&pmt->subtype, &MEDIASUBTYPE_PCM), "Got subtype %s\n",
                    debugstr_guid(&pmt->subtype));
            ok(pmt->bFixedSizeSamples == TRUE, "Got fixed size %d.\n", pmt->bFixedSizeSamples);
            ok(!pmt->bTemporalCompression, "Got temporal compression %d.\n", pmt->bTemporalCompression);
            ok(pmt->lSampleSize == pformat->nBlockAlign, "Got sample size %lu.\n", pmt->lSampleSize);
            ok(IsEqualGUID(&pmt->formattype, &FORMAT_WaveFormatEx), "Got format type %s.\n",
                    debugstr_guid(&pmt->formattype));
            ok(!pmt->pUnk, "Got pUnk %p.\n", pmt->pUnk);
            ok(pmt->cbFormat == sizeof(WAVEFORMATEX), "Got format size %lu.\n", pmt->cbFormat);

            ok(pformat->wFormatTag == WAVE_FORMAT_PCM, "Got format %#x.\n", pformat->wFormatTag);
            ok(pformat->nChannels == 1 || pformat->nChannels == 2, "Got %u channels.\n", pformat->nChannels);
            ok(pformat->wBitsPerSample == 8 || pformat->wBitsPerSample == 16, "Got depth %u.\n", pformat->wBitsPerSample);
            ok(pformat->nBlockAlign == pformat->nChannels * pformat->wBitsPerSample / 8,
                    "Got block align %u.\n", pformat->nBlockAlign);
            ok(pformat->nAvgBytesPerSec == pformat->nSamplesPerSec * pformat->nBlockAlign,
                    "Got %lu bytes per second.\n", pformat->nAvgBytesPerSec);
            ok(!pformat->cbSize, "Got size %u.\n", pformat->cbSize);

            strmbase_dump_media_type(pmt);

            hr = IPin_QueryAccept(pin, pmt);
            ok(hr == S_OK, "Got hr %#lx.\n", hr);

            CoTaskMemFree(pmt->pbFormat);
            CoTaskMemFree(pmt);

            hr = IEnumMediaTypes_Next(enummt, 1, &pmt, NULL);
        } while (hr == S_OK);
    }

    ok(hr == S_FALSE, "Got hr %#lx.\n", hr);

    init_pcm_mt(&mt, &format, 1, 44100, 8);
    mt.subtype = MEDIASUBTYPE_RGB8;
    mt.bFixedSizeSamples = FALSE;
    mt.bTemporalCompression = TRUE;
    mt.lSampleSize = 123;
    format.nAvgBytesPerSec = 123;
    hr = IPin_QueryAccept(pin, &mt);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    init_pcm_mt(&mt, &format, 1, 44100, 8);
    mt.majortype = MEDIATYPE_Stream;
    hr = IPin_QueryAccept(pin, &mt);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);
    mt.majortype = GUID_NULL;
    hr = IPin_QueryAccept(pin, &mt);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);

    init_pcm_mt(&mt, &format, 1, 44100, 8);
    mt.formattype = FORMAT_None;
    hr = IPin_QueryAccept(pin, &mt);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);
    mt.formattype = GUID_NULL;
    hr = IPin_QueryAccept(pin, &mt);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);

    init_pcm_mt(&mt, &format, 1, 44100, 8);
    format.wFormatTag = 0xdead;
    hr = IPin_QueryAccept(pin, &mt);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);

    /* Validation of format members seems to vary across windows versions. */

    IEnumMediaTypes_Release(enummt);
    IPin_Release(pin);
}

static void test_stream_config(IBaseFilter *filter)
{
    AUDIO_STREAM_CONFIG_CAPS caps;
    IEnumMediaTypes *enummt;
    IAMStreamConfig *config;
    AM_MEDIA_TYPE *mt, *mt2;
    int count, size, i;
    IPin *source;
    HRESULT hr;

    IBaseFilter_FindPin(filter, L"Capture", &source);
    IPin_QueryInterface(source, &IID_IAMStreamConfig, (void **)&config);

    count = size = 0xdeadbeef;
    hr = IAMStreamConfig_GetNumberOfCapabilities(config, &count, &size);
    todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
    todo_wine ok(count && count != 0xdeadbeef, "Got count %d.\n", count);
    todo_wine ok(size == sizeof(AUDIO_STREAM_CONFIG_CAPS), "Got size %d.\n", size);

    hr = IPin_EnumMediaTypes(source, &enummt);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    for (i = 0; i < count; ++i)
    {
        winetest_push_context("Caps %u", i);

        hr = IAMStreamConfig_GetStreamCaps(config, i, &mt, (BYTE *)&caps);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);

        hr = IEnumMediaTypes_Next(enummt, 1, &mt2, NULL);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);
        ok(compare_media_types(mt, mt2), "Media types didn't match.\n");

        ok(IsEqualGUID(&caps.guid, &MEDIATYPE_Audio), "Got GUID %s.\n", debugstr_guid(&caps.guid));
        ok(caps.MinimumChannels == 1, "Got minimum channels %lu.\n", caps.MinimumChannels);
        ok(caps.MaximumChannels == 2, "Got maximum channels %lu.\n", caps.MaximumChannels);
        ok(caps.ChannelsGranularity == 1, "Got channels granularity %lu.\n", caps.ChannelsGranularity);
        ok(caps.MinimumBitsPerSample == 8, "Got minimum depth %lu.\n", caps.MinimumBitsPerSample);
        ok(caps.MaximumBitsPerSample == 16, "Got maximum depth %lu.\n", caps.MaximumBitsPerSample);
        ok(caps.BitsPerSampleGranularity == 8, "Got depth granularity %lu.\n", caps.BitsPerSampleGranularity);
        ok(caps.MinimumSampleFrequency == 11025, "Got minimum rate %lu.\n", caps.MinimumSampleFrequency);
        ok(caps.MaximumSampleFrequency == 44100, "Got maximum rate %lu.\n", caps.MaximumSampleFrequency);
        ok(caps.SampleFrequencyGranularity == 11025, "Got rate granularity %lu.\n", caps.SampleFrequencyGranularity);

        DeleteMediaType(mt2);
        DeleteMediaType(mt);

        winetest_pop_context();
    }

    hr = IAMStreamConfig_GetStreamCaps(config, count, &mt, (BYTE *)&caps);
    todo_wine ok(hr == S_FALSE, "Got hr %#lx.\n", hr);
    hr = IEnumMediaTypes_Next(enummt, 1, &mt2, NULL);
    ok(hr == S_FALSE, "Got hr %#lx.\n", hr);

    IEnumMediaTypes_Release(enummt);

    hr = IAMStreamConfig_GetFormat(config, &mt);
    todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
    if (hr == S_OK)
    {
        hr = IAMStreamConfig_GetStreamCaps(config, 0, &mt2, (BYTE *)&caps);
        todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
        if (hr == S_OK)
        {
            ok(compare_media_types(mt, mt2), "Media types didn't match.\n");
            DeleteMediaType(mt2);
        }

        hr = IAMStreamConfig_SetFormat(config, NULL);
        todo_wine ok(hr == E_POINTER, "Got hr %#lx.\n", hr);

        mt->majortype = MEDIATYPE_Video;
        hr = IAMStreamConfig_SetFormat(config, mt);
        todo_wine ok(hr == VFW_E_INVALIDMEDIATYPE, "Got hr %#lx.\n", hr);

        DeleteMediaType(mt);
    }

    for (i = 0; i < count; ++i)
    {
        winetest_push_context("Caps %u", i);

        hr = IAMStreamConfig_GetStreamCaps(config, i, &mt, (BYTE *)&caps);
        todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
        if (hr == S_OK)
        {
            hr = IAMStreamConfig_SetFormat(config, mt);
            todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);

            hr = IAMStreamConfig_GetFormat(config, &mt2);
            todo_wine ok(hr == S_OK, "Got hr %#lx.\n", hr);
            ok(compare_media_types(mt, mt2), "Media types didn't match.\n");
            DeleteMediaType(mt2);

            DeleteMediaType(mt);
        }
        winetest_pop_context();
    }

    /* Unlike the WDM video capture filter, IEnumMediaTypes() still enumerates
     * all media types. */
    IPin_EnumMediaTypes(source, &enummt);
    for (i = 0; i < count; ++i)
    {
        hr = IAMStreamConfig_GetStreamCaps(config, i, &mt, (BYTE *)&caps);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);

        hr = IEnumMediaTypes_Next(enummt, 1, &mt2, NULL);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);
        ok(compare_media_types(mt, mt2), "Media types didn't match.\n");

        DeleteMediaType(mt2);
        DeleteMediaType(mt);
    }
    IEnumMediaTypes_Release(enummt);

    IAMStreamConfig_Release(config);
    IPin_Release(source);
}

START_TEST(audiorecord)
{
    ICreateDevEnum *devenum;
    IEnumMoniker *enummon;
    IBaseFilter *filter;
    IMoniker *mon;
    WCHAR *name;
    HRESULT hr;
    ULONG ref;

    CoInitialize(NULL);

    hr = CoCreateInstance(&CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
            &IID_ICreateDevEnum, (void **)&devenum);
    ok(hr == S_OK, "Got hr %#lx.\n", hr);
    hr = ICreateDevEnum_CreateClassEnumerator(devenum, &CLSID_AudioInputDeviceCategory, &enummon, 0);
    if (hr == S_FALSE)
    {
        skip("No audio input devices present.\n");
        ICreateDevEnum_Release(devenum);
        CoUninitialize();
        return;
    }
    ok(hr == S_OK, "Got hr %#lx.\n", hr);

    test_aggregation();

    while (IEnumMoniker_Next(enummon, 1, &mon, NULL) == S_OK)
    {
        CLSID clsid;

        hr = IMoniker_GetDisplayName(mon, NULL, NULL, &name);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);
        trace("Testing device %s.\n", wine_dbgstr_w(name));
        CoTaskMemFree(name);

        test_property_bag(mon);

        hr = IMoniker_BindToObject(mon, NULL, NULL, &IID_IBaseFilter, (void **)&filter);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);

        hr = IBaseFilter_GetClassID(filter, &clsid);
        ok(hr == S_OK, "Got hr %#lx.\n", hr);
        ok(IsEqualGUID(&clsid, &CLSID_AudioRecord), "Got clsid %s.\n", debugstr_guid(&clsid));

        test_interfaces(filter);
        test_unconnected_filter_state(filter);
        test_pin_info(filter);
        test_media_types(filter);
        /* This calls SetFormat() and hence should be run last. */
        test_stream_config(filter);

        ref = IBaseFilter_Release(filter);
        ok(!ref, "Got outstanding refcount %ld.\n", ref);
        IMoniker_Release(mon);
    }

    IEnumMoniker_Release(enummon);
    ICreateDevEnum_Release(devenum);
    CoUninitialize();
}
