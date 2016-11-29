#version 410

in vec3 vertex_position;
in vec3 vertex_color;
in vec3 vertex_normal;
in vec2 vertex_uv;

uniform mat4 modelview_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat3 transpose_inverse_modelview_matrix;

out vec4 world_model_position;
out vec3 normal_direction;
out vec3 fragment_color;

out vec2 fragment_vertex_uv;

void main () {
  fragment_vertex_uv = vec2(vertex_uv.x, vertex_uv.y);

  world_model_position = modelview_matrix * vec4(vertex_position, 1.0);
  normal_direction = normalize(transpose_inverse_modelview_matrix * vertex_normal);
  fragment_color = vertex_color;

  gl_Position = projection_matrix * view_matrix * world_model_position;
}