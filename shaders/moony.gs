#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_TO_FS {
  vec2 TexCoord;
  vec3 Normal;
  vec3 FragPos;
  vec3 fakeNormal;
} vs_to_gs_var_in[];

const float MAGNITUDE = 0.02;

void makeLine(vec4 point, vec3 norm) {
  gl_Position = point;
  EmitVertex();
  gl_Position = point + vec4(MAGNITUDE * norm, 0.0);
  EmitVertex();
  EndPrimitive();
}



void main() {
  makeLine(gl_in[0].gl_Position, vs_to_gs_var_in[0].fakeNormal);
  makeLine(gl_in[1].gl_Position, vs_to_gs_var_in[1].fakeNormal);
  makeLine(gl_in[2].gl_Position, vs_to_gs_var_in[2].fakeNormal);
}
