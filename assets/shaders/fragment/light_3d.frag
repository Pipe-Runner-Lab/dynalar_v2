#version 410 core

const int MAX_POINT_LIGHTS=4;

struct MeshBaseMaterial{
  vec3 albedo;
  vec3 specular;
  float opacity;
  int metalness;
  int numDiffuseMaps;
  sampler2D diffuseMaps[10];
  int numSpecularMaps;
  sampler2D specularMaps[10];
};

struct AmbientLight{
  vec3 color;
  float ambientIntensity;
};

struct PointLight{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  float specularIntensity;
  vec3 position;
};

out vec4 pixelColor;
in vec2 v_uv;
in vec4 v_color;
in vec3 v_normal;
in vec3 v_fragPos;

// general
// https://stackoverflow.com/questions/33690186/opengl-bool-uniform
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
  
  // TODO: Add support for ambient maps
  ambient=u_ambientLight.color*u_ambientLight.ambientIntensity;
  
  for(int i=0;i<u_numPointLights;i++){
    PointLight pointLight=u_pointLights[i];
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
    
    // TODO Add support for ambient maps
    // TODO: currently only using 1st diffuse map
    if(u_material.numDiffuseMaps>0){
      ambient+=pointLight.color*pointLight.ambientIntensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz;
    }else{
      ambient+=pointLight.color*pointLight.ambientIntensity*u_material.albedo;
    }
    
    if(u_material.numDiffuseMaps>0){
      for(int j=0;j<u_material.numDiffuseMaps;j++){
        diffuse+=pointLight.color*pointLight.diffuseIntensity*diffuseFactor*texture2D(u_material.diffuseMaps[j],v_uv).xyz;
      }
    }
    else{
      diffuse+=pointLight.color*pointLight.diffuseIntensity*diffuseFactor*u_material.albedo;
    }
    
    if(u_material.numSpecularMaps>0){
      for(int j=0;j<u_material.numSpecularMaps;j++){
        specular+=pointLight.color*pointLight.specularIntensity*specularFactor*texture2D(u_material.specularMaps[j],v_uv).xyz;
      }
    }
    else{
      specular+=pointLight.color*pointLight.specularIntensity*specularFactor*u_material.specular;
    }
  }
  
  // TODO: Opactiy map needs to be accounted for
  pixelColor=vec4((diffuse+ambient+specular),u_material.opacity);
}
