#version 460
#extension GL_GOOGLE_include_directive: require

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1,rgba32f) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    uvec2 c1;
    ivec2 extent;
    ivec4 viewport;
};

layout(local_size_x=8, local_size_y=8, local_size_z=1) in;

#define A_GPU 1
#define A_GLSL 1
#include "ffx_a.h"
#define FSR_RCAS_F 1
vec4 FsrRcasLoadF(ivec2 p) { return texelFetch(texSampler, clamp(p, ivec2(0), extent), 0); }
void FsrRcasInputF(inout float r, inout float g, inout float b) {}
#include "ffx_fsr1.h"

void main()
{
    vec3 color;

    if (any(lessThan(gl_GlobalInvocationID.xy, uvec2(viewport.xy))) ||
        any(greaterThan(gl_GlobalInvocationID.xy, uvec2(viewport.zw))))
    {
        color = vec3(0.0, 0.0, 0.0);
    }
    else
    {
        FsrRcasF(color.r, color.g, color.b, uvec2(gl_GlobalInvocationID.xy - viewport.xy), c1.xyxx);
    }

    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), vec4(color, 1.0));
}

