#version 460

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aBarycentric;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;

out vec3 vBarycentric;

// explicitly declare when use program pipeline
out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};  

void main() {
    vBarycentric = aBarycentric;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition,1.f);
}
