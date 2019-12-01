//Librerias y sus .h
#define _USE_MATH_DEFINES
#pragma comment (lib,"lib/freeglut.lib")
#pragma comment (lib,"lib/AntTweakBar.lib")
#pragma comment (lib,"lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/glew.c"
#include "../tools/GL/freeglut.h"
#include "../tools/AntTweakBar/AntTweakBar.h"
#include "../tools/glm/glm.hpp"
#include "../tools/glm/gtc/matrix_transform.hpp"
#include "../tools/glm/gtc/type_ptr.hpp" //for glm::value_ptr
#include "../tools/glm/gtx/transform2.hpp" // for glm::translate, glm::rotate, glm::scale, glm::perspective
#include "../tools/glm/gtc/matrix_inverse.hpp"
#include "../tools/glm/ext.hpp"
#include <vector>  //Include Vector
#include <iostream>
#include <math.h>

#include "TW.h"
#include "Quaternion.h"
#include "Shaders.h"
#include "Objeto.h"
#include "Camara.h"
//#include "SSAO.h"

#include "frameGrab.h"


#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

using namespace std;

float angle;

GLuint shadowFBO;
GLuint shadowFBO_1;
glm::mat4 lightPV;
glm::mat4 lightPV_1;
glm::mat4 shadowBias;
GLuint depthTex;
GLuint depthTex_1;
glm::vec3 PosLight = glm::vec3(0.0f, 10.0f, 40.0f);
glm::vec3 DirLight = glm::vec3(0.0f, -1.0f, 17.1f);
bool Light_Encendido;
float Light_Intensidad;
glm::vec3 PosLight_1 = glm::vec3(5.0f, 10.0f, 40.0f);
glm::vec3 DirLight_1 = glm::vec3(0.0f, -1.0f, 17.1f);
bool Light_1_Encendido;
float Light_1_Intensidad;

const int CUBEMAP_SIZE = 1024;

//dynamic cubemap texture ID
GLuint dynamicCubeMapID;
glm::mat4  Pcubemap;

Shaders *ShaderA;

glm::vec3 traslacion; //Variable para la traslacion
glm::vec3 escalamiento; //Variable para el escalamiento

// Ventana de freeGLUT
int WWidth = 1920;	//1280
int WHeight = 1080;	//720

float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };

glm::mat4 RotMatrix;	// NEW - we keep all of the model's rotations in this matrix (for rotating normals)
glm::mat4 transMatrix;	// Matrix for changing the position of the object
glm::mat4 scaleMatrix;	// Duh..
glm::mat3 normalMatrix;
glm::mat3 tempMatrix1;	// A temporary matrix for holding intermediate multiplications
glm::mat4 M;				// The final model matrix M to change into world coordinates
glm::mat4 V;				// The camera matrix (for position/rotation) to change into camera coordinates
glm::mat4 P;				// The perspective matrix for the camera (to give the scene depth); initialize this ONLY ONCE!
glm::mat4 MV;
glm::mat4 MVP;

//Objetos
Objeto Ground;
Objeto Cube;
Objeto Wolf;
Objeto Luz;
Objeto Grass;
Objeto PortalGun;
Objeto Blacksmith;
Objeto Alduin;
Objeto AlduinEyes;
Objeto CrytekSponza;
Objeto VanquishInnersuit1;
Objeto VanquishInnersuit2;
Objeto VanquishOutersuitlower;
Objeto VanquishOutersuitupper;
Objeto VanquishOutersuitupper2;
Objeto VanquishVisor;
Objeto Porsche1;
Objeto Porsche2;
Objeto Town1;
Objeto Town2;
Objeto Town3;

//Camara
Camara Cam;

bool g_key[256];
//bool g_shift_down = false;
bool fps_mode = false;
int viewport_width = 0;
int viewport_height = 0;
bool mouse_left_down = false;
bool mouse_right_down = false;

// configuracion de movimiento
const float translation_speed = 0.1;
const float rotation_speed = M_PI/180*0.3;

///////////// INICIO AMBIENT OCCLUSION	//////////////////
//SSAO SS_AO;
//se reduce tamaño del vewport por rendimiento - 3840x2160 - 1920x1080 - 1280x720 
//int RTT_WIDTH = WWidth;
//int RTT_HEIGHT = WHeight;
int RTT_WIDTH = 1920;
int RTT_HEIGHT = 1080;

//noise texture ID
GLuint noiseTexID;

//quad vertex array and vertex buffer object IDs
GLuint quadVAOID;
GLuint quadVBOID;
GLuint quadIndicesID;

//FBO ids for normal and filtering FBO
GLuint fboID, filterFBOID;
//colour and depth attachment texture IDs
GLuint normalTextureID, depthTextureID;
//filtered texture colour attachment IDs
GLuint blurTexID[2];

//shaders
Shaders *ShaderFinal;
Shaders *ShaderGaussH;
Shaders *ShaderGaussV;
Shaders *ShaderPrimerPasoSSAO;
Shaders *ShaderSegundoPasoSSAO;

//Activar SSAO
bool ActivarSSAO = true;
//Activar BLUR
bool ActivarBLUR = true;
//Ver Solo AO
bool VerSoloAO = false;

//radio de muestreo para SSAO
float RadioMuestreo = 0.25f;
float Scale = 1;
float Bias = -0.2;
float Intensity = 1.75;
///////////// FIN AMBIENT OCCLUSION	//////////////////

///////////// INICIO ALCHEMY AO	//////////////////
Shaders *AlchemyAO;

GLuint quadViewRaysBOID;

glm::vec3 directions[4];

//Botones
int base_samples = 0;
int min_samples = 64;
float radius = 10.0;
float projection_factor = 0.01;	//0.75
float bias = -1.5;				//1.0
float sigma = 3.0;				//2.0

float NUM_SPIRAL_TURNS = 2.0;		//2.0
float NUM_SAMPLES = 16;				//64
float aoRad = 1.0;					//0.5
float aoBias = 0.01;				//0.002


//Activar Alchemy AO
bool ActivarAlchemyAO = false;
//Activar Ver Solo Alchemy AO
bool ActivarVerAlchemyAO = true;
///////////// FIN ALCHEMY AO	//////////////////

///////////// INICIO SCALABLE AMBIENT OCCLUSION (SAO)	//////////////////
Shaders *SAO;

int SAONUM_SAMPLES = 8;		
int SAONUM_SPIRAL_TURNS = 2;		
int SAOVARIATION = 1;			
float SAOintensity = 100.0;
float SAOradius = 10.0;	//5.0
float SAObias = 0.20;		//0.0
float SAOzNear = 1.0;
float SAOzFar = 1000.0;
float SAOprojScale = 2.0;		//100.0

//Activar SAO
bool ActivarSAO = false;
//Activar Ver Solo Alchemy AO
bool ActivarVerSAO = true;

///////////// FIN SCALABLE AMBIENT OCCLUSION (SAO)	//////////////////

///////////// INICIO Unreal Engine AO	//////////////////
Shaders *UnrealEngineAO;

int NUM_SAMPLE_DIRECTIONS = 4;
int NUM_SAMPLE_STEPS = 1;
int APPLY_ATTENUATION = 0;
int USE_ACTUAL_NORMALS = 0;
float uFOV = 60.0;
float uSampleRadius = 0.50;
float uAngleBias = 0.05;
float uIntensity = 6.0;

//Activar SAO
bool ActivarUnrealEngineAO = false;
//Activar Ver Solo Alchemy AO
bool ActivarVerUnrealEngineAO = true;

///////////// FIN Unreal Engine AO	//////////////////

///////////// INICIO MSSAO	////////////////////////
#define LEVEL_COUNT 5 // number of mip-map levels

bool oddFrame = true; // used to ping-pong buffers in the temporal filtering step
//Activar SSAO
bool ActivarMSSAO = false;
bool MSSAO_ActivarAlchemyAO = true;
//bool MSSAO_ActivarBLUR = true;

// buffers and textures ([0] = finest resolution)
GLuint frameBufs[LEVEL_COUNT];
GLuint filterframeBufs[LEVEL_COUNT];
GLuint posTex[LEVEL_COUNT];
GLuint normTex[LEVEL_COUNT];
GLuint aoTexBlur[LEVEL_COUNT*2];

// shader programs
Shaders *ShaderFinalMSSAO;

float MSSAO_NUM_SPIRAL_TURNS = 2.0;		//2.0
float MSSAO_NUM_SAMPLES = 16;				//64
float MSSAO_aoRad = 2.0;					//0.5
float MSSAO_aoBias = 0.02;				//0.002

float intensidadTex1 = 1.0;
float intensidadTex2 = 0.5;
float intensidadTex3 = 0.3;
float intensidadTex4 = 0.3;
float intensidadTex5 = 0.3;

FrameGrab frameGrab; // used to get screenshots


///////////// FIN MSSAO	//////////////////

///////////// ESCENAS	//////////////////
bool Escena1 = true;
bool Escena2 = false;
bool Escena3 = false;

///////////// MEDIR TIEMPO	//////////////////
GLuint query;
GLuint64 elapsed_time;
GLuint64 SumMSSAOTime;
GLuint64 SumMSSAOBlurTime;
int done = 0;

///////////// DATOS CAMERA	//////////////////
float CamPosX, CamPosY, CamPosZ, CamDirX, CamDirY, CamDirZ, CamYaw, CamPitch;
bool ScreenShot = false;