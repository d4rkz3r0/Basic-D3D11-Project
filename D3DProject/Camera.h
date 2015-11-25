//Notes: Save and Load to and from FLOATS, use temporary XMVECTORS
//for XM Math Calculations but always save your calculations
//to the freaking class floats!
#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
	public:
		//Constructor & Destructor
		Camera();
		~Camera();

		//Accessors & Mutators
		XMFLOAT3 GetRight();
		XMVECTOR GetRightXM();
		XMFLOAT3 GetUp();
		XMVECTOR GetUpXM();
		XMFLOAT3 GetLook();
		XMVECTOR GetLookXM();
		XMFLOAT3 GetPosition();
		XMVECTOR GetPositionXM();
		XMMATRIX GetView() const;
		//XMMATRIX GetProj() const;
		//XMMATRIX GetViewProj() const;
		void SetRight(float x, float y, float z);
		void SetUp(float x, float y, float z);
		void SetLook(float x, float y, float z);
		void SetPosition(float x, float y, float z);
		void SetPosition(XMFLOAT3& position);

		//Camera & Frustum
		//void BuildFrustum(float foVY, float aspectRatio, float nearZ, float farZ);
		void PrepLookAt(XMFLOAT3& position, XMFLOAT3& target, XMFLOAT3& up);
		void BuildLookAt(XMVECTOR _vPosition, XMVECTOR _vTarget, XMVECTOR _vUp); //FPS Style Y UpVec

		////Main Camera Functions
		//Local Translation
		void Strafe(float moveSpeed);
		void Walk(float moveSpeed);
		//Local Rotation - Quaternion Support? Nope.avi
		void Pitch(float angle);
		//Global Rotation
		void RotateY(float angle);
		//Global Y Translation
		void VFly(float moveSpeed);

		void UpdateViewMatrix();

	private:
		////Camera's Local Coordinate System
		//Floats for easy storing and loading
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
		//XMFLOAT4X4 mProj;
};