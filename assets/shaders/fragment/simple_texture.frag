#version 330

out vec4 pixelColor;
in vec2 v_uv;

uniform sampler2D u_textureSampler;

void main(){
  pixelColor=texture2D(u_textureSampler,v_uv);
}