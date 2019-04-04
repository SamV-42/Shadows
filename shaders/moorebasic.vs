#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform  mat4 projection;
uniform  mat4 view;

uniform mat4 model;

void main() {
  vec4 posi = projection * view * model * vec4(position, 1.0);
  posi.z *= 0;
  posi.xy *= 2;
  posi.xy -= 1;
  gl_Position = posi;
  TexCoord = texCoord;
}
