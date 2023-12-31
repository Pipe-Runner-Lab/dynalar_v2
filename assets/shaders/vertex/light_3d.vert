#version 410 core

layout(location=0)in vec3 pos;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 uv;

out vec2 v_uv;
out vec4 v_color;
out vec3 v_normal;
out vec3 v_fragPos;// position in model space

uniform mat4 u_mvpMatrix=mat4(1.);
uniform mat4 u_mMatrix=mat4(1.);
uniform bool u_debugNormals=false;

void main(){
  v_uv=uv;
  gl_Position=u_mvpMatrix*vec4(pos,1.);
  v_normal=mat3(transpose(inverse(u_mMatrix)))*normal;// normal in world space
  v_fragPos=(u_mMatrix*vec4(pos,1.)).xyz;
  
  if(u_debugNormals){
    vec3 scaled_normal=normalize((u_mMatrix*vec4(v_normal,1)).xyz)+vec3(1.)*.5;
    v_color=vec4(scaled_normal,1.);
  }
}