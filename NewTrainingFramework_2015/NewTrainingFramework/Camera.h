#pragma once

class Camera
{
public:

	Vector3 position, target, up;
	GLfloat moveSpeed, rotateSpeed, nearPlane, farPlane, fov, deltaTime;
	Vector3 xAxis, yAxis, zAxis;
	Matrix viewMatrix, worldMatrix, perspectiveMatrix, MVP;

	Camera();

	void updateAxes();
	void updateWorldView();

	void moveOx(int sens);
	void moveOy(int sens);
	void moveOz(int sens);

	void rotateOx(float sens);
	void rotateOy(float sens);
	void rotateOz(float sens);

};