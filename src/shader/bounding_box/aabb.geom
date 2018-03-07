#version 450

layout (lines) in;

layout (std140, binding = 8) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (triangle_strip, max_vertices = 14) out;
void main() {
    vec4 min_vert = gl_in[0].gl_Position;
    vec4 max_vert = gl_in[1].gl_Position;
    gl_Position = projection * view * vec4(min_vert.x, max_vert.y, max_vert.z, 1.0); // 4
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, max_vert.y, max_vert.z, 1.0); // 3
    EmitVertex();
    gl_Position = projection * view  * vec4(min_vert.x, min_vert.y, max_vert.z, 1.0); // 7
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, min_vert.y, max_vert.z, 1.0); // 8
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, min_vert.y, min_vert.z, 1.0); // 5
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, max_vert.y, max_vert.z, 1.0); // 3
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, max_vert.y, min_vert.z, 1.0); // 1
    EmitVertex();
    gl_Position = projection * view  *  vec4(min_vert.x, max_vert.y, max_vert.z, 1.0); // 4
    EmitVertex();
    gl_Position = projection * view  *  vec4(min_vert.x, max_vert.y, min_vert.z, 1.0); // 2
    EmitVertex();
    gl_Position = projection * view  * vec4(min_vert.x, min_vert.y, max_vert.z, 1.0); // 7
    EmitVertex();
    gl_Position = projection * view  * vec4(min_vert.x, min_vert.y, min_vert.z, 1.0); // 6
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, min_vert.y, min_vert.z, 1.0); // 5
    EmitVertex();
    gl_Position = projection * view  * vec4(min_vert.x, max_vert.y, min_vert.z, 1.0); // 2
    EmitVertex();
    gl_Position = projection * view  * vec4(max_vert.x, max_vert.y, min_vert.z, 1.0); // 1
    EmitVertex();
    EndPrimitive();
}
