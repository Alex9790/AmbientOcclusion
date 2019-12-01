#version 450
 
layout(location = 0) in vec3 vVertex;	//object space vertex
layout(location = 1) in vec3 vNormal;	//object space normal
 
//uniforms 
uniform mat4 MVP;				//combined modelview projection matrix
uniform mat3 NormalMatrix;		//normal matrix

smooth out vec3 vEyeSpaceNormal;   //output eye space normal  

void main()
{     
	//get eye space normal by multiplying the object space normal
	//with the normal matrix
	vEyeSpaceNormal = NormalMatrix*vNormal;
	  
	//get the clipspace position
	gl_Position = MVP*vec4(vVertex,1); 
}