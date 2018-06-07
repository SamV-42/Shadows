#version 330 core

struct PointLight {
    vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	float shininess;
};
uniform Material material0;

uniform vec3 viewPos;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

vec3 point_Lighting(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {  //normal and viewDir are normed
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material0.shininess);

    vec3 ambient = light.ambient * vec3(texture(material0.texture_diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material0.texture_diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material0.texture_specular, TexCoord));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

void main()
{
    vec3 output;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    output = vec3(0.0f);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
        output += point_Lighting(pointLights[i], norm, FragPos, viewDir);
    }

    color = vec4(output, 1.0f);
}

