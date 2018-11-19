#version 150

in vec3 pass_Normal;
in vec3 pass_VertPos;
in vec3 pass_eyePos;

//is defined by glUniform3f
uniform vec3 PlanetColor;

uniform float LightIntensity;
uniform vec3 LightColor;


out vec4 out_Color;

vec3 diffuse = PlanetColor;
const vec3 ambient = vec3(0.1,0.0,0.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(0.0, 0.0, 0.0);

float M_pi = 3.14159;
const float reflect = 1.5;
const float shineness = 4.0;


void main() {

    vec3 lightDir = normalize(lightPos - pass_VertPos);
    //vec3 viewDir = normalize(- pass_VertPos);
    //vec3 halfway = normalize(lightDir + viewDir);
    vec3 normal = normalize(pass_Normal);
    float spec = 0.0;

    vec3 phi = LightColor*LightIntensity;
    float distance = length(lightDir);
    vec3 beta = phi/(distance*distance);
    float lambertian = max(dot(lightDir,normal), 0.0);
    float reflectivity = reflect/M_pi;

    if(lambertian > 0.0) {
       vec3 viewDir = normalize(- pass_VertPos);
       vec3 halfway = normalize(lightDir + viewDir);
      float specAngle = max(dot(halfway, normal),0.0);
      spec = pow(specAngle, 4*shineness);
    }

   vec3 linear_Color = ambient + diffuse*lambertian*beta*reflectivity + specularColor*spec*beta;
  out_Color = vec4(linear_Color,1.0);
}

/*
const vec3 ambient = vec3(0.1, 0.0, 0.0);
vec3 diffuseColor = PlanetColor;
vec3 lightPos = vec3(0.0, 0.0, 0.0); 
float shineness = 16.0;
float specAngle = 0;
const vec3 specColor = vec3(1.0, 1.0, 1.0);


void main() {
    vec3 lightDir = normalize(lightPos - pass_VertPos);

    vec3 normal = normalize(pass_Normal);
    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;//neu

    if(lambertian > 0.0) {
    vec3 viewDir = normalize(pass_VertPos- pass_eyePos);

    // this is blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specAngle, 16.0);
    }


  out_Color = vec4(ambient +lambertian * diffuseColor + specular * specColor, 1.0);


}*/
