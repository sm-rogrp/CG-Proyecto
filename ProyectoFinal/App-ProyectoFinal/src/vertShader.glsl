#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;

out vec4 varyingColor;


struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};
struct Material
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

uniform int op_shader;

void main(){
    if (op_shader == 1){ // blingpong
        varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
        varyingLightDir = light.position - varyingVertPos;
        varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;
        
        varyingHalfVector =
            normalize(normalize(varyingLightDir)
            + normalize(-varyingVertPos)).xyz;
    }

    if (op_shader == 2){ // goraud
        vec4 color;
        // convert vertex position to view space
        vec4 P = mv_matrix * vec4(vertPos,1.0);
        // convert normal to view space
        vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);
        // calculate view-space light vector (from point to light)
        vec3 L = normalize(light.position - P.xyz);
        //  view vector is negative of view space position
        vec3 V = normalize(-P.xyz);
        //  R is reflection of -L around the plane defined by N
        vec3 R = reflect(-L,N);
        // ambient, diffuse, and specular contributions
        vec3 ambient =
            ((globalAmbient * material.ambient)
            + (light.ambient * material.ambient)).xyz;
        vec3 diffuse =
            light.diffuse.xyz * material.diffuse.xyz
            * max(dot(N,L), 0.0);
        vec3 specular =
            pow(max(dot(R,V), 0.0f), material.shininess)
            * material.specular.xyz * light.specular.xyz;
        // send the color output to the fragment shader
        varyingColor = vec4((ambient + diffuse + specular), 1.0);
    }

    if (op_shader == 3){ // phong
        varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
        varyingLightDir = light.position - varyingVertPos;
        varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;
    }

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);

}