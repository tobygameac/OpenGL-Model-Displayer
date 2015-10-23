#version 410

attribute vec3 vertex_position;
attribute vec3 vertex_color;
attribute vec3 vertex_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat3 transpose_inverse_model_matrix;

varying vec4 world_model_position;
varying vec3 normal_direction;
varying vec3 fragment_color;

void main () {
  world_model_position = model_matrix * vec4(vertex_position, 1.0);
  normal_direction = normalize(transpose_inverse_model_matrix * vertex_normal);
  fragment_color = vertex_color;

  gl_Position = projection_matrix * view_matrix * world_model_position;
}
