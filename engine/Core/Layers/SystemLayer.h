/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Layers/Layer.h>
#include <Types/Vector.h>
#include "SystemLayerHelper.h"

class IThreadPool;


class SystemLayer : public Layer
{
	friend class LayerManager;

public:
	SystemLayer(const String Name) noexcept;

	SystemLayer() = delete;

	virtual ~SystemLayer() = default;

	/** This is not automatic! Please call if you want post-Attached phase. */
	virtual void Init() override;

	virtual void Attached() override;

	virtual void Removed() override;

	virtual void Update() override;

	virtual bool IsRunningAnAnotherInstance() = 0;

	virtual String GetMachineVersion() = 0;

	virtual String GetError();

	virtual Vector3<float> InspectPixel(const Vector2<float> screenCoordinates) = 0;

	virtual String GetMachineName() = 0;

	virtual MessageBoxDecision AddMessageBox(const MessageBoxDescriptor* Info) = 0;
	
	/** Converts your "skinny" buffer to "fat". You must release, after working with it! */
	virtual wchar_t* ConvertToWide(const char* Buffer) = 0;

	/** Converts your "fat" buffer to "skinny". You must release, after working with it! */
	virtual char* ConvertToNarrow(const wchar_t* Buffer) = 0;

	IThreadPool* GetThreadPool() const { return threadPool; };

protected:
	IThreadPool* threadPool;
};
