#version 410 core

const int MAX_DIRECTIONAL_LIGHTS=4;
const int MAX_POINT_LIGHTS=4;
const int MAX_SPOT_LIGHTS=4;
const int MAX_TEXTURES=10;
const int MAX_SHADOW_MAPS=10;
const float DIRECTIONAL_BIAS_FACTOR=.00009;
const float OMNIDIRECTIONAL_BIAS_FACTOR=.07;

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

struct DirectionalLight{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  float specularIntensity;
  vec3 direction;
  int shadowMapIdx;
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
  int shadowMapIdx;
  float farPlane;
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
  float innerCutoff;
  float outerCutoff;
  int shadowMapIdx;
  float farPlane;
};

out vec4 pixelColor;
in vec2 v_uv;
in vec4 v_color;
in vec3 v_normal;
in vec3 v_fragPos;
in vec4 v_lightSpaceFragPositions[10];

// general
// https://stackoverflow.com/questions/33690186/opengl-bool-uniform
uniform bool u_debugNormals=false;
uniform MeshBaseMaterial u_material;

// lights
uniform vec3 u_viewPos;
uniform int u_numAmbientLights;
uniform AmbientLight u_ambientLight;
uniform int u_numDirectionalLights;
uniform DirectionalLight u_directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_numSpotLights;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

// shadows
uniform sampler2D u_shadowMaps[MAX_SHADOW_MAPS];// for directional shadows
uniform samplerCube u_shadowCubeMaps[MAX_SHADOW_MAPS];// for omnidirectional shadows

const vec3 sampleOffsetDirections[20]=vec3[]
(
  vec3(1,1,1),vec3(1,-1,1),vec3(-1,-1,1),vec3(-1,1,1),
  vec3(1,1,-1),vec3(1,-1,-1),vec3(-1,-1,-1),vec3(-1,1,-1),
  vec3(1,1,0),vec3(1,-1,0),vec3(-1,-1,0),vec3(-1,1,0),
  vec3(1,0,1),vec3(-1,0,1),vec3(1,0,-1),vec3(-1,0,-1),
  vec3(0,1,1),vec3(0,-1,1),vec3(0,-1,-1),vec3(0,1,-1)
);

float ComputeDirectionalShadow(int shadowMapIdx,vec3 lightDir,vec3 normal){
  if(shadowMapIdx<0){
    return 0.;
  }
  
  float bias=max(DIRECTIONAL_BIAS_FACTOR*10*(1.-dot(normal,lightDir)),DIRECTIONAL_BIAS_FACTOR);// dynamic bias
  
  vec3 projCoords=v_lightSpaceFragPositions[shadowMapIdx].xyz/v_lightSpaceFragPositions[shadowMapIdx].w;
  projCoords=projCoords*.5+.5;// transform to [0,1] range from [-1,1]
  float currentDepth=projCoords.z;
  
  // if currentDepth is greater than 1 or less than 0, then the fragment is outside the light's frustum
  if(currentDepth>1.||currentDepth<0.){
    return 0.;
  }
  
  // PCF (using a 3x3 kernel/filter)
  vec2 texelSize=1./textureSize(u_shadowMaps[shadowMapIdx],0);// texture size on mipmap lvl 0
  float shadow=0.;
  for(int x=-1;x<=1;++x){
    for(int y=-1;y<=1;++y){
      vec2 offset=vec2(x,y)*texelSize;
      float pcfDepth=texture(u_shadowMaps[shadowMapIdx],projCoords.xy+offset).r;
      shadow+=currentDepth-bias>pcfDepth?1.:0.;
    }
  }
  shadow/=9.;
  
  // float closestDepth=texture(u_shadowMaps[shadowMapIdx],projCoords.xy).r;
  // float shadow=(currentDepth-bias>closestDepth?1.:0.);// adjusted shadow map depth
  return shadow;
}

float ComputeOmniDirectionalShadow(int shadowMapIdx,vec3 lightDir,vec3 lightPosition,vec3 normal,float farPlane){
  if(shadowMapIdx<0){
    return 0.;
  }
  
  const int numSamples=20;
  float diskRadius=.02;
  
  vec3 fragToLight=v_fragPos-lightPosition;
  
  float bias=max(OMNIDIRECTIONAL_BIAS_FACTOR*10*(1.-dot(normal,lightDir)),OMNIDIRECTIONAL_BIAS_FACTOR);// dynamic bias
  
  float shadow=0.;
  float currentDepth=length(fragToLight);
  
  for(int i=0;i<numSamples;++i)
  {
    float closestDepth=texture(u_shadowCubeMaps[shadowMapIdx],fragToLight+sampleOffsetDirections[i]*diskRadius).r;// [0, 1]
    closestDepth*=farPlane;// bring back to [0, farPlane]
    shadow+=(currentDepth-bias>closestDepth?1.:0.);// adjusted shadow map depth
  }
  return shadow/float(numSamples);
}

// float ComputeDirectionalPerspectiveShadow(int shadowMapIdx,vec3 lightDir,vec3 lightPosition,vec3 normal,float farPlane){
  //   if(shadowMapIdx<0){
    //     return 0.;
  //   }
  
  //   vec3 fragToLight=v_fragPos-lightPosition;
  
  //   // float bias=max(DIRECTIONAL_BIAS_FACTOR*10*(1.-dot(normal,lightDir)),DIRECTIONAL_BIAS_FACTOR);// dynamic bias
  //   float bias=0.;
  
  //   float currentDepth=length(fragToLight);
  
  //   // vec2 texelSize=1./textureSize(u_shadowMaps[shadowMapIdx],0);// texture size on mipmap lvl 0
  //   // float shadow=0.;
  //   // for(int x=-1;x<=1;++x){
    //     //   for(int y=-1;y<=1;++y){
      //       //     vec2 offset=vec2(x,y)*texelSize;
      //       //     float pcfDepth=texture(u_shadowMaps[shadowMapIdx],fragToLight.xy+offset).r;
      //       //     pcfDepth*=farPlane;// bring back to [0, farPlane]
      //       //     shadow+=currentDepth-bias>pcfDepth?1.:0.;
    //     //   }
  //   // }
  //   // shadow/=9.;
  
  //   float closestDepth=texture(u_shadowMaps[shadowMapIdx],fragToLight.xy).r;
  //   closestDepth*=farPlane;// bring back to [0, farPlane]
  //   float shadow=(currentDepth-bias>closestDepth?1.:0.);// adjusted shadow map depth
  //   return shadow;
// }

float ComputeAttenuation(float constant,float linear,float quadratic,float distance){
  return 1./(constant+linear*distance+quadratic*distance*distance);
}

vec3 ComputeAmbientComponent(vec3 color,float ambientIntensity){
  if(u_material.numDiffuseMaps>0){
    return color*ambientIntensity*texture2D(u_material.diffuseMaps[0],v_uv).xyz;
  }else{
    return color*ambientIntensity*u_material.albedo;
  }
}

vec3 ComputeDiffuseComponent(vec3 color,float diffuseIntensity,float diffuseFactor){
  if(u_material.numDiffuseMaps>0){
    vec3 diffuse=vec3(0.f);
    for(int j=0;j<u_material.numDiffuseMaps;j++){
      diffuse+=(color*diffuseIntensity*diffuseFactor*texture2D(u_material.diffuseMaps[j],v_uv).xyz);
    }
    return(diffuse);
  }
  else{
    return(color*diffuseIntensity*diffuseFactor*u_material.albedo);
  }
}

vec3 ComputeSpecularComponent(vec3 color,float specularIntensity,float specularFactor){
  if(u_material.numSpecularMaps>0){
    vec3 specular=vec3(0.f);
    for(int j=0;j<u_material.numSpecularMaps;j++){
      specular+=(color*specularIntensity*specularFactor*texture2D(u_material.specularMaps[j],v_uv).xyz);
    }
    return specular;
  }
  else{
    return(color*specularIntensity*specularFactor*u_material.specular);
  }
}

vec3 ComputeAmbientLight(AmbientLight ambientLight){
  return ComputeAmbientComponent(ambientLight.color,ambientLight.ambientIntensity);
}

vec3 ComputeDirectionalLight(DirectionalLight directionalLight,vec3 normal,vec3 viewDir){
  vec3 lightDir=normalize(-directionalLight.direction);
  float diffuseFactor=max(dot(normal,lightDir),0.f);
  
  vec3 reflectDir=reflect(-lightDir,normal);
  float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
  
  float shadow=ComputeDirectionalShadow(directionalLight.shadowMapIdx,lightDir,normal);
  
  return(ComputeAmbientComponent(directionalLight.color,directionalLight.ambientIntensity)+(1-shadow)*(
    ComputeDiffuseComponent(directionalLight.color,directionalLight.diffuseIntensity,diffuseFactor)+
    ComputeSpecularComponent(directionalLight.color,directionalLight.specularIntensity,specularFactor)));
  }
  
  vec3 ComputePointLight(PointLight pointLight,vec3 normal,vec3 viewDir){
    vec3 lightDir=normalize(pointLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
    
    // attenuation
    float distance=length(pointLight.position-v_fragPos);
    float attenuation=ComputeAttenuation(pointLight.constant,pointLight.linear,pointLight.quadratic,distance);
    
    float shadow=ComputeOmniDirectionalShadow(pointLight.shadowMapIdx,lightDir,pointLight.position,normal,pointLight.farPlane);
    // float shadow=0.;
    
    return(ComputeAmbientComponent(pointLight.color,pointLight.ambientIntensity)+
    (1-shadow)*(ComputeDiffuseComponent(pointLight.color,pointLight.diffuseIntensity,diffuseFactor)+
    ComputeSpecularComponent(pointLight.color,pointLight.specularIntensity,specularFactor)))*attenuation;
  }
  
  vec3 ComputeSpotLight(SpotLight spotLight,vec3 normal,vec3 viewDir){
    vec3 lightDir=normalize(spotLight.position-v_fragPos);
    float diffuseFactor=max(dot(normal,lightDir),0.f);
    
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularFactor=pow(max(dot(viewDir,reflectDir),0.),u_material.metalness);
    
    // attenuation
    float distance=length(spotLight.position-v_fragPos);
    float attenuation=ComputeAttenuation(spotLight.constant,spotLight.linear,spotLight.quadratic,distance);
    
    float phi=cos(radians(spotLight.innerCutoff));
    float gamma=cos(radians(spotLight.outerCutoff));
    float theta=dot(lightDir,normalize(-spotLight.direction));
    float epsilon=phi-gamma;
    float intensity=clamp((theta-gamma)/epsilon,0.,1.);
    
    float shadow=ComputeDirectionalShadow(spotLight.shadowMapIdx,lightDir,normal);
    
    return(ComputeAmbientComponent(spotLight.color,spotLight.ambientIntensity)+
    (1-shadow)*(ComputeDiffuseComponent(spotLight.color,spotLight.diffuseIntensity,diffuseFactor*intensity)+
    ComputeSpecularComponent(spotLight.color,spotLight.specularIntensity,specularFactor*intensity)))*attenuation;
  }
  
  void main(){
    if(u_debugNormals){
      pixelColor=v_color;
      return;
    }
    
    vec3 normal=normalize(v_normal);
    vec3 viewDir=normalize(u_viewPos-v_fragPos);
    
    vec3 cumulative=vec3(0.f);
    
    // ambient light
    // TODO: Add support for ambient maps
    if(u_numAmbientLights==1){
      cumulative+=ComputeAmbientLight(u_ambientLight);
    }
    
    // directional light
    for(int i=0;i<u_numDirectionalLights;i++){
      cumulative+=ComputeDirectionalLight(u_directionalLights[i],normal,viewDir);
    }
    
    // point light
    for(int i=0;i<u_numPointLights;i++){
      cumulative+=ComputePointLight(u_pointLights[i],normal,viewDir);
    }
    
    // spot light
    for(int i=0;i<u_numSpotLights;i++){
      cumulative+=ComputeSpotLight(u_spotLights[i],normal,viewDir);
    }
    
    // TODO: Opactiy map needs to be accounted for
    pixelColor=vec4(cumulative,u_material.opacity);
    
    // gamma correction
    float gamma=2.2;
    pixelColor.rgb=pow(pixelColor.rgb,vec3(1./gamma));
  }
  