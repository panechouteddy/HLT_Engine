#include "pch.h"
#include "hlt_Transform3D.h"


void hlt_Transform3D::Move(float distance)
{
	pos.x += front.x * distance;
	pos.y += front.y * distance;
	pos.z += front.z * distance;
}

void hlt_Transform3D::Move(float distance, XMFLOAT3 dir)
{
	XMFLOAT3 saveFront = front;
	front = dir;
	Move(distance);
	front = saveFront;
}

void hlt_Transform3D::SetScale(float scale)
{
	sca.x = scale;
	sca.y = scale;
	sca.z = scale;
}

void hlt_Transform3D::ResetRotation()
{
	front = { 0.f, 0.f, 1.f };
	right = { 1.f, 0.f, 0.f };
	up = { 0.f, 1.f, 0.f };
	quaternion = { 0.f, 0.f, 0.f, 1.f };
	XMStoreFloat4x4(&rotation, XMMatrixIdentity());
}

void hlt_Transform3D::Identity()
{
	pos = { 0.f, 0.f, 0.f };
	SetScale(1.f);
	XMStoreFloat4x4(&rotation, XMMatrixIdentity());
}

void hlt_Transform3D::SetRotation(hlt_Transform3D& transform)
{
	front = transform.front;
	right = transform.right;
	up = transform.up;
	quaternion = transform.quaternion;
	XMStoreFloat4x4(&rotation, XMLoadFloat4x4(&transform.rotation));
}

void hlt_Transform3D::AddYPR(FXMVECTOR ypr)
{
	XMFLOAT3 yprVector;
	XMStoreFloat3(&yprVector, ypr);
	XMVECTOR frontAxis = XMLoadFloat3(&front);
	XMVECTOR rightAxis = XMLoadFloat3(&right);
	XMVECTOR upAxis = XMLoadFloat3(&up);

	XMVECTOR qRot = XMLoadFloat4(&quaternion);
	if (yprVector.x)
		qRot = XMQuaternionMultiply(qRot, XMQuaternionRotationAxis(frontAxis, yprVector.x));
	if (yprVector.y)
		qRot = XMQuaternionMultiply(qRot, XMQuaternionRotationAxis(rightAxis, yprVector.y));
	if (yprVector.z)
		qRot = XMQuaternionMultiply(qRot, XMQuaternionRotationAxis(upAxis, yprVector.z));

	qRot = XMQuaternionNormalize(qRot);
	XMStoreFloat4(&quaternion, qRot);

	XMMATRIX mRot = XMMatrixRotationQuaternion(qRot);
	XMStoreFloat4x4(&rotation, mRot);

	right.x = rotation._11;
	right.y = rotation._12;
	right.z = rotation._13;
	up.x = rotation._21;
	up.y = rotation._22;
	up.z = rotation._23;
	front.x = rotation._31;
	front.y = rotation._32;
	front.z = rotation._33;
}

void hlt_Transform3D::AddYPR(float yaw, float pitch, float raw)
{
	XMFLOAT3 ypr{ yaw, pitch, raw };
	AddYPR(XMLoadFloat3(&ypr));
}

void hlt_Transform3D::SetYPR(FXMVECTOR ypr)
{
	ResetRotation();
	AddYPR(ypr);
}

void hlt_Transform3D::OrbitAround()
{
}