#version 150

in vec3 pass_Normal;
in vec3 pass_VertPos;
in vec3 pass_eyePos;
flat in int shader_Mode;

//is defined by glUniform3f
uniform vec3 PlanetColor;

uniform float LightIntensity;
uniform vec3 LightColor;



out vec4 out_Color;

vec3 diffuse = PlanetColor;
const vec3 ambient = vec3(0.1, 0.0 ,0.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(0.0, 0.0, 0.0);

const float M_pi = 3.1415926535897932384626433832795;
const float reflect = 2.5;
const float shineness = 4.0;


void main() {
    if (shader_Mode == 1) {

      vec3 lightDir = normalize(lightPos - pass_VertPos);
      //vec3 viewDir = normalize(- pass_VertPos);
      //vec3 halfway = normalize(lightDir + viewDir);
      vec3 normal = normalize(pass_Normal);
      float spec = 0.0;

      vec3 phi = LightColor*LightIntensity;
      float distance = length(lightDir);
      vec3 beta = phi/(4*M_pi*(distance*distance));
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

    if (shader_Mode == 2) {

      vec3 phi = LightColor*LightIntensity;
      vec3 lightDir = normalize(lightPos - pass_VertPos);
      float distance = length(lightDir);
      vec3 beta = phi/(4*M_pi*(distance*distance));
      vec3 viewDir = normalize(- pass_VertPos);
      
      vec3 halfway = normalize(lightDir + viewDir);
      float normal_View_Angle = dot(-normalize(pass_VertPos), normalize(pass_Normal));
      float spec = 0.0;

      diffuse = PlanetColor;
      //ambient = vec3(0.01, 0.01, 0.01);
      float specAngle= max(dot(halfway, pass_Normal),0.0);
      spec = pow(specAngle, 4*shineness);
      float lambertian = max(dot(lightDir,normalize(pass_Normal)), 0.0);
      float reflectivity = reflect/M_pi;

      if(lambertian > 0.9) {lambertian = 1;}
      else if(lambertian > 0.6) {lambertian = 0.9;}
      else if(lambertian > 0.3) {lambertian = 0.6;}
      else if(lambertian > 0.0) {lambertian = 0.3;}

      // highlight the planets border in a different color
      if(abs(normal_View_Angle) < 0.3) {
        out_Color = vec4(1.0, 0.0, 0.0,1.0);
      } 
      else {

        // calculate planet color
        vec3 linear_Color = ambient + diffuse*lambertian*beta*reflectivity + specularColor*spec*beta;
        out_Color = vec4(linear_Color,1.0);
      }
    

    }
  
}

/*
// Cell-Shading-Model
  if (shader_Mode == 2) {

    float normal_View_Angle = dot(-normalize(vertex_Position), normalize(pass_Normal));

    diffuse_Color = planet_Color;
    ambient_Color = vec3(0.01, 0.01, 0.01);

    specular_Angle = max(dot(H, normalize(pass_Normal)), 0.0);
    specular = pow(specular_Angle, shininess * 10);
    lambertian = max(dot(L, normalize(pass_Normal)), 0.0);

    if(lambertian > 0.9) {lambertian = 1;}
    else if(lambertian > 0.6) {lambertian = 0.9;}
    else if(lambertian > 0.3) {lambertian = 0.6;}
    else if(lambertian > 0.0) {lambertian = 0.3;}

    // highlight the planets border in a different color
    if(abs(normal_View_Angle) < 0.3) {
      color_Linear = vec3(1.0, 0.0, 0.0);
    } else {
      // calculate planet color
      color_Linear = ambient_Color + lambertian * diffuse_Color + specular * specular_Color;
    }

    vec3 L  = normalize(light_Position - vertex_Position); // light direction
    vec3 V = normalize(-vertex_Position); // view direction
    vec3 H = normalize(L + V); // halfway vector
}
*/
