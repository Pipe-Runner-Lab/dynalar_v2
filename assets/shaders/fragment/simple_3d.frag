#version 330

out vec4 pixelColor;
in vec2 v_uv;
in vec4 v_color;

struct MeshBaseMaterial{
  vec4 albedo;
};

uniform sampler2D u_textureSampler;
// https://stackoverflow.com/questions/33690186/opengl-bool-uniform
uniform bool u_shouldUseTexture=false;
uniform bool u_debugNormals=false;
uniform MeshBaseMaterial u_material;

void main(){
  if(u_debugNormals){
    pixelColor=v_color;
    return;
  }
  
  if(u_shouldUseTexture){
    pixelColor=v_color*texture2D(u_textureSampler,v_uv);
  }
  else{
    pixelColor=u_material.albedo;
  }
}
