#include "pch.h"
#include "Camera.h"

using namespace Renderer;

void Camera::Strafe(float Dist)
{
	Position = Position + Right * Dist;
}

void Camera::Walk(float Dist)
{
	Position = Position + View * Dist;
}

void Camera::UpdateMatrices()
{
	ViewMatrix = XMMatrixLookToLH(Position, View, Up);
	ProjMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(FoVY),
		Aspect,
		NearPlane,
		FarPlane
		);
}

void Camera::RotateAroundPoint(XMVECTOR& Point, XMVECTOR& Axis, float Angle)
{
	XMVECTOR offset = Position - Point;
	XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
	offset = XMVector3Rotate(offset, quatRot);
	Position = Point + offset;
}

void Camera::RotateByAxisAngle(XMVECTOR& Axis, float Angle)
{
	XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
	View = XMVector3Normalize(XMVector3Rotate(View, quatRot));
	Right = XMVector3Normalize(XMVector3Rotate(Right, quatRot));
	Up = XMVector3Normalize(XMVector3Rotate(Up, quatRot));
}

void Camera::RotateAroundUp(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(Up, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}

void Camera::RotateAroundRight(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(Right, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}

void Camera::RotateAroundView(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(View, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}