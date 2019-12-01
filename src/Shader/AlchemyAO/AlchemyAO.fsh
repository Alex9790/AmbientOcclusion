#version 450

uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D noiseTex;

uniform vec3 wc_camera_eye_position;	//posicion de la camara
//uniform float z_far;					//far del frustrum (en el ejemplo pone 10000.0f)
const float z_far = 1000.0;

uniform vec2 tc_window;	//dimension de la ventana (width, height)

uniform mat4 projection_matrix;
uniform mat4 view_projection_matrix;
//uniform mat4 inverse_view_projection_matrix;



uniform int base_samples;
uniform int min_samples;
uniform float radius;
uniform float projection_factor;	//0.75
uniform float bias;
uniform float sigma;



/**
struct vertex_data
{
	vec3 wc_camera_ray_direction;
};
noperspective in vertex_data vertex;
*/

noperspective in vec3 wc_camera_ray_direction2;

smooth in vec2 vUV;	//interpolated texture coordinates

out vec4 ambient_occlusion;

vec2 screenRes = tc_window;
uniform bool VerAO;		
uniform float NUM_SPIRAL_TURNS;		//2.0
uniform float NUM_SAMPLES;			//64
uniform float aoRad;				//0.5
uniform float aoBias;				//0.002
float pi = 3.14159265359;
vec2 focalLength = vec2(1.0/tan(45.0*0.5)*screenRes.y/screenRes.x,1.0/tan(45.0*0.5));

float linearDepth(float d, float near, float far)
{
	d = d*2.0 - 1.0;
	vec2 lin = vec2((near-far)/(2.0*near*far),(near+far)/(2.0*near*far));
	return -1.0/(lin.x*d+lin.y);
}

vec3 UVtoViewSpace(vec2 uv, float z)
{
	vec2 UVtoViewA = vec2(-2.0/focalLength.x,-2.0/focalLength.y);
	vec2 UVtoViewB = vec2(1.0/focalLength.x,1.0/focalLength.y);
	uv = UVtoViewA*uv + UVtoViewB;
	return vec3(uv*z,z);
}

vec3 GetViewPos(vec2 uv)
{
	float z = linearDepth(texture(depthTex, uv).r,0.1,100.0);
	return UVtoViewSpace(uv, z);
}

vec2 RotateDir(vec2 dir, vec2 cosSin)
{
	return vec2(dir.x*cosSin.x - dir.y*cosSin.y, dir.x*cosSin.y + dir.y*cosSin.x);
}

vec2 tapLocation(int sampleNumber, float spinAngle, out float ssR)
{
	float alpha = float(sampleNumber + 0.5) * (1.0 / NUM_SAMPLES);
	float angle = alpha * (NUM_SPIRAL_TURNS * 6.28) + spinAngle;
	ssR = alpha;
	return vec2(cos(angle), sin(angle));
}

void main()
{
	float ao = 0.0;
	//float aoRad = 0.5;
	//float aoBias = 0.002;	
	//vec2 aoTexCoord = gl_FragCoord.xy/vec2(screenRes.x,screenRes.y);
	vec2 aoTexCoord = vUV;

	if(texture(depthTex, aoTexCoord).r < 1.0)
	{
//		vec2 aoRes = vec2(screenRes.x,screenRes.y);
//		vec3 aoRand = texture(noiseTex, aoTexCoord*aoRes/1).xyz;	//aoRandTex
		vec3 aoC = GetViewPos(aoTexCoord);
		vec3 aoN_C = normalize(cross(dFdx(aoC),dFdy(aoC)));
		float uvDiskRadius = 0.5*focalLength.y*aoRad/aoC.z;
		vec2 pixelPosC = gl_FragCoord.xy;
		pixelPosC.y = screenRes.y-pixelPosC.y;
		float randPatternRotAngle = (3*int(pixelPosC.x)^int(pixelPosC.y)+int(pixelPosC.x)*int(pixelPosC.y))*10.0;
//		float aoAlpha = 2.0*pi/NUM_SAMPLES + randPatternRotAngle;
		for(int i = 0; i < NUM_SAMPLES; ++i)
		{
			float ssR;
			vec2 aoUnitOffset = tapLocation(i, randPatternRotAngle, ssR);
			ssR *= uvDiskRadius;
			vec2 aoTexS = aoTexCoord + ssR*aoUnitOffset;
			vec3 aoQ = GetViewPos(aoTexS);
			vec3 aoV = aoQ - aoC;
			float aoVv = dot(aoV,aoV);
			float aoVn = dot(aoV, aoN_C);
			float aoEp = 0.01;
			float aoF = max(aoRad*aoRad - aoVv, 0.0);
			ao += aoF*aoF*aoF*max((aoVn - aoBias)/(aoEp+aoVv),0.0);

			//ao += float(aoVv < (aoRad*aoRad)) * max((aoVn - aoBias) / (aoEp + aoVv), 0.0) * (aoRad*aoRad) * 0.6;
		}
		float aoTemp = aoRad*aoRad*aoRad;
		ao /= aoTemp*aoTemp;
		ao = max(0.0,1.0 - ao*(1.0/NUM_SAMPLES));
		float A = 1.0;
		if(VerAO){
			if(ao >= 0.9){
				A = 0.0;
			}else{
				A = 1.0;												
			}
			ao = 0.0;
		}
		ambient_occlusion = vec4(vec3(ao),A);		
	}
}