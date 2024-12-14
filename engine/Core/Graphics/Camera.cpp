/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Camera.h"
#include <Graphics/D3D11/Direct3DDevice.h>
#include <ThirdParty/ImGUI/imgui.h>

using namespace DirectX;
static constexpr DirectX::XMVECTOR DEFAULT_UP = { 0.f, 1.f, 0.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_FORWARD = { 0.f, 0.f, 1.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_BACKWARD = { 0.f, 0.f, -1.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_LEFT = { -1.f, 0.f, 0.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_RIGHT = { 1.f, 0.f, 0.f, 0.f };


void Camera::Render()
{
	// Create rotation matrix from rotation angles (in radians)
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(currentRotation.x), // Pitch (X-axis)
		DirectX::XMConvertToRadians(currentRotation.y), // Yaw (Y-axis)
		DirectX::XMConvertToRadians(currentRotation.z)  // Roll (Z-axis)
	);

	DirectX::XMFLOAT3 posConverted = { currentPosition.x, currentPosition.y, currentPosition.z };
	DirectX::XMVECTOR currentPos = DirectX::XMLoadFloat3(&posConverted);

	DirectX::XMVECTOR currentLookAt = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	DirectX::XMVECTOR currentUp = DirectX::XMVector3TransformCoord(DEFAULT_UP, rotation);

	currentLookAt = DirectX::XMVectorAdd(currentPos, currentLookAt);

	viewMatrix = DirectX::XMMatrixLookAtLH(currentPos, currentLookAt, currentUp);
}


void Camera::initDebug()
{
	if (ImGui::Begin("Transform Camera"))
	{
		ImGui::SliderFloat("TX", &currentPosition.x, -100, 150);
		ImGui::SliderFloat("TY", &currentPosition.y, -100, 150);
		ImGui::SliderFloat("TZ", &currentPosition.z, -100, 150);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("RX", &currentRotation.x, -100, 150);
		ImGui::SliderFloat("RY", &currentRotation.y, -100, 150);
		ImGui::SliderFloat("RZ", &currentRotation.z, -100, 150);
		ImGui::End();
	}
}


void Camera::setProjectionMatrix(float fov, float aspectRatio, float nearZ, float farZ)
{
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ);
}

	//DirectX::XMStoreFloat4x4(&projectionMatrix,
	//	DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ));

void Camera::updateViewMatrix()
{
	DirectX::XMFLOAT3 posConverted = { currentPosition.x, currentPosition.y, currentPosition.z };

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&posConverted);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(currentRotation.x),
		DirectX::XMConvertToRadians(currentRotation.y),
		DirectX::XMConvertToRadians(currentRotation.z)
	);

	target = DirectX::XMVector3TransformCoord(target, rotation);
	up = DirectX::XMVector3TransformCoord(up, rotation);

	target = DirectX::XMVectorAdd(pos, target);

	viewMatrix = DirectX::XMMatrixLookAtLH(pos, target, up);
}

void Camera::bindRender()
{
}

void Camera::unbindRender()
{
}
