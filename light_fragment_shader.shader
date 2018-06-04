#version 330 core

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	float shininess;
};

in vec2 TexCoord;

out vec4 color;

uniform Material material0;

void main()
{
    color = vec4(mix(texture(material0.texture_diffuse, TexCoord).xyz, vec3(1.0f,1.0f,1.0f), 0.5), 1);
}
