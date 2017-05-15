#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "cgameobject.h"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"

#define DEG2RAD(x) ((float)(glm::pi<float>() / 180.0f * x))
#define RAD2DEG(x) (180.0 / glm::pi<float>() * x)

class CGameObject;

class CCamera 
{
private:
	glm::vec3 initPosition, finalPosition;
	glm::vec3 initLookAt, finalLookAt;

	glm::vec3 lookAtVel;
	glm::vec3 lookAtAccel;

	void updateLookAt();
	void updateMoveTo();

public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 lookAt;
	glm::vec3 vectorVelocidad;

	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	float yaw;
	float pitch;

	int screenwidth, screenheight;
	int centerX, centerY;

	CCamera(int h, int w, float aspect);
	CCamera(glm::vec3 *look);
	CCamera(glm::vec3 *pos, glm::vec3 *look);
	~CCamera();

	void LookAtNow(CGameObject *object);
	void MoveToNow(float x, float y, float z);

	void UpdateLookAt();
	void UpdateMoveTo();

	// right rotation along y-axis (yaw)
	void RotateYaw(float radians);
	void RotatePitch(float radians);
	void RotateRoll(float radians);

	// do physics calculations
	void Animate(double deltaTime);

	// Model, view, projection matrix
	glm::mat4 m_M4model;
	glm::mat4 m_M4view;
	glm::mat4 m_M4projection;
	glm::mat4 m_M4modelView;
	glm::mat3 m_M3normal;
	glm::mat4 m_M4MVP;
	glm::mat4 m_M4ViewProjectionInverse;
};

#endif
