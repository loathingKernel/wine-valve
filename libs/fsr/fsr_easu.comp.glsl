#version 460
#extension GL_GOOGLE_include_directive: require

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1,rgba32f) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    uvec4 c1, c2, c3, c4;
};

layout(local_size_x=8, local_size_y=8, local_size_z=1) in;

#define A_GPU 1
#define A_GLSL 1
#include "ffx_a.h"
#define FSR_EASU_F 1
vec4 FsrEasuRF(vec2 p){return vec4(textureGather(texSampler, p, 0));}
vec4 FsrEasuGF(vec2 p){return vec4(textureGather(texSampler, p, 1));}
vec4 FsrEasuBF(vec2 p){return vec4(textureGather(texSampler, p, 2));}
#include "ffx_fsr1.h"

void main()
{
    vec3 color;

    if (any(greaterThanEqual(gl_GlobalInvocationID.xy, c4.zw)))
        return;

    FsrEasuF(color, uvec2(gl_GlobalInvocationID.xy), c1, c2, c3, c4);

    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), vec4(color, 1.0));
}
