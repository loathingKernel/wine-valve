#version 460
#extension GL_GOOGLE_include_directive: require
#extension GL_EXT_shader_explicit_arithmetic_types_float16 : require

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1,rgba16f) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    uvec2 c1;
    ivec2 extent;
    ivec4 viewport;
};

layout(local_size_x=8, local_size_y=8, local_size_z=1) in;

#define A_GPU 1
#define A_GLSL 1
#define A_HALF 1
#include "ffx_a.h"
#define FSR_RCAS_H 1
f16vec4 FsrRcasLoadH(i16vec2 p) { return f16vec4(texelFetch(texSampler, clamp(p, ivec2(0), extent), 0)); }
void FsrRcasInputH(inout float16_t r, inout float16_t g, inout float16_t b) {}
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
        FsrRcasH(color.r, color.g, color.b, uvec2(gl_GlobalInvocationID.xy - viewport.xy), c1.xyxx);
    }

    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), f16vec4(color, 1.0));
}

