/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Camera.h"
//#include <Graphics/D3D11/Direct3DDevice.h>
//#include <ThirdParty/ImGUI/imgui.h>
#include <Application/Application.h>
#include <Window/WindowManager.h>

using namespace DirectX;

Camera::Camera()
{
	setPosition({ 0.f, 1.f, 0.f });
}

Camera::~Camera()
{

}

void Camera::Render()
{
	DirectX::XMFLOAT3 currentEyePos = { currentPosition.x, currentPosition.y, currentPosition.z };
	DirectX::XMVECTOR currentEyePosVec = DirectX::XMLoadFloat3(&currentEyePos);
	DirectX::XMMATRIX B = DirectX::XMMatrixRotationRollPitchYaw(currentRotation.x, currentRotation.y, currentRotation.z);

	DirectX::XMVECTOR targetDirectory = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, B);
	targetDirectory += currentEyePosVec;

	DirectX::XMVECTOR upDirectroy = DirectX::XMVector3TransformCoord(DEFAULT_UP, B);

	viewMX = DirectX::XMMatrixLookAtLH(currentEyePosVec, targetDirectory, upDirectroy);
}

void Camera::Frame(float deltaTime)
{

}

void Camera::setFOV(float angle, float nearZ, float farZ)
{
	static float aspect = 0.f;
	if (Application::Get() && aspect != 0.f)
	{
		Vector2<uint32> d = Application::Get()->getWindowSize();

		aspect = (float)d.x / (float)d.y;
	}

	projMX = DirectX::XMMatrixPerspectiveFovLH(angle, aspect, nearZ, farZ);
}

void Camera::setPosition(const Vector3<float>& newPos)
{
	currentPosition = newPos;
}

void Camera::setRotation(const Vector3<float>& newRot)
{
	currentRotation = newRot;
}

void Camera::initDebug()
{
	//if (ImGui::Begin("Transform Camera"))
	//{
	//	ImGui::SliderFloat("TX", &currentPosition.x, -100, 150);
	//	ImGui::SliderFloat("TY", &currentPosition.y, -100, 150);
	//	ImGui::SliderFloat("TZ", &currentPosition.z, -100, 150);
	//	ImGui::Text("Rotation");
	//	ImGui::SliderFloat("RX", &currentRotation.x, -100, 150);
	//	ImGui::SliderFloat("RY", &currentRotation.y, -100, 150);
	//	ImGui::SliderFloat("RZ", &currentRotation.z, -100, 150);
	//	ImGui::End();
	//}
}


void Camera::setProjectionMatrix(float fov, float aspectRatio, float nearZ, float farZ)
{
	Application* app = Application::Get();
	if (!app)
		return;

	const Vector2<uint32> windowSize = app->getWindowManager()->getFirstWindow()->getSize();
	const float aspect = (float)windowSize.x / (float)windowSize.y;

}

	//DirectX::XMStoreFloat4x4(&projectionMatrix,
	//	DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ));

void Camera::updateViewMatrix()
{

}

void Camera::bindRender()
{
}

void Camera::unbindRender()
{
}
