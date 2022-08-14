#version 460
#extension GL_GOOGLE_include_directive: require
#extension GL_EXT_shader_explicit_arithmetic_types_float16 : require

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1,rgba16f) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    uvec4 c1, c2, c3, c4;
};

layout(local_size_x=8, local_size_y=8, local_size_z=1) in;

#define A_GPU 1
#define A_GLSL 1
#define A_HALF 1
#include "ffx_a.h"
#define FSR_EASU_H 1
f16vec4 FsrEasuRH(vec2 p){return f16vec4(textureGather(texSampler, p, 0));}
f16vec4 FsrEasuGH(vec2 p){return f16vec4(textureGather(texSampler, p, 1));}
f16vec4 FsrEasuBH(vec2 p){return f16vec4(textureGather(texSampler, p, 2));}
f16vec3 FsrEasuSampleH(vec2 p){return f16vec3(textureLod(texSampler, p, 0).xyz);}
#include "ffx_fsr1_lite.h"

void main()
{
    f16vec3 color;

    if (any(greaterThanEqual(gl_GlobalInvocationID.xy, c4.zw)))
        return;

    FsrEasuLiteH(color, uvec2(gl_GlobalInvocationID.xy), c1, c2, c3, c4);

    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), f16vec4(color, 1.0));
}
