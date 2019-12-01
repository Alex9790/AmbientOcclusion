#version 450

in vec3 Posicion;
in vec3 Normal;
in vec2 TexCoord;

in vec3 LightDir[4];
in vec3 ViewDir;

uniform sampler2D TexBase;
uniform sampler2D TexMulti;
uniform sampler2D NormalMapTex;
uniform samplerCube CubeMapTex;

in vec3 ReflectDir; 
uniform bool DrawSkyBox;

uniform bool VerSoloAO;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light[4];

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void phongModel(int Index, vec3 norm, out vec3 ambAndDiff, out vec3 spec )
{
	vec3 n = normalize( norm );

	//direccion normalizada hacia la fuente de luz
	//vec3 s = normalize( vec3(Light[Index].Position) - Posicion );
	vec3 s;
	if( Light[Index].Position.w == 0.0 ){
		s = normalize(vec3(Light[Index].Position));
	}else{
		s = normalize( vec3(Light[Index].Position) - Posicion );
	}
	//direccion hacia el espectador
	vec3 v = normalize(vec3(-Posicion));

	//direccion de refleccion pura
	vec3 r = reflect( -s, n );

	float sDotn = max( dot(s,n), 0.0 );
	float rDotv = max( dot(r,v), 0.0 );

	vec3 ambient = Light[Index].Intensity * Material.Ka;
	vec3 diffuse = Light[Index].Intensity * Material.Kd * sDotn;
	spec = vec3(0.0);

	if( sDotn > 0.0 )
        spec = Light[Index].Intensity * Material.Ks * pow( rDotv, Material.Shininess );

    ambAndDiff = ambient + diffuse;
	//return LightIntensity * ( Ka + ( Kd * sDotn ) + ( Ks * pow(rDotv, Shininess) ) );
}

void main() {

	vec3 ambAndDiff, spec, ambAndDiffTotal, specTotal;
    vec4 texColorBase = texture( TexBase, TexCoord );
	vec4 texColorMulti = texture( TexMulti, TexCoord );
	vec4 texNormal = texture( NormalMapTex, TexCoord );
	vec4 cubeMapColor = texture (CubeMapTex, ReflectDir);

	vec4 texColor = mix(texColorBase, texColorMulti, 0.4);
	
	vec4 Color;
		
	if( DrawSkyBox ){
		FragColor = cubeMapColor;
	}else{

		if(VerSoloAO){
			FragColor = vec4(1,1,1,1); return;
		}

		if( gl_FrontFacing ) {
			//3 point light y 1 directional light
			phongModel(0, Normal, ambAndDiff, spec);
			ambAndDiffTotal = ambAndDiff;
			specTotal = spec;

			phongModel(1, Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;

			phongModel(2, Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;

			phongModel(3, Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;
				
			Color = Color + (vec4( ambAndDiffTotal, 1.0 ) * texColor) + vec4(specTotal,1.0);
			FragColor = Color;
			//FragColor = vec4(1,1,1,1);

		} else {
			//3 point light y 1 directional light
			phongModel(0, -Normal, ambAndDiff, spec);
			ambAndDiffTotal = ambAndDiff;
			specTotal = spec;

			phongModel(1, -Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;

			phongModel(2, -Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;

			phongModel(3, -Normal, ambAndDiff, spec);
			ambAndDiffTotal += ambAndDiff;
			specTotal += spec;
				
			Color = Color + (vec4( ambAndDiffTotal, 1.0 ) * texColor) + vec4(specTotal,1.0);
			FragColor = Color;
			
		}
	}
}