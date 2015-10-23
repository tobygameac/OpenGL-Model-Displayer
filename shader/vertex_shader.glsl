#version 410

attribute vec3 vertex_position;
attribute vec3 vertex_color;
attribute vec3 vertex_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 fragment_color;

void main () {
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);
  fragment_color = vertex_color;
}
