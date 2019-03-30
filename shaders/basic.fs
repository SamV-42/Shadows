#version 330 core

in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
//  if(gl_FrontFacing) {
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    color = vec4(texture(skybox, R).rgb, 1.0);
//  } else {
  //  color = vec4(0.8f, 0.6f, 0.3f, 1.0f);
//  }
}
