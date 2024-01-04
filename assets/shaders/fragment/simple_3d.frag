#version 330

out vec4 pixelColor;
in vec2 v_uv;
in vec4 v_color;

struct MeshBaseMaterial{
  vec4 albedo;
};

uniform bool u_debugNormals=false;
uniform MeshBaseMaterial u_material;

void main(){
  if(u_debugNormals){
    pixelColor=v_color;
    return;
  }
  
  pixelColor=u_material.albedo;
}
