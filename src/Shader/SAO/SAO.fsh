#version 450

uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D noiseTex;

uniform vec2 tc_window;	//dimension de la ventana (width, height)

smooth in vec2 vUV;	//interpolated texture coordinates

out vec4 ambient_occlusion;

uniform int NUM_SAMPLES;		//Cantidad de muestras directas a tomar de cada pixel

uniform int NUM_SPIRAL_TURNS;	//Cantidad de vueltass alrededor del circulo q hace el patron en espiral
	
uniform int VARIATION;			//seleccionar las diferentes funciones de caida (falloff fuction)

//uniforms
//uniform float intensity;
uniform float sampleRadiusWS;	//radio
uniform float bias;				//bias para evitar AO en curvas suaves
uniform float zNear;			
uniform float zFar;
vec4 projInfo = vec4( 1.0, 1.0, 1.0, 0.0 );
//uniform vec4 projInfo;
vec2 viewportResolution = tc_window;
uniform float projScale;			//100.0 // how many pixels a 1m size 1m away from the camera is.  Or 500 is a good value.
uniform float           intensityDivR6;		/** intensity / radius^6 */

//////////////////////////////////////////////

float pi = 3.14159265359;
vec2 focalLength = vec2(1.0/tan(45.0*0.5)*viewportResolution.y/viewportResolution.x,1.0/tan(45.0*0.5));

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
	float z = linearDepth(texture(depthTex, uv).r,zNear,zFar);
	return UVtoViewSpace(uv, z);
}

//////////////////////////////////////////////

// RGBA depth
float unpackDepth( const in vec4 rgba_depth ) {

    const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ), 1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );
    float depth = dot( rgba_depth, bit_shift );
    return depth;
	//return 2.0*zNear / ( zFar+zNear - depth*(zFar-zNear) ); // regular depth buffer
}

/*
    Clipping plane constants for use by reconstructZ
    clipInfo = (z_f == -inf()) ? Vector3(z_n, -1.0f, 1.0f) : Vector3(z_n * z_f,  z_n - z_f,  z_f);
*/
float reconstructCSZ(float depth) {
//      return clipInfo[0] / (clipInfo[1] * d + zFar[2]);
    return ( zNear * zFar ) / ( (zNear - zFar) * depth + zFar );
}

vec3 reconstructCSPosition(vec2 S, float z) {
    return vec3((S.xy * projInfo.xy + projInfo.zw) * z, z);
}

/** Reconstructs screen-space unit normal from screen-space position */
vec3 reconstructCSFaceNormal(vec3 C) {
    return normalize(cross(dFdx(C), dFdy(C)));
	//return normalize(cross(dFdy(C), dFdx(C)));
	//return normalize(cross(dFdx(C),dFdy(C)));
}

vec3 reconstructNonUnitCSFaceNormal(vec3 C) {
    //return cross(dFdy(C), dFdx(C));			//-cross(dFdy(C), dFdx(C));
	return cross(dFdx(C), dFdy(C));
}

// returns a unit vector and a screen-space radius for the tap on a unit disk
// (the caller should scale by the actual disk radius)
vec2 tapLocation(int sampleNumber, float spinAngle, out float radiusSS) {
    // radius relative to radiusSS
    float alpha = (float(sampleNumber) + 0.5) * (1.0 / float(NUM_SAMPLES));
    float angle = alpha * (float(NUM_SPIRAL_TURNS) * 6.28) + spinAngle;

    radiusSS = alpha;
    return vec2(cos(angle), sin(angle));
}

/** Read the camera-space position of the point at screen-space pixel uv + unitOffset * radiusSS.  Assumes length(unitOffset) == 1 */
vec3 getOffsetPositionVS(vec2 uv, vec2 unitOffset, float radiusSS) {
	//uv = uv + radiusSS * unitOffset * (1.0 / viewportResolution.x);
	ivec2 ssP = ivec2(radiusSS * unitOffset) + ivec2(uv);
	//vec2 ssP = radiusSS * unitOffset * (1.0/viewportResolution) + uv;
	float zDepth = unpackDepth( texture2D( depthTex, uv ) );
	float depthCS = reconstructCSZ( zDepth );
	//return  reconstructCSPosition( uv , depthCS );
	//return  reconstructCSPosition( uv + vec2(0.5), depthCS );
	return  reconstructCSPosition( vec2(ssP) + vec2(0.5), depthCS );
}

/** Compute the occlusion due to sample with index \a i about the pixel at \a uv that corresponds
    to camera-space point \a C with unit normal \a normalVS, using maximum screen-space sampling radius \a sampleRadiusSS 

    Note that units of H() in the HPG12 paper are meters, not
    unitless.  The whole falloff/sampling function is therefore
    unitless.  In this implementation, we factor out (9 / radius).

    Four versions of the falloff function are implemented below
*/
float sampleAO(vec2 uv, vec3 positionVS, vec3 normalVS, float sampleRadiusSS,
                int tapIndex, float rotationAngle)
{
    const float epsilon = 0.01;
    float radius2 = sampleRadiusWS * sampleRadiusWS;

    // offset on the unit disk, spun for this pixel
    float radiusSS;
    vec2 unitOffset = tapLocation(tapIndex, rotationAngle, radiusSS);
    radiusSS *= sampleRadiusSS;

	// The occluding point in camera space
    //vec3 Q = getOffsetPositionVS(uv, unitOffset, radiusSS);
	vec2 aoTexS = uv + radiusSS*unitOffset;
	vec3 Q = GetViewPos(aoTexS);

    vec3 v = Q - positionVS;

    float vv = dot(v, v);
    //float vn = dot(v, normalVS) - bias;
	float vn = dot(v, normalVS);

	if (VARIATION == 0){

		// (from the HPG12 paper)
		// Note large epsilon to avoid overdarkening within cracks
		//return float(vv < radius2) * max(vn / (epsilon + vv), 0.0);
		return float(vv < radius2) * max((vn - bias) / (epsilon + vv), 0.0) * radius2 * 0.6;
	}

	if (VARIATION == 1){ // default / recommended

		// Smoother transition to zero (lowers contrast, smoothing out corners). [Recommended]
		//float f = max(radius2 - vv, 0.0) / radius2;
		//return f * f * f * max(vn / (epsilon + vv), 0.0);
		float f = max(radius2 - vv, 0.0); 
		return f * f * f * max((vn - bias) / (epsilon + vv), 0.0);
	}

    if (VARIATION == 2){

		// Medium contrast (which looks better at high radii), no division.  Note that the
		// contribution still falls off with radius^2, but we've adjusted the rate in a way that is
		// more computationally efficient and happens to be aesthetically pleasing.
		float invRadius2 = 1.0 / radius2;
		//return 4.0 * max(1.0 - vv * invRadius2, 0.0) * max(vn, 0.0);
		return 4.0 * max(1.0 - vv * invRadius2, 0.0) * max(vn - bias, 0.0);
	}

	if (VARIATION == 3){

		// Low contrast, no division operation
		//return 2.0 * float(vv < radius2) * max(vn, 0.0);
		return 2.0 * float(vv < radius2) * max(vn - bias, 0.0);
	}
}

void main() {
	
	ivec2 ssC = ivec2(gl_FragCoord.xy);			// Pixel being shaded
	float occlusion = 0.0;
	float Ver = 0.0;
	//vec2 aoTexCoord = gl_FragCoord.xy/vec2(tc_window.x,tc_window.y);
	vec2 aoTexCoord = vUV;
	if(texture( depthTex, aoTexCoord ).x < 1.0)
	{
		float zDepth = unpackDepth( texture2D( depthTex, vUV ) );

		float depthCS = reconstructCSZ( zDepth );

		//vec3 originVS = reconstructCSPosition( vUV, depthCS);						// World space point being shaded
		vec3 originVS = reconstructCSPosition( vUV + vec2(0.5), depthCS);
		
		//vec3 normalVS = normalize( reconstructNonUnitCSFaceNormal(originVS) );

		vec3 aoC = GetViewPos(aoTexCoord);											// World space point being shaded
		vec3 aoN_C = normalize(cross(dFdx(aoC),dFdy(aoC)));

		//float randomPatternRotationAngle = fract( fract( vUV.x * viewportResolution.x * 894.0 ) * 3.0 + fract( vUV.y * viewportResolution.y * 999.0 ) * 5.0 + fract( zDepth * 131.0 ) + fract( aoC.x * 3234.0 ) + fract( aoC.y * 99.0 ) );
		float randomPatternRotationAngle = (3 * ssC.x ^ ssC.y + ssC.x * ssC.y) * 10;		// Hash function used in the HPG12 AlchemyAO paper
		//float randomPatternRotationAngle = 2.0 * PI * sampleNoise.x;

		//vec3 n_C = reconstructCSFaceNormal(originVS);			// Reconstruct normals from positions.
		
		// radius of influence in screen space
		float radiusWS = sampleRadiusWS;
		// radius of influence in world space
		//float radiusSS = projScale * radiusWS / originVS.z;
		float radiusSS = -projScale * radiusWS / originVS.z;		// Choose the screen-space sample radius // proportional to the projected area of the sphere
	
		//normalVS = n_C;

		//float uvDiskRadius = 0.5*focalLength.y*radiusWS/aoC.z * projScale;
		originVS = aoC;
		vec3 normalVS = aoN_C;
		//radiusSS = uvDiskRadius;

		//float occlusion = 0.0;
		for (int i = 0; i < NUM_SAMPLES; ++i) {
			occlusion += sampleAO(aoTexCoord, originVS, normalVS, radiusSS, i,
								randomPatternRotationAngle);
		}
		
		//occlusion = 1.0 - occlusion /	(4.0 * float( NUM_SAMPLES ));						
		//occlusion = clamp(pow(occlusion, 1.0 + intensity), 0.0, 1.0);
		occlusion = max(0.0, 1.0 - occlusion * intensityDivR6 * (5.0 / NUM_SAMPLES));
		occlusion = 1.0 - occlusion;
/*		vec2 p;
		float key = clamp(zDepth * (1.0 / -300.0), 0.0, 1.0);
		// Round to the nearest 1/256.0
		float temp = floor(key * 256.0);
		// Integer part
		p.x = temp * (1.0 / 256.0);
		// Fractional part
		p.y = key * 256.0 - temp;

		ambient_occlusion.gb = p;
*/		
		// Bilateral box-filter over a quad for free, respecting depth edges
		// (the difference that this makes is subtle)
		if (abs(dFdx(depthCS)) < 0.02) {
			occlusion -= dFdx(occlusion) * ((ssC.x & 1) - 0.5);
		}
		if (abs(dFdy(depthCS)) < 0.02) {
			occlusion -= dFdy(occlusion) * ((ssC.y & 1) - 0.5);
		}	

		//ambient_occlusion.r = occlusion;
		//ambient_occlusion.w = 1.0;
		Ver = 1.0;
	}
		//    vec4 color = texture2D( tDiffuse, vUV );
		//--- gl_FragColor = vec4( occlusion, occlusion, occlusion, 1.0 );
		//ambient_occlusion = vec4( occlusion, occlusion, occlusion, Ver );
		ambient_occlusion = vec4( 0.0, 0.0, 0.0, occlusion );		
		//    gl_FragColor = vec4(radiusSS, radiusSS, radiusSS, 1.0);
}