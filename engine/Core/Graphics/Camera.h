/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Object/Object.h>
#include <DirectXMath.h>
#include <Graphics/QueuedRender.h>
#include <Types/Vector.h>

using namespace DirectX;

class Camera : public Object, public IQueuedRender
{
private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 target;
    DirectX::XMFLOAT3 up;

    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projectionMatrix;

public:
    Camera() {
        currentPosition = { 0.0f, 0.0f, -5.0f };
        currentRotation = { 0.0f, 0.0f, 0.0f };
        updateViewMatrix();
        setProjectionMatrix(70.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    }

    ~Camera() {}

    void Frame(float deltaTime) {
        // Placeholder for future per-frame updates, ha szükséges
    }

    void setPosition(const Vector3<float>& newPos) {
        currentPosition = { newPos.x, newPos.y, newPos.z };
        updateViewMatrix();
    }

    void setRotation(const Vector3<float>& newRot) {
        currentRotation = { newRot.x, newRot.y, newRot.z };
        updateViewMatrix();
    }

    Vector3<float> getPosition() const {
        return { currentPosition.x, currentPosition.y, currentPosition.z };
    }

    Vector3<float> getRotation() const {
        return { currentRotation.x, currentRotation.y, currentRotation.z };
    }

    void initDebug();

    virtual void Render() override;

    void setProjectionMatrix(float fov, float aspectRatio, float nearZ, float farZ);

    const DirectX::XMMATRIX& getViewMatrix() const {
        return viewMatrix;
    }

    const DirectX::XMMATRIX& getProjectionMatrix() const {
        return projectionMatrix;
    }

private:
    Vector3<float> currentPosition;

    Vector3<float> currentRotation;

    void updateViewMatrix();

    // Inherited via IQueuedRender
    void bindRender() override;
    void unbindRender() override;
};
