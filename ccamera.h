#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "cobject.h"

#define DEG2RAD(x) (glm::pi<float>() / 180.0 * x)
#define RAD2DEG(x) (180.0 / glm::pi<float>() * x)

class CObject;

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

	CCamera();
	CCamera(glm::vec3 *look);
	CCamera(glm::vec3 *pos, glm::vec3 *look);
	~CCamera();

	void LookAtNow(CObject *object);
	/*void LookAt(CObject *object);
	void MoveTo(CObject *object);
	void MoveToNow(CObject *object);*/
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
};

#endif