#version 330

layout (location=0) in vec3 position; 

uniform mat4 proj_matrix;
uniform mat4 mv_matrix;
uniform vec2 u_resolution;
uniform vec3 u_color;
uniform float u_time;

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    // gl_Position = vec4(position, 1.0);
}