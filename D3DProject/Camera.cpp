#include "Camera.h"

//Builds Camera at the Origin
Camera::Camera() :
	mPos(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f), 
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f),
	mAspect(1.77f)
{
	SetFrustum(XM_PIDIV2, mAspect, 0.5f, 1000.f);
}

Camera::~Camera(){}

XMFLOAT3 Camera::GetRight()
{
	return mRight;
}

XMVECTOR Camera::GetRightXM()
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Camera::GetUp()
{
	return mUp;
}

XMVECTOR Camera::GetUpXM()
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Camera::GetLook()
{
	return mLook;
}

XMVECTOR Camera::GetLookXM()
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Camera::GetPosition()
{
	return mPos;
}

XMVECTOR Camera::GetPositionXM()
{
	return XMLoadFloat3(&mPos);
}

XMMATRIX Camera::GetView() const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX Camera::GetProj() const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX Camera::GetViewProj() const
{
	return (XMMatrixMultiply(GetView(), GetProj()));
}

void Camera::SetRight(float x, float y, float z)
{
	mRight = XMFLOAT3(x, y, z);
}

void Camera::SetUp(float x, float y, float z)
{
	mUp = XMFLOAT3(x, y, z);
}

void Camera::SetLook(float x, float y, float z)
{
	mLook = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(XMFLOAT3& position)
{
	mPos = position;
}

void Camera::SetFrustum(float foVY, float aspectRatio, float nearZ, float farZ)
{
	mFoVY = foVY;
	mAspect = aspectRatio;
	mNearZ = nearZ;
	mFarZ = farZ;
	
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(mFoVY, mAspect, mNearZ, mFarZ));
}

void Camera::PrepView(XMFLOAT3& position, XMFLOAT3& target, XMFLOAT3& up)
{
	XMVECTOR vPosition = XMLoadFloat3(&position);
	XMVECTOR vTarget = XMLoadFloat3(&target);
	XMVECTOR vUp = XMLoadFloat3(&up);

	//Use Position, Target, and Up to calculate Look, Right, Up
	SetView(vPosition, vTarget, vUp);
}

void Camera::SetView(XMVECTOR _vPosition, XMVECTOR _vTarget, XMVECTOR _vUp)
{
	XMVECTOR vLook = XMVector3Normalize(XMVectorSubtract(_vTarget, _vPosition));
	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(_vUp, vLook));
	XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	XMStoreFloat3(&mRight, vRight);
	XMStoreFloat3(&mUp, vUp);
	XMStoreFloat3(&mLook, vLook);
	XMStoreFloat3(&mPos, _vPosition);
}

void Camera::Strafe(float moveSpeed)
{
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	vMoveDirection = XMVectorScale(vMoveDirection, mSpeedScalar);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);
	XMVECTOR vRight = XMLoadFloat3(&mRight);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vRight, vPosition));
}

void Camera::Step(float moveSpeed)
{
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	vMoveDirection = XMVectorScale(vMoveDirection, mSpeedScalar);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);
	XMVECTOR vLook = XMLoadFloat3(&mLook);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vLook, vPosition));
}

void Camera::VFly(float moveSpeed)
{
	
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	vMoveDirection = XMVectorScale(vMoveDirection, mSpeedScalar);
	XMVECTOR vGlobalY = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vGlobalY, vPosition));
}

void Camera::Pitch(float angle)
{
	XMMATRIX camRightRotationMX = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	
	//Grab camera Y and Z axes
	XMVECTOR vUp = XMLoadFloat3(&mUp);
	XMVECTOR vLook = XMLoadFloat3(&mLook);

	//Don't use RightRotationMX's position vector when multiplying
	XMVECTOR vUpdatedUp = XMVector3TransformNormal(vUp, camRightRotationMX);
	XMVECTOR vUpdatedLook = XMVector3TransformNormal(vLook, camRightRotationMX);

	XMStoreFloat3(&mUp, vUpdatedUp);
	XMStoreFloat3(&mLook, vUpdatedLook);
}

void Camera::RotateY(float angle)
{
	XMMATRIX globalYRotationMX = XMMatrixRotationY(angle);

	//Grab all camera axes
	XMVECTOR vRight = XMLoadFloat3(&mRight);
	XMVECTOR vLook = XMLoadFloat3(&mLook);
	XMVECTOR vUp = XMLoadFloat3(&mUp);

	//Don't use YRotationMX's position vector when multiplying
	XMVECTOR vUpdatedRight = XMVector3TransformNormal(vRight, globalYRotationMX);
	XMVECTOR vUpdatedUp = XMVector3TransformNormal(vUp, globalYRotationMX);
	XMVECTOR vUpdatedLook = XMVector3TransformNormal(vLook, globalYRotationMX);

	XMStoreFloat3(&mRight, vUpdatedRight);
	XMStoreFloat3(&mLook, vUpdatedLook);
	XMStoreFloat3(&mUp, vUpdatedUp);
}

void Camera::Look(float xAngle, float yAngle, float zAngle)
{
	//Pitch(X), Yaw(Y), Roll Order(Z) (Ignore Function Name)
	XMMATRIX globalRotationMX = XMMatrixRotationRollPitchYaw(xAngle, yAngle, zAngle);


	//Loaded
	XMVECTOR vRight = XMLoadFloat3(&mRight);
	XMVECTOR vUp = XMLoadFloat3(&mUp);
	XMVECTOR vLook = XMLoadFloat3(&mLook);

	
	
	

	//Saved
	XMStoreFloat3(&mRight, XMVector3TransformNormal(vRight, globalRotationMX));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(vUp, globalRotationMX));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(vLook, globalRotationMX));
}

void Camera::UpdateViewMatrix()
{
	//Load
	XMVECTOR vRight = XMLoadFloat3(&mRight);
	XMVECTOR vUp = XMLoadFloat3(&mUp);
	XMVECTOR vLook = XMLoadFloat3(&mLook);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);

	//(Camera's Orientation)
	//Re-Orthonormalize Basis Vectors
	vLook = XMVector3Normalize(vLook);
	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
	vRight = XMVector3Cross(vUp, vLook);
	
	XMStoreFloat3(&mRight, vRight);
	XMStoreFloat3(&mUp, vUp);
	XMStoreFloat3(&mLook, vLook);

	//(Camera's Position)
	//newPos = currPos * normalizedDirectionVec
	float newXPos = -XMVectorGetX(XMVector3Dot(vPosition, vRight));
	float newYPos = -XMVectorGetX(XMVector3Dot(vPosition, vUp));
	float newZPos = -XMVectorGetX(XMVector3Dot(vPosition, vLook));

	//Save
	///mView(Row, Col)
	mView(0, 0) = mRight.x;
	mView(0, 1) = mRight.y;
	mView(0, 2) = mRight.z;
	mView(0, 3) = 0.0f;

	mView(1, 0) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(1, 2) = mUp.z;
	mView(1, 3) = 0.0f;

	mView(2, 0) = mLook.x;
	mView(2, 1) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(2, 3) = 0.0f;

	mView(3, 0) = newXPos;
	mView(3, 1) = newYPos;
	mView(3, 2) = newZPos;
	mView(3, 3) = 1.0f;
}