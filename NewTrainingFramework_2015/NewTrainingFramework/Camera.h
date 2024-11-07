#pragma once

class Camera
{
public:

	///Matrix M, V, P;
	Vector3 position, target, up;
	GLfloat moveSpeed, rotateSpeed, nearPlane, farPlane, fov, deltaTime;
	Vector3 xAxis, yAxis, zAxis;
	Matrix viewMatrix, worldMatrix, perspectiveMatrix;

	Camera();

	void updateAxes();
	void updateWorldView();

	void moveOx(int sens);
	void moveOy(int sens);
	void moveOz(int sens);

	void rotateOx(float angle);
	void rotateOy(float angle);
	void rotateOz(float angle);

};