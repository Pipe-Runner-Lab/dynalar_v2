#version 330

out vec4 pixelColor;
in vec3 v_color;

void main(){
  pixelColor=vec4(v_color,1.);
}