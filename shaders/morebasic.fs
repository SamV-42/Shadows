#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D text;

void main() {
  color = texture(text, TexCoord);
}
