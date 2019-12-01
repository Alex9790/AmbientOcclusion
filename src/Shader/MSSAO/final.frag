#version 450
  
layout(location=0) out vec4 vFragColor;  //fragment shader output
 
smooth in vec2 vUV;	//interpolated texture coordinate from vertex shader

//uniforms
uniform bool esMSSAO;
uniform sampler2D textureMap; //texture map to display
uniform sampler2D textureMap2;
uniform sampler2D textureMap3;
uniform sampler2D textureMap4;
uniform sampler2D textureMap5;

uniform float intensidadTex1;
uniform float intensidadTex2;
uniform float intensidadTex3;
uniform float intensidadTex4;
uniform float intensidadTex5;


void main()
{ 	 
	//return the texture sample value at the given texture coordinate
	//vFragColor = vec4(texture(textureMap, vUV).xyz, 0.8); 
	//vFragColor = texture(textureMap, vUV);

	if(esMSSAO){
		vec4 tex1 = texture(textureMap, vUV);
		vec4 tex2 = texture(textureMap2, vUV);
		vec4 tex3 = texture(textureMap3, vUV);
		vec4 tex4 = texture(textureMap4, vUV);
		vec4 tex5 = texture(textureMap5, vUV);
/*
		vec4 mixText = mix(tex1, tex2, 0.5);
		mixText = mix(mixText, tex3, 0.5);
		mixText = mix(mixText, tex4, 0.5);
		mixText = mix(mixText, tex5, 0.5);
*/
		vec4 mixText = tex1*intensidadTex1 + tex2*intensidadTex2 + tex3*intensidadTex3 + tex4*intensidadTex4 + tex5*intensidadTex5;
		vFragColor = mixText;
	}else{
		vFragColor = texture(textureMap, vUV);
	}
}