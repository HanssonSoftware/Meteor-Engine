/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Layers/OSLayer.h>

class WinLayer : public OSLayer
{
public:
	WinLayer(const String Name) noexcept;

	WinLayer() = delete;

	virtual ~WinLayer() = default;

	virtual void Init() override;

	virtual void Attached() override;

	virtual void Removed() override;

	virtual void Update() override;

	virtual String GetError() override;

	virtual String GetMachineVersion() override;

	virtual Vector3<float> InspectPixel(const Vector2<float> screenCoordinates) override;

	virtual String GetMachineName() override;

	virtual bool IsRunningAnAnotherInstance() override;

	virtual MessageBoxDecision AddMessageBox(const MessageBoxDescriptor* Info);

	/** Converts your "skinny" buffer to "fat". You must release, after working with it! */
	virtual wchar_t * ConvertToWide(const char* Buffer) override;
	
	/** Converts your "fat" buffer to "skinny". You must release, after working with it! */
	virtual char * ConvertToNarrow(const wchar_t* Buffer) override;

private:
	constexpr const int evaluateMessageBoxFlags(const int Code) const noexcept;

	constexpr const MessageBoxDecision evaluateMessageBoxReturn(const int Code) const noexcept;
};

