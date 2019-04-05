#version 330 core

layout (location = 0) in float position;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

void main() {
  gl_Position = projection * view * vec4((position + -8.0), 0.0, -3.0, 1.0);
}
