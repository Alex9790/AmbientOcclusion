#ifndef __CAMARA_H__
#define __CAMARA_H__

#include <stdio.h>
#include <math.h>
#include "../tools/GL/freeglut.h"

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

class Camara
{
public:
	Camara() { Init(); }
	~Camara(){}

	void Init();
	void Refresh();
	void SetPos(float x, float y, float z);
    void GetPos(float &x, float &y, float &z);
	void SetDirectionVector(float x, float y, float z);
    void GetDirectionVector(float &x, float &y, float &z);
	void SetYaw(float angle);
	void GetYaw(float &angle);
	void SetPitch(float angle);
	void GetPitch(float &angle);
	void SetCamera(float PosX, float PosY, float PosZ, float DirX, float DirY, float DirZ, float YawAngle, float PitchAngle);

	// Navigation
	void Move(float incr);
	void Strafe(float incr);
	void Fly(float incr);
	void RotateYaw(float angle);
	void RotatePitch(float angle);

//private:
	float m_x, m_y, m_z;   // Position
    float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
private:
	float m_yaw, m_pitch; // Various rotation angles
	float m_strafe_lx, m_strafe_lz; // Always 90 degree to direction vector
};

#endif
