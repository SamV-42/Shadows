#version 330 core

in vec2 TexCoord;

out vec4 color;

struct Material {
	sampler2D texture_diffuse;
  sampler2D texture_specular;
	float shininess;
};

uniform Material material0;

void main() {

    //color = vec4(1.0f, 0.5f, 0.75f, 1.0f);
    color = texture(material0.texture_diffuse, TexCoord);
}
