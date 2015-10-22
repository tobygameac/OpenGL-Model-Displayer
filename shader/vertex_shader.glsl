#version 410

attribute vec3 vertex_position;
attribute vec3 vertex_color;

uniform mat4 mvp_matrix;

out vec3 fragment_color;

void main () {
  gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
  fragment_color = vertex_color;
}
