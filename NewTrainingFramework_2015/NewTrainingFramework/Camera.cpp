#include "stdafx.h"
#include "../Utilities/Math.h"
#include "Camera.h"
#include "Globals.h"

Vector4 Vec3ToVec4(Vector3 vec)
{
	Vector4 rez = (vec.x, vec.y, vec.z, 0);
	return rez;
}

Vector3 Vec4ToVec3(Vector4 vec)
{
	Vector3 rez;
	rez.x = vec.x; rez.y = vec.y; rez.z = vec.z;
	return rez;
}

Camera::Camera()
{
	position = Vector3(0.0f, 0.0f, -1.0f); target = Vector3(0.0f, 0.0f, 0.0f); up = Vector3(0.0f, 1.0f, 0.0f);
	moveSpeed = 8.0f; rotateSpeed = 3.0f;
	nearPlane = 0.2f; farPlane = 100.0f;
	fov = 0.785398f;
	perspectiveMatrix.SetPerspective(fov, (float)Globals::screenWidth / (float)Globals::screenHeight, nearPlane,farPlane);
}

void Camera::updateAxes()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updateWorldView()
{
	updateAxes();

	Matrix T,R;

	T.m[0][0] = 1.0f; T.m[0][1] = 0.0f; T.m[0][2] = 0.0f; T.m[0][3] = 0.0f;
	T.m[1][0] = 0.0f; T.m[1][1] = 1.0f; T.m[1][2] = 0.0f; T.m[1][3] = 0.0f;
	T.m[2][0] = 0.0f; T.m[2][1] = 0.0f; T.m[2][2] = 1.0f; T.m[2][3] = 0.0f;
	T.m[3][0] = position.x; T.m[3][1] = position.y; T.m[3][2] = position.z; T.m[3][3] = 1.0f;

	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0.0f;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0.0f;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f; R.m[3][1] = 0.0f; R.m[3][2] = 0.0f; R.m[3][3] = 1.0f;

	worldMatrix = R * T;

	Matrix T_inv, R_trans;
	R_trans = R.Transpose();

	T_inv.m[0][0] = 1.0f; T_inv.m[0][1] = 0.0f; T_inv.m[0][2] = 0.0f; T_inv.m[0][3] = 0.0f;
	T_inv.m[1][0] = 0.0f; T_inv.m[1][1] = 1.0f; T_inv.m[1][2] = 0.0f; T_inv.m[1][3] = 0.0f;
	T_inv.m[2][0] = 0.0f; T_inv.m[2][1] = 0.0f; T_inv.m[2][2] = 1.0f; T_inv.m[2][3] = 0.0f;
	T_inv.m[3][0] = -position.x; T_inv.m[3][1] = -position.y; T_inv.m[3][2] = -position.z; T_inv.m[3][3] = 1.0f;

	viewMatrix = T_inv * R_trans;

	//MVP = worldMatrix * viewMatrix * perspectiveMatrix;
	//MVP = viewMatrix;

}

void Camera::moveOx(int sens)
{
	Vector3 forward = xAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOy(int sens)
{
	Vector3 forward = yAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::moveOz(int sens)
{
	Vector3 forward = zAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	updateWorldView();
}

void Camera::rotateOx(float sens)
{
	Matrix mRotateOX;
	mRotateOX.SetRotationX(sens * rotateSpeed * deltaTime);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	up = Vec4ToVec3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	target = Vec4ToVec3(rotatedTarget * worldMatrix);

	updateWorldView();
}

void Camera::rotateOy(float sens)
{
	Matrix mRotateOY;
	mRotateOY.SetRotationY(sens * rotateSpeed * deltaTime);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;
	target = Vec4ToVec3(rotatedTarget * worldMatrix);

	updateWorldView();
}

void Camera::rotateOz(float sens)
{
	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(sens * rotateSpeed * deltaTime);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	up = Vec4ToVec3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();

	updateWorldView();
}


