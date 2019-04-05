#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_TO_FS {
  vec2 TexCoord;
  vec3 Normal;
  vec3 FragPos;
} vs_to_gs_var_in[];

out VS_TO_FS {  //huh, guess this work?
  vec2 TexCoord;
  vec3 Normal;
  vec3 FragPos;
} vs_to_gs_var_out;

uniform float time;


vec3 GetNormal() {
  vec3 a = (gl_in[0].gl_Position - gl_in[1].gl_Position).xyz ;
  vec3 b = (gl_in[0].gl_Position - gl_in[2].gl_Position).xyz ;
  return normalize(cross(b, a));
}

vec4 explode(vec4 position, vec3 normal) {
  float magnitude = 0.0f;
  float amount = magnitude * ((sin(time) + 1.0f) / 2.0f) ;
  return position + vec4(amount * normal, 0.0f);
}


void main() {
  //  /*

  //*/

  vec3 normal = GetNormal();

  vs_to_gs_var_out.TexCoord = vs_to_gs_var_in[0].TexCoord;
  vs_to_gs_var_out.Normal   = vs_to_gs_var_in[0].Normal;
  vs_to_gs_var_out.FragPos  = vs_to_gs_var_in[0].FragPos;
  gl_Position = explode(gl_in[0].gl_Position, normal);
  EmitVertex();

  vs_to_gs_var_out.TexCoord = vs_to_gs_var_in[1].TexCoord;
  vs_to_gs_var_out.Normal   = vs_to_gs_var_in[1].Normal;
  vs_to_gs_var_out.FragPos  = vs_to_gs_var_in[1].FragPos;
  gl_Position = explode(gl_in[1].gl_Position, normal);
  EmitVertex();

  vs_to_gs_var_out.TexCoord = vs_to_gs_var_in[2].TexCoord;
  vs_to_gs_var_out.Normal   = vs_to_gs_var_in[2].Normal;
  vs_to_gs_var_out.FragPos  = vs_to_gs_var_in[2].FragPos;
  gl_Position = explode(gl_in[2].gl_Position, normal);
  EmitVertex();

  EndPrimitive();
}
