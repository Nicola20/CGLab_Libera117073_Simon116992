#version 150

in vec3 pass_Normal;
in vec3 pass_VertPos;
in vec3 pass_eyePos;
//flat in int shader_Mode;

//is defined by glUniform3f
uniform vec3 PlanetColor;

uniform float LightIntensity;
uniform vec3 LightColor;
uniform int ShaderMode;



out vec4 out_Color;

vec3 diffuse = PlanetColor;
vec3 ambient = vec3(0.1,0.0,0.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const vec3 lightPos = vec3(0.0, 0.0, 0.0);

const float M_pi = 3.1415926535897932384626433832795;
const float reflect = 2.5;
const float shineness = 4.0;


void main() {

      vec3 lightDir = normalize(lightPos - pass_VertPos);
      vec3 normal = normalize(pass_Normal);
      float spec = 0.0;

      vec3 phi = LightColor*LightIntensity;
      float distance = length(lightDir);
      vec3 beta = phi/(4*M_pi*(distance*distance));
      float lambertian = max(dot(lightDir,normal), 0.0);
      float reflectivity = reflect/M_pi;

    if (ShaderMode == 1) {
     // if(lambertian > 0.0) {
        vec3 viewDir = normalize(-pass_VertPos);
        vec3 halfway = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfway, normal),0.0);
        spec = pow(specAngle, 4*shineness);
     // }

      vec3 linear_Color = ambient + diffuse*lambertian*beta*reflectivity + specularColor*spec*beta;
      out_Color = vec4(linear_Color,1.0);
    }


    if (ShaderMode == 2) {

       vec3 eye_dir = normalize( -pass_eyePos);
      float view_angle = dot(eye_dir, normal);

      //float normal_View_Angle = dot(-normalize(pass_VertPos), normalize(pass_Normal));

      vec3 viewDir = normalize(- pass_VertPos);
      
      vec3 halfway = normalize(lightDir + viewDir);
      float specAngle= max(dot(halfway, normal),0.0);
      spec = pow(specAngle, 4*shineness);
      float lambertian = max(dot(lightDir,normal), 0.0);

      if(lambertian > 0.66) {lambertian = 1;}
      else if(lambertian > 0.33) {lambertian = 0.66;}
      else if(lambertian > 0.0) {lambertian = 0.33;}


      // highlight the planets border in a different color
      if(abs(view_angle) < 0.4) {
        out_Color = vec4(1.0, 1.0, 1.0,1.0);
      } 
      else {

        // calculate planet color
        vec3 linear_Color = ambient + diffuse*lambertian*beta*reflectivity + specularColor*spec*beta;
        out_Color = vec4(linear_Color,1.0);
      }

    }
  
}
