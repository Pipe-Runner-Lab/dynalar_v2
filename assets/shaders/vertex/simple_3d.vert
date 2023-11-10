#version 330

layout(location=0)in vec3 pos;
layout(location=1)in vec2 uv;

out vec2 v_uv;

uniform mat4 mvp;

void main(){
  v_uv=uv;
  gl_Position=mvp*vec4(pos,1.);
}