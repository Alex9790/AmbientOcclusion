#version 450

uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D noiseTex;

uniform vec3 wc_camera_eye_position;	//posicion de la camara
//uniform float z_far;					//far del frustrum (en el ejemplo pone 10000.0f)
const float z_far = 1000.0;

uniform vec2 tc_window;	//dimension de la ventana (width, height)

smooth in vec2 vUV;	//interpolated texture coordinates

out vec4 ambient_occlusion;

uniform mat4 view_matrix ;
uniform mat4 inverse_view_projection_matrix;


const float PI = 3.14159265359;

// number of directions to sample in UV space
uniform int NUM_SAMPLE_DIRECTIONS;

// number of sample steps when raymarching along a direction
uniform int NUM_SAMPLE_STEPS;


float uFOV = 60.0;
uniform float uSampleRadius;
uniform float uAngleBias;
uniform float uIntensity;
vec2  uNoiseScale = vec2(1.0, 1.0);

//////////////////////////////////////////////

vec2 focalLength = vec2(1.0/tan(45.0*0.5)*tc_window.y/tc_window.x,1.0/tan(45.0*0.5));

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
	float z = linearDepth(texture(depthTex, uv).r,0.01,1000.0);
	return UVtoViewSpace(uv, z);
}

//////////////////////////////////////////////

/*vec2 snapUV(vec2 uv) {
  vec2 temp = uv * viewportResolution;
  temp = floor(temp) + step(vec2(0.5), ceil(temp));
  
  return temp * (1.0 / viewportResolution);
}*/

// reconstructs view-space unit normal from view-space position
vec3 reconstructNormalVS(vec3 positionVS) {
  return normalize(cross(dFdx(positionVS), dFdy(positionVS)));
}

vec3 getPositionViewSpace(vec2 uv) {
  //float depth = decodeGBufferDepth(sGBuffer, uv, clipFar);
  float depth = texture(depthTex, uv).r;
  
  vec2 uv2  = uv * 2.0 - vec2(1.0);
  vec4 temp = inverse_view_projection_matrix * vec4(uv2, -1.0, 1.0);
  vec3 cameraFarPlaneWS = (temp / temp.w).xyz;
  
  vec3 cameraToPositionRay = normalize(cameraFarPlaneWS - wc_camera_eye_position);
  vec3 originWS = cameraToPositionRay * depth + wc_camera_eye_position;
  vec3 originVS = (view_matrix * vec4(originWS, 1.0)).xyz;
  
  return originVS;
}

void main() {
  const float TWO_PI = 2.0 * PI;
  
  //vec3 originVS = getPositionViewSpace(vUV);
  //vec3 normalVS = reconstructNormalVS(originVS);

	//vec2 aoTexCoord = gl_FragCoord.xy/vec2(tc_window.x,tc_window.y);
	vec2 aoTexCoord = vUV;
	if(texture(depthTex, aoTexCoord).r < 1.0)
	{
		vec3 aoC = GetViewPos(aoTexCoord);											// World space point being shaded
		vec3 aoN_C = normalize(cross(dFdx(aoC),dFdy(aoC)));

		//vec2 tc_depths = gl_FragCoord.xy / tc_window;
		//vec3 wc_normal = texture( normalTex, aoTexCoord ).xyz;
		//wc_normal = normalize(texture(normalTex, aoTexCoord).xyz*2.0 - 1.0);

		vec3 originVS = aoC;
		vec3 normalVS = aoN_C;
	  
		float radiusSS = 0.0; // radius of influence in screen space
		float radiusWS = 0.0; // radius of influence in world space

		radiusSS = uSampleRadius;
		  vec4 temp0 = inverse_view_projection_matrix * vec4(0.0, 0.0, -1.0, 1.0);
		  vec3 out0  = temp0.xyz;
		  vec4 temp1 = inverse_view_projection_matrix * vec4(radiusSS, 0.0, -1.0, 1.0);
		  vec3 out1  = temp1.xyz;
  
		  // NOTE (travis): empirically, the main introduction of artifacts with HBAO 
		  // is having too large of a world-space radius; attempt to combat this issue by 
		  // clamping the world-space radius based on the screen-space radius' projection
		  radiusWS = min(tan(radiusSS * uFOV / 2.0) * originVS.y / 2.0, length(out1 - out0));
  
		  // early exit if the radius of influence is smaller than one fragment
		  // since all samples would hit the current fragment.
		  /*if (radiusSS < 1.0 / tc_window.x) {
			ambient_occlusion = vec4(1.0, 1.0, 1.0, 1.0);
			return;
		  }*/
  
		  const float theta = TWO_PI / float(NUM_SAMPLE_DIRECTIONS);
		  float cosTheta = cos(theta);
		  float sinTheta = sin(theta);
  
		  // matrix to create the sample directions
		  mat2 deltaRotationMatrix = mat2(cosTheta, -sinTheta, sinTheta, cosTheta);
  
		  // step vector in view space
		  vec2 deltaUV = vec2(1.0, 0.0) * (radiusSS / (float(NUM_SAMPLE_DIRECTIONS * NUM_SAMPLE_STEPS) + 1.0));
  
		  // we don't want to sample to the perimeter of R since those samples would be 
		  // omitted by the distance attenuation (W(R) = 0 by definition)
		  // Therefore we add a extra step and don't use the last sample.
		  vec3 sampleNoise    = texture2D(noiseTex, vUV * uNoiseScale).xyz;
		  sampleNoise.xy      = sampleNoise.xy * 2.0 - vec2(1.0);
		  mat2 rotationMatrix = mat2(sampleNoise.x, -sampleNoise.y, 
									 sampleNoise.y,  sampleNoise.x);
  
		  // apply a random rotation to the base step vector
		  deltaUV = rotationMatrix * deltaUV;
  
		  float jitter = sampleNoise.z;
		  float occlusion = 0.0;
		  float occlusion2 = 0.0;

		  float oldAngle1   = uAngleBias;
		float oldAngle2   = uAngleBias;
  
	  for (int i = 0; i < NUM_SAMPLE_DIRECTIONS; ++i) {
		// incrementally rotate sample direction
		deltaUV = deltaRotationMatrix * deltaUV;
    
		vec2 sampleDirUV = deltaUV;
		
    
		for (int j = 0; j < NUM_SAMPLE_STEPS; ++j) {
			//Direccion Original
			vec2 sampleUV     = aoTexCoord + (jitter + float(j)) * sampleDirUV;
			vec3 sampleVS     = getPositionViewSpace(sampleUV);
			vec3 sampleDirVS  = (sampleVS - originVS);
      
			// angle between fragment tangent and the sample
			float gamma = (PI / 2.0) - acos(dot(normalVS, normalize(sampleDirVS)));
      
			if (gamma > oldAngle1) {
				float value = sin(gamma) - sin(oldAngle1);
        
				occlusion += value;

				oldAngle1 = gamma;
			}

			//Direccion Reflejo
			sampleUV     = aoTexCoord + (jitter + float(j)) * (-sampleDirUV);
			sampleVS     = getPositionViewSpace(sampleUV);
			sampleDirVS  = (sampleVS - originVS);
      
			// angle between fragment tangent and the sample
			float gamma2 = (PI / 2.0) - acos(dot(normalVS, normalize(sampleDirVS)));
      
			if (gamma2 > oldAngle2) {
	  			float value = sin(gamma2) - sin(oldAngle2);
        
				occlusion2 += value;
	
				oldAngle2 = gamma2;
			}
		}
	  }

		occlusion = occlusion + occlusion2;
		occlusion /= NUM_SAMPLE_DIRECTIONS * PI;

		//occlusion = 1.0 - occlusion / float(NUM_SAMPLE_DIRECTIONS);
		//occlusion = clamp(pow(occlusion, 1.0 + uIntensity), 0.0, 1.0);
		//gl_FragColor = vec4(occlusion, occlusion, occlusion, 1.0);

		occlusion = max(0.0, 1.0 - occlusion * uIntensity * (5.0 / NUM_SAMPLE_DIRECTIONS));
		//occlusion = 1.0 - occlusion;

		//ambient_occlusion = vec4(occlusion, occlusion, occlusion, 1.0);
	  
		ambient_occlusion = vec4(0.0, 0.0, 0.0, occlusion);
	}
}