#version 410

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat4 inverse_modelview_matrix;

uniform lowp float texture_flag;
uniform sampler2D texture;

in vec4 world_model_position;
in vec3 normal_direction;
in vec3 fragment_color;
in vec2 fragment_vertex_uv;

vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

Material front_material = Material(
  vec4(0.2, 0.2, 0.2, 1.0),
  texture_flag * texture2D(texture, fragment_vertex_uv) + (1.0 - texture_flag) * vec4(fragment_color, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
  );

Material back_material = Material(
  vec4(0.2, 0.2, 0.2, 1.0),
  (texture_flag * texture2D(texture, fragment_vertex_uv) + (1.0 - texture_flag) * vec4(fragment_color, 1.0)),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
  );

struct Light {
  vec4 position;
  vec4 diffuse;
  vec4 specular;

  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;

  float spot_cutoff;
  float spot_exponent;

  vec3 spot_direction;
};

const int NUMBER_OF_LIGHTS = 2;
Light lights[NUMBER_OF_LIGHTS];

void main () {
  lights[0] = Light(
    vec4(1.0, 1.0, 1.0, 0.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    0.0, 1.0, 0.0,
    180.0, 0.0,
    vec3(0.0, 0.0, 0.0)
    );

  lights[1] = Light(
    vec4(-1.0, -1.0, -1.0, 0.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    0.0, 1.0, 0.0,
    180.0, 10.0,
    vec3(0.0, 0.0, 0.0)
    );

  vec3 normal_direction = normalize(normal_direction);
  vec4 material_ambient;
  vec4 material_diffuse;
  vec4 material_specular;
  float material_shininess;

  if (gl_FrontFacing) {
    material_ambient = front_material.ambient;
    material_diffuse = front_material.diffuse;
    material_specular = front_material.specular;
    material_shininess = front_material.shininess;
  } else {
    material_ambient = back_material.ambient;
    material_diffuse = back_material.diffuse;
    material_specular = back_material.specular;
    material_shininess = back_material.shininess;
    normal_direction = -normal_direction;
  }

  vec3 view_direction = normalize(vec3(inverse_modelview_matrix * vec4(0.0, 0.0, 0.0, 1.0) - world_model_position));
  vec3 light_direction;
  float attenuation;

  vec3 total_intensity = vec3(scene_ambient) * vec3(material_ambient);

  for (int index = 0; index < NUMBER_OF_LIGHTS; ++index) {
    if (lights[index].position.w == 0.0) { // directional light
      attenuation = 1.0;
      light_direction = normalize(vec3(lights[index].position));
    } else {  
      vec3 position_to_light_source = vec3(lights[index].position - world_model_position);
      float distance_to_light_source = length(position_to_light_source);
      light_direction = normalize(position_to_light_source);
      attenuation = 1.0 / (lights[index].constant_attenuation
        + lights[index].linear_attenuation * distance_to_light_source
        + lights[index].quadratic_attenuation * distance_to_light_source * distance_to_light_source);

      if (lights[index].spot_cutoff <= 90.0) { // spotlight
        float clamped_cosine = max(0.0, dot(-light_direction, normalize(lights[index].spot_direction)));
        if (clamped_cosine < cos(radians(lights[index].spot_cutoff))) {
          attenuation = 0.0;
        } else {
          attenuation = attenuation * pow(clamped_cosine, lights[index].spot_exponent);   
        }
      }
    }

    vec3 diffuse_reflection = attenuation * vec3(lights[index].diffuse) * vec3(material_diffuse) * max(0.0, dot(normal_direction, light_direction));

    vec3 specular_reflection;
    if (dot(normal_direction, light_direction) < 0.0) { // wrong side
      specular_reflection = vec3(0.0, 0.0, 0.0);
    } else {
      specular_reflection = attenuation * vec3(lights[index].specular) * vec3(material_specular)
        * pow(max(0.0, dot(reflect(-light_direction, normal_direction), view_direction)), material_shininess);
    }

    total_intensity = total_intensity + diffuse_reflection + specular_reflection;
  }

  gl_FragColor = vec4(total_intensity, 1.0);
  //gl_FragColor = front_material.diffuse;
}