#version 330

layout(location=0)in vec3 pos;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 uv;
layout(location=3)in vec4 color;

out vec2 v_uv;
out vec4 v_color;

uniform mat4 u_mvp=mat4(1.);

void main(){
  v_uv=uv;
  v_color=color;
  gl_Position=u_mvp*vec4(pos,1.);
}