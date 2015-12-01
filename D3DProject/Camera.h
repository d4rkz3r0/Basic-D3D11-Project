#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
	public:
		Camera();
		~Camera();

		//Accessors & Mutators
		XMFLOAT3 GetRight();
		XMFLOAT3 GetUp();
		XMFLOAT3 GetLook();
		XMFLOAT3 GetPosition();
		void SetRight(float x, float y, float z);
		void SetUp(float x, float y, float z);
		void SetLook(float x, float y, float z);
		void SetPosition(float x, float y, float z);
		void SetPosition(XMFLOAT3& position);
		void SetTarget(float x, float y, float z);

		//SIMD Accessors
		XMVECTOR GetUpXM();
		XMVECTOR GetRightXM();
		XMVECTOR GetLookXM();
		XMVECTOR GetPositionXM();
		XMMATRIX GetView() const;
		XMMATRIX GetProj() const;
		XMMATRIX GetViewProj() const;

		//Camera & Frustum
		void SetFrustum(float foVY, float aspectRatio, float nearZ, float farZ);

		//View Matrix Helpers (Optional)
		void PrepView(XMFLOAT3& position, XMFLOAT3& target, XMFLOAT3& up);
		void SetView(XMVECTOR _vPosition, XMVECTOR _vTarget, XMVECTOR _vUp); 

		//Camera Transformation
		void VFly(float moveSpeed);   //newPos = (directionToMove * vectorYouNeedToMove) + currPos
		void Strafe(float moveSpeed); //newPos = (directionToMove * vectorYouNeedToMove) + currPos
		void Step(float moveSpeed);   //newPos = (directionToMove * vectorYouNeedToMove) + currPos
		void Pitch(float angle);      //newOrientation = (upVec & lookVec * rightMX)
		void RotateY(float angle);    //newOrientation = (allVecs * globalYMX)
		void Look(float xAngle, float yAngle, float zAngle); //Quaternion-Based Rotation

		bool mReverseMult = false;
		void UpdateViewMatrix(); //Maintain Orthonormalcy (Not a word)

	private:
		//Camera's Local Coordinate System
		XMFLOAT3 mRight;
		XMFLOAT3 mUp;
		XMFLOAT3 mLook;
		XMFLOAT3 mPos;

		//Frustum's Properties
		float mFoVY;
		float mAspect;
		float mNearZ;
		float mFarZ;

		//Camera and it's Frustum
		XMFLOAT4X4 mView;
		XMFLOAT4X4 mProj;

		//Misc
		const float mSpeedScalar = 10.0f;
};