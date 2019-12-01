#version 450

layout(location = 0) in vec2 vVertex;
layout(location = 1) in vec3 wc_view_ray_direction;

/**
struct vertex_data
{
	vec3 wc_view_ray_direction;
};
noperspective out vertex_data vertex;
*/

noperspective out vec3 wc_view_ray_direction2;
smooth out vec2 vUV;					//interpolated texture coordinate

void main()
{
	//get clipspace position from the object space vertex position
	gl_Position = vec4(vVertex*2-1.0,0,1);

	wc_view_ray_direction2 = wc_view_ray_direction;

	//pass object space vertex position as the texture coordinate
	vUV = vVertex;
}
