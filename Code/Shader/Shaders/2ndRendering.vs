#version 330 core

layout (location = 0) in vec2 position;

out vec2 TexCoord;

void main()
{
  gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
  TexCoord = 0.5f*(position+vec2(1.0f,1.0f));
}
