#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_TO_FS {
  vec2 TexCoord;
  vec3 Normal;
  vec3 FragPos;
  vec3 fakeNormal;
} vs_to_fs_var1;

uniform mat4 model;
layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

uniform vec2 offsets[25];
uniform int offset_enabled;

void main()
{
    mat4 translation;
    mat4 new_model = model;
    if(offset_enabled == 1) {
        translation[0][0] = 1;
        translation[1][1] = 1;
        translation[2][2] = 1;
        translation[3][3] = 1;

        translation[3][0] = offsets[gl_InstanceID].x;
        translation[3][2] = offsets[gl_InstanceID].y;
        new_model = translation * model;
    }
    gl_Position = projection * view * new_model * vec4(position, 1.0f);
    vs_to_fs_var1.FragPos = vec3(model * vec4(position, 1.0f));
    vs_to_fs_var1.TexCoord = texCoord;
    vs_to_fs_var1.Normal = mat3(transpose(inverse(model)))*normal;
    vs_to_fs_var1.fakeNormal = normalize(vec3( projection * vec4( (mat3(transpose(inverse(view * model)))*normal) , 1.0) ));
}
