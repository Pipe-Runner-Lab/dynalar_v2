#version 330

layout(location=0)in vec3 pos;

out vec3 v_color;

void main(){
  v_color=clamp(pos,0.f,1.f);
  gl_Position=vec4(pos,1.);
}