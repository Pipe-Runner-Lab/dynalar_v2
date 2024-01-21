#version 410 core

in vec4 FragPos;
uniform vec3 u_lightPosition;
uniform float u_farPlane;

void main(){
  // get distance between fragment and light source
  float lightDistance=length(FragPos.xyz-u_lightPosition);
  // map to [0;1] range by dividing by u_farPlane
  lightDistance=lightDistance/u_farPlane;
  // write this as modified depth
  gl_FragDepth=lightDistance;
}