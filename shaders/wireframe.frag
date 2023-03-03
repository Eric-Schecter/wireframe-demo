#version 460

in GS_FS_INTERFACE
{
  vec3 barycentric;
} fs_in;

const float lineWidth=2.;
const vec3 color = vec3(1.f,0.f,0.f);

out vec4 fColor;

float computeAlpha(vec3 barycentric){
    vec3 d=fwidth(barycentric);
    vec3 a3=smoothstep(vec3(0.),d*lineWidth,barycentric);
    return mix(1.,0.,min(min(a3.x,a3.y),a3.z));
}

void main() {
    float alpha = computeAlpha(fs_in.barycentric);
    fColor = vec4(color,alpha);
} 
