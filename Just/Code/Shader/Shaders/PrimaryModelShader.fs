#version 330 core

struct Material {
  sampler2D texture_diffuse;
  sampler2D texture_specular;
  float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 f_FragPos;
in vec3 f_Normal;
in vec2 f_TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material0;

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  vec3 result;
  vec3 viewDir = normalize(f_FragPos - viewPos);
  vec3 norm = normalize(f_Normal);

  for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
    result += CalcPointLight(pointLights[i], material0, norm, f_FragPos, viewDir);
  }
  vec3 fragColor = result;

  float gamma = 2.2;
  fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));

  color = vec4(fragColor, 1.0f);
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 spec_vec;
  vec3 reflectDir = reflect(-lightDir, normal); spec_vec = reflectDir;
  //vec3 halfwayDir = normalize(lightDir + viewDir); spec_vec = halfwayDir;
  float spec = pow(max(dot(normal, spec_vec), 0.0), mat.shininess);

  float distance = length(light.position - fragPos);
  //float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  float attenuation = 1.0f / (distance * distance);

  float gamma = 2.2;
  vec3 ambient = light.ambient * pow(texture(mat.texture_diffuse, TexCoords).rgb, vec3(gamma));
  vec3 diffuse = light.diffuse * diff * pow(texture(mat.texture_diffuse, TexCoords).rgb, vec3(gamma));
  vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular, TexCoords));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}
