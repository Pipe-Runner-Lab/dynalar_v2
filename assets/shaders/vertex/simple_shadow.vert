#version 410 core

layout(location=0)in vec3 pos;

// mvp from perspective of light source
uniform mat4 u_mvpMatrix=mat4(1.);
uniform int u_shadowMapType=0;

void main(){
  gl_Position=u_mvpMatrix*vec4(pos,1.);
}