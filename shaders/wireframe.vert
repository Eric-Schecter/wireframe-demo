#version 460

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aBarycentric;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;

// explicitly declare when use program pipeline
out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};  

out VS_GS_INTERFACE
{
  vec3 barycentric;
} vs_out;

void main() {
    vs_out.barycentric = aBarycentric;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition,1.f);
}
