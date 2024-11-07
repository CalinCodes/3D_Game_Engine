#include "stdafx.h"
#include "../Utilities/Math.h"
#include "Camera.h"
#include "Globals.h"

///extern Matrix M, V, P;
extern Vector3 position = Vector3(0.0f, 0.0f, -1.0f), target = Vector3(0.0f, 0.0f, 0.0f), up = Vector3(0.0f, 1.0f, 0.0f);
extern GLfloat moveSpeed = 0.1f, rotateSpeed = 0.1f, nearPlane = 0.2f, farPlane = 10.0f, fov = 0.785398f, deltaTime;
extern Vector3 xAxis, yAxis, zAxis;
extern Matrix viewMatrix, worldMatrix, perspectiveMatrix;

Vector4 Vec3ToVec4(Vector3 vec)
{
	Vector4 rez = (vec, 0);
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
	moveSpeed = 0.1f; rotateSpeed = 0.1f;
	nearPlane = 0.2f; farPlane = 10.0f;
	fov = 0.785398f;
	perspectiveMatrix.SetPerspective(fov,nearPlane,farPlane,Globals::screenWidth/Globals::screenHeight);
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

	T.m[0][0] = 1.0f; T.m[0][0] = 0.0f; T.m[0][0] = 0.0f; T.m[0][0] = 0.0f;
	T.m[0][0] = 0.0f; T.m[0][0] = 0.0f; T.m[0][0] = 0.0f; T.m[0][0] = 0.0f;
	T.m[0][0] = 0.0f; T.m[0][0] = 0.0f; T.m[0][0] = 1.0f; T.m[0][0] = 0.0f;
	T.m[0][0] = position.x; T.m[0][0] = position.y; T.m[0][0] = position.z; T.m[0][0] = 1.0f;

	R.m[0][0] = xAxis.x; R.m[0][0] = xAxis.y; R.m[0][0] = xAxis.z; R.m[0][0] = 0.0f;
	R.m[0][0] = yAxis.x; R.m[0][0] = yAxis.y; R.m[0][0] = yAxis.z; R.m[0][0] = 0.0f;
	R.m[0][0] = zAxis.x; R.m[0][0] = zAxis.y; R.m[0][0] = zAxis.z; R.m[0][0] = 0.0f;
	R.m[0][0] = 1.0f; R.m[0][0] = 0.0f; R.m[0][0] = 0.0f; R.m[0][0] = 1.0f;

	worldMatrix = R * T;

	Matrix T_inv, R_trans;
	R_trans = R.Transpose();

	T_inv.m[0][0] = 1.0f; T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f;
	T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f;
	T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 0.0f; T_inv.m[0][0] = 1.0f; T_inv.m[0][0] = 0.0f;
	T_inv.m[0][0] = -position.x; T_inv.m[0][0] = -position.y; T_inv.m[0][0] = -position.z; T_inv.m[0][0] = 1.0f;

	viewMatrix = T_inv * R_trans;

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

void Camera::rotateOx(float angle)
{
	Matrix mRotateOX;
	mRotateOX.SetRotationX(angle);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	up = Vec4ToVec3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(Vec3ToVec4(target) - Vec3ToVec4(position)).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	target = Vec4ToVec3(rotatedTarget * worldMatrix);

	updateWorldView();
}

void Camera::rotateOy(float angle)
{
	Matrix mRotateOY;
	mRotateOY.SetRotationY(angle);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(Vec3ToVec4(target) - Vec3ToVec4(position)).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;
	target = Vec4ToVec3(rotatedTarget * worldMatrix);

	updateWorldView();
}

void Camera::rotateOz(float angle)
{
	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(angle);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	up = Vec4ToVec3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();

	updateWorldView();
}

