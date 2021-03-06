#include "pch.h"
#include "Camera.h"

namespace Renderer
{
	void Camera::Set(
		XMVECTOR& position,
		XMVECTOR& view,
		XMVECTOR& up,
		float fovY,
		float aspectRatio,
		float nearPlane,
		float farPlane)
	{
		Position = position;
		View = view;
		Up = up;
		Right = XMVector3Cross(-View, Up);
		FoVY = fovY;
		Aspect = aspectRatio;
		NearPlane = nearPlane;
		FarPlane = farPlane;
		OrientMatrix = XMMatrixIdentity();
		UpdateCorners();
	}

	void Camera::Set(
		XMVECTOR& position,
		XMVECTOR& topLeft,
		XMVECTOR& topRight,
		XMVECTOR& bottomLeft,
		XMVECTOR& bottomRight
		)
	{
		TopLeft = topLeft;
		TopRight = topRight;
		BottomLeft = bottomLeft;
		BottomRight = bottomRight;
		Position = position;

		XMVECTOR v = (topLeft + bottomRight) / 2;
		XMVECTOR top = (topLeft + topRight) / 2;
		
		XMVECTOR w = XMVector3Length(topRight - topLeft);
		float width;
		XMVectorGetByIndexPtr(&width, w, 0);

		XMVECTOR h = XMVector3Length(topRight - bottomRight);
		float height;
		XMVectorGetByIndexPtr(&height, h, 0);

		XMVECTOR u = top - v;

		XMVECTOR a = XMVector3AngleBetweenVectors(TopRight, BottomRight);
		float angle;
		XMVectorGetByIndexPtr(&angle, a, 0);

		Up = XMVector3Normalize(u);
		View = XMVector3Normalize(v);
		Right = XMVector3Cross(-View, Up); // always left handed coord system
		Aspect = width / height;
		FoVY = angle;
	}

	void Camera::Set(const Renderer::Camera& c)
	{
		Position = c.Position;
		View = c.View;
		Up = c.Up;
		Right = XMVector3Cross(-View, Up);
		FoVY = c.FoVY;
		Aspect = c.Aspect;
		NearPlane = c.NearPlane;
		FarPlane = c.FarPlane;
		OrientMatrix = c.OrientMatrix;
		UpdateCorners();
	}

	void Camera::UpdateCorners()
	{
		float height = 2 * tan(FoVY / 2) * NearPlane;
		float width = height * Aspect;
		XMVECTOR r = Right * width / 2.0;
		XMVECTOR u = Up * height / 2.0;
		XMVECTOR v = View * NearPlane;
		TopLeft = XMVector3Normalize(v + u - r);
		TopRight = XMVector3Normalize(v + u + r);
		BottomLeft = XMVector3Normalize(v - u - r);
		BottomRight = XMVector3Normalize(v - u + r);
	}

	void Camera::Strafe(float Dist)
	{
		Position = Position + Right * Dist;
	}

	void Camera::Walk(float Dist)
	{
		Position = Position + View * Dist;
	}

	void Camera::Pitch(float Angle)
	{
		RotateAroundRight(Angle);
	}

	void Camera::Roll(float Angle)
	{
		RotateAroundView(Angle);
	}

	void Camera::Yaw(float Angle)
	{
		RotateAroundUp(Angle);
	}

	void Camera::UpdateMatrices()
	{
		ViewMatrix = XMMatrixLookToLH(Position, View, Up);
		ProjMatrix = XMMatrixPerspectiveFovLH(
			FoVY,
			Aspect,
			NearPlane,
			FarPlane
			);
		UpdateCorners();
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

	void Camera::MakeFrustumBuffers(std::shared_ptr<DeviceResources>& deviceResources)
	{
		std::vector<Camera::FrustumVertex> OurVertices;
		std::vector<UINT> OurIndices;
		int index = 0;
		std::vector<XMVECTOR> corners;
		corners.push_back(TopLeft);
		corners.push_back(TopRight);
		corners.push_back(BottomRight);
		corners.push_back(BottomLeft);
		float px, py, pz;
		Base::Math::VectorGet(Position, &px, 0);
		Base::Math::VectorGet(Position, &py, 1);
		Base::Math::VectorGet(Position, &pz, 2);

		float x, y, z;
		Base::Math::VectorGet(BottomLeft, &x, 0);
		Base::Math::VectorGet(BottomLeft, &y, 1);
		Base::Math::VectorGet(BottomLeft, &z, 2);
		float fscale = 10.0;
		Camera::FrustumVertex blv = { 
			x * fscale + px, y * fscale + py, z * fscale + pz, 1,
			1, 0, 0, 1
		};
		OurVertices.push_back(blv);
		OurIndices.push_back(index++);
		for (int i = 0; i < corners.size(); i++)
		{
			XMVECTOR corner = corners[i];
			Base::Math::VectorGet(corner, &x, 0);
			Base::Math::VectorGet(corner, &y, 1);
			Base::Math::VectorGet(corner, &z, 2);
			Camera::FrustumVertex v = { 
				x*fscale+px, y*fscale+py, z*fscale+pz, 1,
				1, 0, 0, 1
			};
			OurVertices.push_back(v);
			OurIndices.push_back(index++);
			OurVertices.push_back(v);
			OurIndices.push_back(index++);

			Camera::FrustumVertex v2 = { 
				px, py, pz, 1,
				1, 0, 0, 1
			};
			OurVertices.push_back(v2);
			OurIndices.push_back(index++);
			OurVertices.push_back(v);
			OurIndices.push_back(index++);
		}
		OurIndices.pop_back();
		OurVertices.pop_back();
		numIndices = OurIndices.size();

		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(Camera::FrustumVertex) * OurVertices.size();
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&vertexBD, &vertexSRD, &frustumvertexbuffer)
			);

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(UINT) * (int)OurIndices.size();    // indices are stored in short values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&indexBD, &indexSRD, &frustumindexbuffer)
			);
	}
}
