#version 410

in vec3 fragment_color;

void main () {
  gl_FragColor = vec4(fragment_color, 1.0);
}