#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
}gl_in[];  

out gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

out GS_FS_INTERFACE
{
  vec3 barycentric;
} my_gs_out;

void main(){
	for(int i =0;i<gl_in.length();i++){
		gl_Position =gl_in[i].gl_Position;
		my_gs_out.barycentric = vec3(i%3== 0 ? 1 : 0,i%3==1 ? 1 : 0,i%3==2 ? 1 : 0);
		EmitVertex();
	}
	EndPrimitive();
}