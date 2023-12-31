#version 410 core

const int MAX_POINT_LIGHTS=4;
float specularStrength=.7;

struct MeshBaseMaterial{
  vec4 albedo;
  int numDiffuseMaps;
  sampler2D diffuseMaps[10];
  int numSpecularMaps;
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
  
  vec3 ambient=vec3(0.f);
  vec3 diffuse=vec3(0.f);
  vec3 specular=vec3(0.f);
  
  if(u_shouldUseTexture){
    ambient=u_ambientLight.color*u_ambientLight.intensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz;
  }else{
    ambient=u_ambientLight.color*u_ambientLight.intensity;
  }
  
  for(int i=0;i<u_numPointLights;i++){
    PointLight pointLight=u_pointLights[i];
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),32);
    
    if(u_shouldUseTexture){
      for(int j=0;j<u_material.numDiffuseMaps;j++){
        diffuse+=pointLight.color*pointLight.intensity*diffuseFactor*texture2D(u_material.diffuseMaps[j],v_uv).xyz;
      }
      
      for(int j=0;j<u_material.numSpecularMaps;j++){
        specular+=pointLight.color*specularStrength*specularFactor*texture2D(u_material.specularMaps[j],v_uv).xyz;
      }
    }
    else{
      diffuse+=pointLight.color*pointLight.intensity*diffuseFactor*u_material.albedo.xyz;
      specular+=pointLight.color*specularStrength*specularFactor*u_material.albedo.xyz;
    }
  }
  
  pixelColor=vec4((diffuse+ambient+specular),1.);
}
