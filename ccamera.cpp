#include "stdafx.h"

#include "ccamera.h"

#include <glm\gtc\matrix_inverse.hpp>

CCamera::CCamera()
{
	position = glm::vec3(0.0, 0.0, 0.0);
	lookAt = glm::vec3(0.0, 0.0, 1.0);

	forward = lookAt;
	up = glm::vec3(0.0, 1.0, 0.0);
	right = glm::vec3(1.0, 0.0, 0.0);

	velocity = glm::vec3(0.0, 0.0, 0.0);
	acceleration = glm::vec3(0.0, 0.0, 0.0);
	vectorVelocidad = glm::vec3(0.0, 0.0, 0.0);

	yaw = 0.0;
	pitch = 0.0;

	m_M4model = glm::mat4(1.0);
	m_M4view = glm::mat4(1.0);
	m_M4projection = glm::mat4(1.0);
	m_M3normal = glm::mat3(1.0);
	m_M4MVP = glm::mat4(1.0);
}

CCamera::CCamera(glm::vec3 *look)
{
	position = glm::vec3(0.0, 0.0, 0.0);
	lookAt = glm::normalize(*look);

	forward = lookAt;
	up = glm::vec3(0.0, 1.0, 0.0);
	
	//Producto vectorial
	right = glm::cross(forward, up);

	velocity = glm::vec3(0.0, 0.0, 0.0);
	acceleration = glm::vec3(0.0, 0.0, 0.0);
	vectorVelocidad = glm::vec3(0.0, 0.0, 0.0);

	yaw = 0.0;
	pitch = 0.0;
}

CCamera::CCamera(glm::vec3 *pos, glm::vec3 *look)
{
	position = *pos;
	lookAt = glm::normalize(*look);

	forward = lookAt;
	up = glm::vec3(0.0, 1.0, 0.0);
	right = glm::vec3(1.0, 0.0, 0.0);

	velocity = glm::vec3(0.0, 0.0, 0.0);
	acceleration = glm::vec3(0.0, 0.0, 0.0);
	vectorVelocidad = glm::vec3(0.0, 0.0, 0.0);

	yaw = 0.0;
	pitch = 0.0;
}

CCamera::~CCamera()
{
}

void CCamera::UpdateLookAt()
{
	glm::vec3 look = glm::vec3(finalLookAt.x - lookAt.x,
		finalLookAt.y - lookAt.y,
		finalLookAt.z - lookAt.z);

	lookAtVel = look * 0.5f;
}

void CCamera::UpdateMoveTo()
{
	glm::vec3 pos = glm::vec3(finalPosition.x - position.x,
		finalPosition.y - position.y,
		finalPosition.z - position.z);

	velocity = pos * 0.5f;
}

void CCamera::RotatePitch(float radians)
{
	float sine   = sinf(radians);
	float cosine = cosf(radians);

	up.y = cosine * glm::length(up);
	up.z = sine   * glm::length(up);

	forward.y = -sine  * glm::length(forward);
	forward.z = cosine * glm::length(forward);
	/*     x   y      z       p
	|  1  0       0       0 |
	M = |  0  cos(A) -sin(A)  0 |
	|  0  sin(A)  cos(A)  0 |
	|  0  0       0       1 |
	*/
}

void CCamera::RotateYaw(float radians)
{
	float sine   = sinf(radians);
	float cosine = cosf(radians);

	right.x = cosine * glm::length(right);
	right.z = sine   * glm::length(right);

	forward.x = -sine  * glm::length(forward);
	forward.z = cosine * glm::length(forward);

	/*	   x      y    z      p
	|  cos(A)  0  -sin(A)  0 |
	M = |  0       1   0       0 |
	|  sin(A)  0   cos(A)  0 |
	|  0       0   0       1 |
	*/

}

void CCamera::RotateRoll(float radians)
{
	float sine   = sinf(radians);
	float cosine = cosf(radians);

	right.x = cosine * glm::length(right);
	right.y = sine   * glm::length(right);

	up.x = -sine  * glm::length(forward);
	up.y = cosine * glm::length(forward);
	/*
	|  cos(A)  -sin(A)   0   0 |
	M = |  sin(A)   cos(A)   0   0 |
	|  0        0        1   0 |
	|  0        0        0   1 |
	*/
}

void CCamera::MoveToNow(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void CCamera::LookAtNow(CObject *object)
{
	lookAt = object->m_position;
}

/*void CCamera::MoveToNow(CObject *object)
{
	position = object->m_CVPosition;
}

// look at an object over time
void CCamera::LookAt(CObject *object)
{
	glm::vec3 v = glm::vec3(object->m_CVPosition - lookAt);

	initLookAt = lookAt;
	finalLookAt = object->m_CVPosition;

	lookAtAccel = lookAt * -0.25f;

	UpdateLookAt();
}

// move to an object over time
void CCamera::MoveTo(CObject *object)
{
	glm::vec3 v = glm::vec3(object->m_CVPosition - position);

	initPosition = position;
	finalPosition = object->m_CVPosition;

	acceleration = position * -0.25f;

	UpdateMoveTo();
}*/


void CCamera::Animate(double deltaTime)
{
	if ((yaw >= 360.0f) || (yaw <= -360.0f))
		yaw = 0.0f;

	if (pitch > 90.0f)
		pitch = 90.0f;

	if (pitch < -90.0f)
		pitch = -90.0f;

	// calculate trig functions
	double cosYaw = cos(DEG2RAD(yaw));
	double sinYaw = sin(DEG2RAD(yaw));
	double sinPitch = sin(DEG2RAD(pitch));

	// speed is velocity z-component
	float speed = velocity.z * (float)deltaTime;

	// strafe speed is velocity x-component
	float strafeSpeed = velocity.x * (float)deltaTime;

	// speed limit
	if (speed > 15.0)
		speed = 15.0;
	if (strafeSpeed > 15.0)
		strafeSpeed = 15.0;
	if (speed < -15.0)
		speed = -15.0;
	if (strafeSpeed < -15.0)
		strafeSpeed = -15.0;

	// friction
	if (glm::length(velocity) > 0.0)
		acceleration = velocity * -1.5f;

	velocity = velocity + acceleration * (float)deltaTime;

	// calculate new position of camera
	position.x += float(cos(DEG2RAD(yaw + 90.0)))*strafeSpeed;
	position.z += float(sin(DEG2RAD(yaw + 90.0)))*strafeSpeed;
	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	// calculate lookAt based on new position
	lookAt.x = float(position.x + cosYaw);
	lookAt.y = float(position.y + sinPitch);
	lookAt.z = float(position.z + sinYaw);

	// Establecemos un vector que nos indique la dirección de movimiento
	//   de la cámara, ya que el vector velocidad es local.
	vectorVelocidad = glm::normalize(lookAt - position) * glm::length(velocity);
	vectorVelocidad.y = 0.0;

	/*position.x = 7000.0;
	position.y = 2000.0;
	position.z = 7000.0;

	lookAt.x = 0.0;
	lookAt.y = 0.0;
	lookAt.z = 0.0;*/

	m_M4model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	m_M4view = glm::lookAt(position, lookAt, up);
	m_M4projection = glm::perspective(50.0f, 1920.0f / 1200.0f, 1.0f, 8000.0f);

	//Calcula matriz modelview
	m_M4modelView = m_M4view * m_M4model;

	//Matrix normal
	//m_M3normal = glm::transpose(glm::inverse(glm::mat3(m_M4modelView)));
	m_M3normal = glm::inverseTranspose(glm::mat3(m_M4modelView));

	//Importante el orden de las transformaciones: MVP = proj * view * model
	m_M4MVP = m_M4projection * m_M4modelView;
}