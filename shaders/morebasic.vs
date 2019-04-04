#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};
uniform mat4 model;

uniform int flatscreen;

void main() {
  vec4 posi = projection * view * model * vec4(position, 1.0);
  posi.z *= flatscreen;
  gl_Position = posi;
  TexCoord = texCoord;
}
