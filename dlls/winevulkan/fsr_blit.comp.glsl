#version 450

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    //both in real image coords
    vec2 offset;
    vec2 extents;
} constants;

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main()
{
    vec2 texcoord = (vec2(gl_GlobalInvocationID.xy) - constants.offset) / constants.extents;
    vec4 c = texture(texSampler, texcoord);
    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), c);
}
