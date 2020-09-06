#version 330

out vec4 color;

uniform vec2 u_resolution;
uniform vec3 u_color;
uniform float u_time;

void main(void) {
    color = vec4(u_color, 1);
    // color = vec4(1.0,0.0,0.0,1.0);
}