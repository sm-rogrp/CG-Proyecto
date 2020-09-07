#version 330

in vec4 varyingColor;
out vec4 fragColor;

struct PositionalLight{ 
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material{ 
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 norm_matrix; // for transforming normals
uniform mat4 proj_matrix;
uniform mat4 mv_matrix;

uniform vec2 u_resolution;
uniform vec3 u_color;
uniform float u_time;
uniform int show_smooth; // true or false 

void main(void) {
    if (show_smooth == 1){
        fragColor = varyingColor * vec4(u_color, 1.0);
    } 
    if (show_smooth == 0){
        fragColor = vec4(u_color, 1.0);
    }
}