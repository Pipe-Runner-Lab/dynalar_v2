#version 410 core

void main(){
  // * This line can be commented out to get the same result
  gl_FragDepth=gl_FragCoord.z;
}