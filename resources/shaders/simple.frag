#version 150

in vec3 pass_Normal;
in vec3 pass_VertPos;
//in vec3 pass_eyePos;

//is defined by glUniform3f
uniform vec3 PlanetColor;

uniform float LightIntensity;
uniform vec3 LightColor;


out vec4 out_Color;

vec3 diffuse = PlanetColor;
vec3 ambient = diffuse;
vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(0.0, 0.0, 0.0);

float M_pi = 3.14159;
float reflect = 0.5;
float shineness = 16.0;


void main() {

    vec3 lightDir = normalize(lightPos - pass_VertPos);
    vec3 viewDir = normalize(- pass_VertPos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 normal = normalize(pass_Normal);
    float spec = 0.0;

    vec3 phi = LightColor*LightIntensity;
    vec3 beta = phi/(4*M_pi*(pow(length(lightDir), 2)));
    float lambertian = max(dot(lightDir,normal), 0.0);
    float reflectivity = reflect/M_pi;

    if(lambertian > 0.0) {

      float specAngle = max(dot(halfway, normal),0.0);
      spec = pow(specAngle, 4*shineness);
    }

  //out_Color = vec4(PlanetColor, 1.0);
   vec3 linear_Color = ambient + diffuse*lambertian*beta*reflectivity + specularColor*spec*beta;
  out_Color = vec4(linear_Color,1.0);
}
