#version 410 core

const int MAX_POINT_LIGHTS=4;
const int MAX_SPOT_LIGHTS=4;
const int MAX_TEXTURES=10;

struct MeshBaseMaterial{
  vec3 albedo;
  vec3 specular;
  float opacity;
  int metalness;
  int numDiffuseMaps;
  sampler2D diffuseMaps[MAX_TEXTURES];
  int numSpecularMaps;
  sampler2D specularMaps[MAX_TEXTURES];
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
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  float specularIntensity;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float cutoff;
  float outerCutoff;
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
uniform vec3 u_viewPos;
uniform AmbientLight u_ambientLight;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_numSpotLights;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

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
  
  // ambient light
  // TODO: Add support for ambient maps
  if(u_material.numDiffuseMaps>0){
    ambient+=u_ambientLight.color*u_ambientLight.ambientIntensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz;
  }else{
    ambient+=u_ambientLight.color*u_ambientLight.ambientIntensity*u_material.albedo;
  }
  
  // point light
  for(int i=0;i<u_numPointLights;i++){
    PointLight pointLight=u_pointLights[i];
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
    
    // attenuation
    float distance=length(pointLight.position-v_fragPos);
    float attenuation=1./(pointLight.constant+pointLight.linear*distance+pointLight.quadratic*distance*distance);
    
    // TODO Add support for ambient maps
    // TODO: currently only using 1st diffuse map
    if(u_material.numDiffuseMaps>0){
      ambient+=(pointLight.color*pointLight.ambientIntensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz*attenuation);
    }else{
      ambient+=(pointLight.color*pointLight.ambientIntensity*u_material.albedo*attenuation);
    }
    
    if(u_material.numDiffuseMaps>0){
      for(int j=0;j<u_material.numDiffuseMaps;j++){
        diffuse+=(pointLight.color*pointLight.diffuseIntensity*diffuseFactor*texture2D(u_material.diffuseMaps[j],v_uv).xyz*attenuation);
      }
    }
    else{
      diffuse+=(pointLight.color*pointLight.diffuseIntensity*diffuseFactor*u_material.albedo*attenuation);
    }
    
    if(u_material.numSpecularMaps>0){
      for(int j=0;j<u_material.numSpecularMaps;j++){
        specular+=(pointLight.color*pointLight.specularIntensity*specularFactor*texture2D(u_material.specularMaps[j],v_uv).xyz*attenuation);
      }
    }
    else{
      specular+=(pointLight.color*pointLight.specularIntensity*specularFactor*u_material.specular*attenuation);
    }
  }
  
  // spot light
  for(int i=0;i<u_numSpotLights;i++){
    SpotLight spotLight=u_spotLights[i];
    vec3 lightDir=normalize(spotLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
    
    // attenuation
    float distance=length(spotLight.position-v_fragPos);
    float attenuation=1./(spotLight.constant+spotLight.linear*distance+spotLight.quadratic*distance*distance);
    
    float phi=cos(radians(spotLight.cutoff));
    float gamma=cos(radians(spotLight.outerCutoff));
    float theta=dot(lightDir,normalize(-spotLight.direction));
    float epsilon=phi-gamma;
    float intensity=clamp((theta-gamma)/epsilon,0.,1.);
    
    // process ambient irrespective of cutoff
    if(u_material.numDiffuseMaps>0){
      ambient+=(spotLight.color*spotLight.ambientIntensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz*attenuation);
    }else{
      ambient+=(spotLight.color*spotLight.ambientIntensity*u_material.albedo*attenuation);
    }
    
    // process diffuse and specular only if within cutoff
    if(u_material.numDiffuseMaps>0){
      for(int j=0;j<u_material.numDiffuseMaps;j++){
        diffuse+=(spotLight.color*spotLight.diffuseIntensity*diffuseFactor*texture2D(u_material.diffuseMaps[j],v_uv).xyz*intensity*attenuation);
      }
    }
    else{
      diffuse+=(spotLight.color*spotLight.diffuseIntensity*diffuseFactor*u_material.albedo*intensity*attenuation);
    }
    
    if(u_material.numSpecularMaps>0){
      for(int j=0;j<u_material.numSpecularMaps;j++){
        specular+=(spotLight.color*spotLight.specularIntensity*specularFactor*texture2D(u_material.specularMaps[j],v_uv).xyz*intensity*attenuation);
      }
    }
    else{
      specular+=(spotLight.color*spotLight.specularIntensity*specularFactor*u_material.specular*intensity*attenuation);
    }
  }
  
  // TODO: Opactiy map needs to be accounted for
  pixelColor=vec4((diffuse+ambient+specular),u_material.opacity);
  
  // gamma correction
  float gamma=2.2;
  pixelColor.rgb=pow(pixelColor.rgb,vec3(1./gamma));
}
