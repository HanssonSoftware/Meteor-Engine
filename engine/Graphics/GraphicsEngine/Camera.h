/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Object/Object.h>
#include <DirectXMath.h>
#include <GraphicsEngine/QueuedRender.h>
#include <Types/Vector.h>

using namespace DirectX;
static constexpr DirectX::XMVECTOR DEFAULT_UP = { 0.f, 1.f, 0.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_FORWARD = { 0.f, 0.f, 1.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_BACKWARD = { 0.f, 0.f, -1.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_LEFT = { -1.f, 0.f, 0.f, 0.f };
static constexpr DirectX::XMVECTOR DEFAULT_RIGHT = { 1.f, 0.f, 0.f, 0.f };

class Camera : public Object, public IQueuedRender
{
public:
    Camera();

    ~Camera();

    void Frame(float deltaTime);

    void setFOV(float angle, float nearZ,float farZ);

    void setPosition(const Vector3<float>& newPos);

    void setRotation(const Vector3<float>& newRot);

    Vector3<float> getPosition() const 
    {
        return { currentPosition.x, currentPosition.y, currentPosition.z };
    }

    Vector3<float> getRotation() const 
    {
        return { currentRotation.x, currentRotation.y, currentRotation.z };
    }

    void initDebug();

    void setProjectionMatrix(float fov, float aspectRatio, float nearZ, float farZ);

    virtual void Render() override;


    DirectX::XMMATRIX getViewMatrix() const noexcept
    {
        return viewMX;
    }

    DirectX::XMMATRIX getProjectionMatrix() const
    {
        return projMX;
    }

private:
    Vector3<float> currentPosition;

    Vector3<float> currentLookAt;

    Vector3<float> currentRotation;

    void updateViewMatrix();

    DirectX::XMMATRIX viewMX;
    DirectX::XMMATRIX projMX;

    // Inherited via IQueuedRender
    void bindRender() override;

    void unbindRender() override;
};
