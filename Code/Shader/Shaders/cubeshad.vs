#version 330 core

layout (location = 0) in vec3 position;

out vec3 texCoords;

layout (std140) uniform Matrices {
  uniform mat4 projection;
  uniform mat4 view;
};

uniform mat4 modifiedView;

void main() {
  vec4 pos = projection * modifiedView * vec4(position, 1.0);
  gl_Position = pos.xyww;
  texCoords = position;
}
