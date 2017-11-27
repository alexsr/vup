#version 450

layout (lines) in;

layout (std140, binding = 9) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (triangle_strip, max_vertices = 14) out;
void main() {
    vec4 min_vert = gl_in[0].gl_Position;
    vec4 max_vert = gl_in[1].gl_Position;
    gl_Position = projection * vec4(min_vert.x, max_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, max_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(min_vert.x, min_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, min_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, min_vert.y, min_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, max_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, max_vert.y, min_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection *  vec4(min_vert.x, max_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection *  vec4(min_vert.x, max_vert.y, min_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(min_vert.x, min_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(min_vert.x, min_vert.y, min_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, min_vert.y, min_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(min_vert.x, max_vert.y, max_vert.z, 1.0);
    EmitVertex();
    gl_Position = projection * vec4(max_vert.x, max_vert.y, min_vert.z, 1.0);
    EmitVertex();
    EndPrimitive();
}
