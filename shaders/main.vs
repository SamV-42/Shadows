#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_TO_GS {
  vec2 TexCoord;
  vec3 Normal;
  vec3 FragPos;
} vs_to_fs_var1;

uniform mat4 model;
layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_to_fs_var1.FragPos = vec3(model * vec4(position, 1.0f));
    vs_to_fs_var1.TexCoord = texCoord;
    vs_to_fs_var1.Normal = mat3(transpose(inverse(model)))*normal;
}
