#version 330

const int MAX_POINT_LIGHTS=4;
float specularStrength=.7;

struct MeshBaseMaterial{
  vec4 albedo;
  sampler2D albedoMaps[10];
  sampler2D specularMaps[10];
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
uniform vec3 u_viewPos;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

void main(){
  if(u_debugNormals){
    pixelColor=v_color;
    return;
  }
  
  vec3 normal=normalize(v_normal);
  vec3 viewDir=normalize(u_viewPos-v_fragPos);
  
  vec3 ambient=u_ambientLight.color*u_ambientLight.intensity;
  
  vec3 diffuse=vec3(0.f);
  vec3 specular=vec3(0.f);
  for(int i=0;i<u_numPointLights;i++){
    PointLight pointLight=u_pointLights[i];
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    diffuse+=pointLight.color*pointLight.intensity*diffuseFactor;
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),32);
    specular+=pointLight.color*specularStrength*specularFactor;
  }
  
  if(u_shouldUseTexture){
    pixelColor=vec4(ambient,1.)*texture2D(u_textureSampler,v_uv);
  }
  else{
    pixelColor=vec4((diffuse+ambient+specular)*u_material.albedo.xyz,1.);
  }
}
