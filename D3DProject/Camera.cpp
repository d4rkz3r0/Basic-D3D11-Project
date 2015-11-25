#include "Camera.h"

//Builds Default Camera at the Origin
Camera::Camera() :
	mPos(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f), 
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	//BuildFrustum(XM_PIDIV2, 1.0f, 0.05f, 1000.f);
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

//XMMATRIX Camera::GetProj() const
//{
//	return XMLoadFloat4x4(&mProj);
//}

////Returns the "VP" part of the WVP Matrix
////Call this after UpdateViewMatrix();
//XMMATRIX Camera::GetViewProj() const
//{
//	return (XMMatrixMultiply(GetView(), GetProj()));
//}

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

////Builds the "P" part of the WVP Matrix
//void Camera::BuildFrustum(float foVY, float aspectRatio, float nearZ, float farZ)
//{
//	//Save Properties
//	mFoVY = foVY;
//	mAspect = aspectRatio;
//	mNearZ = nearZ;
//	mFarZ = farZ;
//	
//	//Save Result in friendly format
//	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(mFoVY, mAspect, mNearZ, mFarZ));
//}


//Prep to Build View Matrix
void Camera::PrepLookAt(XMFLOAT3& position, XMFLOAT3& target, XMFLOAT3& up)
{
	//Load necessary floats into XMVECTORS for XM Calcs
	XMVECTOR vPosition = XMLoadFloat3(&position);
	XMVECTOR vTarget = XMLoadFloat3(&target);
	XMVECTOR vUp = XMLoadFloat3(&up);

	//Use Position, Target, and Up to calculate
	//the Look, Right, newUP
	BuildLookAt(vPosition, vTarget, vUp);
}

//Prep to Build View Matrix - Calculate proper perpendicular vectors 
void Camera::BuildLookAt(XMVECTOR _vPosition, XMVECTOR _vTarget, XMVECTOR _vUp)
{
	//+Z
	//Calculate Vector that would be the camera's forward axis if you were facing a target.
	//Remove it's magnitude (length) since we only care about the direction of travel.
	XMVECTOR vLook = XMVector3Normalize(XMVectorSubtract(_vTarget, _vPosition));

	//+X
	//Calculate Vector that would be the camera's RIGHT if it were perpendicular between arg1 and arg2.
	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(_vUp, vLook));

	//+Y
	//Calculate Vector that would be the camera's relative UP if it were perpendicular between arg1 and arg2.
	XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	//Save results to floats
	XMStoreFloat3(&mRight, vRight);  //1st row of View Matrix - X Vec
	XMStoreFloat3(&mUp, vUp);        //2nd row of View Matrix - Y Vec
	XMStoreFloat3(&mLook, vLook);    //3rd row of View Matrix - Z Vec
	XMStoreFloat3(&mPos, _vPosition); //4th row of View Matrix - Pos Vec
}

//Local Left and Right Translation Offset
void Camera::Strafe(float moveSpeed)
{
	//I wish
	//mPos.z += moveAmount;

	//speed into a vector
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	//Pull just the floats we need (Left/Right Movement means just cam's XVec) into Vectors For XM Calculations
	XMVECTOR vPosition = XMLoadFloat3(&mPos);
	XMVECTOR vRight = XMLoadFloat3(&mRight);

	//Beautiful Function
	//newCameraPosition = (directionToMove * vectorYouNeedToMove) + camera's current Position Vector
	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vRight, vPosition));
}

void Camera::Walk(float moveSpeed)
{
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);
	XMVECTOR vLook = XMLoadFloat3(&mLook);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vLook, vPosition));
}

//X Axis Rotation
void Camera::Pitch(float angle)
{
	//Build a MX around the camera's X to merge into View MX
	XMMATRIX mxRotation = XMMatrixRotationNormal(XMLoadFloat3(&mRight), angle);

	//Extract floats into Vectors we need
	//Will be affecting the View MX's up and look, y vec and z vec, respectively.
	XMVECTOR vUp = XMLoadFloat3(&mUp);
	XMVECTOR vLook = XMLoadFloat3(&mLook);

	//newViewMXRow = currMXRow * transformationMX, not using transformationMX's 4th, positional row.
	XMVECTOR vUpdatedUp = XMVector3TransformNormal(vUp, mxRotation); //2nd Row
	XMVECTOR vUpdatedLook = XMVector3TransformNormal(vLook, mxRotation); //3rd Row

	XMStoreFloat3(&mUp, vUpdatedUp);
	XMStoreFloat3(&mLook, vUpdatedLook);
}

void Camera::RotateY(float angle)
{
	//Global Y MX
	XMMATRIX mxRotation = XMMatrixRotationY(angle);
	XMVECTOR vSavedPos = XMLoadFloat3(&mPos);
	XMMATRIX mxOrigin = XMMatrixTranslationFromVector(XMVectorZero());
	XMMATRIX mxCurrView = XMLoadFloat4x4(&mView);
	mxCurrView = XMMatrixMultiply(mxOrigin, mxOrigin);
	mxCurrView = XMMatrixTranslationFromVector(vSavedPos);

	XMStoreFloat4x4(&mView, mxCurrView);


	////Going to Be affecting All rows since this is a Global Rotation!
	//XMVECTOR vRight = XMLoadFloat3(&mRight);
	//XMVECTOR vLook = XMLoadFloat3(&mLook);
	//XMVECTOR vUp = XMLoadFloat3(&mUp);

	////newViewMXRow = currMXRow * transformationMX, not using transformationMX's 4th, positional row.
	//XMVECTOR vNewRight = XMVector3TransformNormal(vRight, mxRotation);
	//XMVECTOR vNewUp = XMVector3TransformNormal(vUp, mxRotation);
	//XMVECTOR vNewLook = XMVector3TransformNormal(vLook, mxRotation);

	////Save out
	//XMStoreFloat3(&mRight, vNewRight);
	//XMStoreFloat3(&mLook, vNewLook);
	//XMStoreFloat3(&mUp, vNewUp);

}

void Camera::VFly(float moveSpeed)
{
	XMVECTOR vGlobalY = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR vMoveDirection = XMVectorReplicate(moveSpeed);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);

	//MoveDirection * GlobalY + currPos = newPos
	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(vMoveDirection, vGlobalY, vPosition));
}

//Maintain Orthronormal Property of View Matrix after
//all of the transformations
void Camera::UpdateViewMatrix()
{
	//Extract all relevant floats into Vectors
	XMVECTOR vLook = XMLoadFloat3(&mLook);
	XMVECTOR vUp = XMLoadFloat3(&mUp);
	XMVECTOR vRight = XMLoadFloat3(&mRight);
	XMVECTOR vPosition = XMLoadFloat3(&mPos);

	////Rebuild View MX Input Row Vecs 1-3 in a Orthronormal fashion
	//Look doesn't need Cross Product to find
	vLook = XMVector3Normalize(vLook);
	//Up needs Look and Right crossed
	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
	//Right is using normalized Look and Up so the result is normalized already
	vRight = XMVector3Cross(vUp, vLook);

	//Store into floats
	XMStoreFloat3(&mLook, vLook);
	XMStoreFloat3(&mUp, vUp);
	XMStoreFloat3(&mRight, vRight);

	//4th Column Values
	float mRightW = 0.0f;
	float mUpW	  = 0.0f;
	float mLookW  = 0.0f;
	float newPositionVecWComponent = 1.0f;

	//Each Position Vector Component must be recalculated
	//newPositionComponent = currPositionVec * currAppropriateNormailzedAxis
	float newPositionVecXComponent = -XMVectorGetX(XMVector3Dot(vPosition, vRight));
	float newPositionVecYComponent = -XMVectorGetX(XMVector3Dot(vPosition, vUp));
	float newPositionVecZComponent = -XMVectorGetX(XMVector3Dot(vPosition, vLook));
	
	////Populate View Matrix - Finally
	//1st Row - X Right Vector
	mView(0, 0) = mRight.x;
	mView(0, 1) = mRight.y;
	mView(0, 2) = mRight.z;
	mView(0, 3) = mRightW;
	//2nd Row - Y Up Vector
	mView(1, 0) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(1, 2) = mUp.z;
	mView(1, 3) = mUpW;
	//3rd Row - Z Look Vector
	mView(2, 0) = mLook.x;
	mView(2, 1) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(2, 3) = mLookW;
	//4th Row - Position Vector
	mView(3, 0) = newPositionVecXComponent;
	mView(3, 1) = newPositionVecYComponent;
	mView(3, 2) = newPositionVecZComponent;
	mView(3, 3) = newPositionVecWComponent;
}