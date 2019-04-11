#version 330 core
layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;

out vec2 f_TexCoords;
out vec3 f_FragPos;
out vec3 f_Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    f_TexCoords = v_TexCoords;
    f_FragPos = vec3(model * vec4(v_Position, 1.0f));
    f_Normal = mat3(transpose(inverse(model))) * v_Normal;

    gl_Position = projection * view * model * vec4(v_Position, 1.0);
}
