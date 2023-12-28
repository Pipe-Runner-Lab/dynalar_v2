#version 330

const int MAX_POINT_LIGHTS=4;

struct MeshBaseMaterial{
  vec4 albedo;
};

struct AmbientLight{
  vec3 color;
  float intensity;
};

struct PointLight{
  vec3 color;
  float intensity;
  vec3 position;
};

out vec4 pixelColor;
in vec2 v_uv;
in vec4 v_color;
in vec3 v_normal;
in vec3 v_fragPos;

// general
uniform sampler2D u_textureSampler;
// https://stackoverflow.com/questions/33690186/opengl-bool-uniform
uniform bool u_shouldUseTexture=false;
uniform bool u_debugNormals=false;
uniform MeshBaseMaterial u_material;

// lights
uniform AmbientLight u_ambientLight;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

void main(){
  if(u_debugNormals){
    pixelColor=v_color;
    return;
  }
  
  vec4 ambient=vec4(u_ambientLight.color*u_ambientLight.intensity,1.f);
  
  vec4 diffuse=vec4(0.f);
  for(int i=0;i<u_numPointLights;i++){
    PointLight pointLight=u_pointLights[i];
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(v_normal,lightDir),0.f);
    diffuse+=vec4(pointLight.color*pointLight.intensity*diffuseFactor,1.f);
  }
  
  if(u_shouldUseTexture){
    pixelColor=ambient*texture2D(u_textureSampler,v_uv);
  }
  else{
    pixelColor=(diffuse+ambient)*u_material.albedo;
  }
}
