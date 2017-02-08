#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (std430) buffer;
layout(binding = 0) writeonly buffer output_buffer_data
{
    vec3 pos[];
} output_data;

layout (location = 0) uniform float t;

void main()
{
    uint ident = gl_GlobalInvocationID.x;
    vec3 pos = vec3
    (
        sin(t*2)*2 + 5,
        cos(t*2)*2,
        cos(t*2)*2 - t/2
    );
    output_data.pos[ident] = pos;
}
