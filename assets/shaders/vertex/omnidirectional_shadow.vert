#version 410 core

layout(location=0)in vec3 pos;

// mvp from perspective of light source
uniform mat4 u_mMatrix=mat4(1.);

void main(){
  gl_Position=u_mMatrix*vec4(pos,1.);
}