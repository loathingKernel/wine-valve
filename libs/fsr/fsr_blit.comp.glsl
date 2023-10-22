#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1) uniform writeonly image2D outImage;
layout(push_constant) uniform pushConstants {
    //both in real image coords
    vec2 offset;
    vec2 extents;
} constants;

void main()
{
    vec2 texcoord = (vec2(gl_GlobalInvocationID.xy) - constants.offset) / constants.extents;
    vec4 c = texture(texSampler, texcoord);

    // Convert linear -> srgb
    bvec3 isLo = lessThanEqual(c.rgb, vec3(0.0031308f));
    vec3 loPart = c.rgb * 12.92f;
    vec3 hiPart = pow(c.rgb, vec3(5.0f / 12.0f)) * 1.055f - 0.055f;
    c.rgb = mix(hiPart, loPart, isLo);

    imageStore(outImage, ivec2(gl_GlobalInvocationID.xy), c);
}
