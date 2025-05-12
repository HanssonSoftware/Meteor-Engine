/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Layers/Layer.h>
#include <Types/Vector.h>

class IWindow;
class IThreadPool;

struct MessageBoxDescriptor
{
	String Title;

	String Description;

	/** */
	IWindow* dependentWindow;

	/** Defines your message type, can be multiple: Immediate action / Question. */
	int Type;
};


enum MessageBoxType
{
	MESSAGEBOXTYPE_EMPTY = 1 << 1,
	MESSAGEBOXTYPE_INFORMATION = 1 << 2,
	MESSAGEBOXTYPE_WARNING = 1 << 3,
	MESSAGEBOXTYPE_ERROR = 1 << 4,

	/** Supply dependentWindow! */
	MESSAGEBOXTYPE_SERIOUS = 1 << 5,
	/** Supply dependentWindow! */
	MESSAGEBOXTYPE_IMPORTANT = 1 << 6,
	/** Supply dependentWindow! */
	MESSAGEBOXTYPE_IMMEDIATE_ACTION = 1 << 7,
};

enum MessageBoxButtons
{
	MESSAGEBOXBUTTONS_OK = 1 << 8,
	MESSAGEBOXBUTTONS_YES = 1 << 9,
	MESSAGEBOXBUTTONS_NO = 1 << 10,
	MESSAGEBOXBUTTONS_CANCEL = 1 << 11
};

enum MessageBoxDecision
{
	MESSAGEBOXDECISION_INVALID = -1,
	MESSAGEBOXDECISION_OK = 0,
	MESSAGEBOXDECISION_CANCEL = 1,
	MESSAGEBOXDECISION_RETRY = 2,
	MESSAGEBOXDECISION_STOP = 3,
	MESSAGEBOXDECISION_YES = 4,
	MESSAGEBOXDECISION_NO = 5,
	MESSAGEBOXDECISION_CUSTOM = 6,
	MESSAGEBOXDECISION_CUSTOM_1 = 7,
	MESSAGEBOXDECISION_CUSTOM_2 = 8,
	MESSAGEBOXDECISION_CUSTOM_3 = 9
};

class OSLayer : public Layer
{
	friend class LayerManager;

public:
	OSLayer(const String Name) noexcept;

	OSLayer() = delete;

	virtual ~OSLayer() = default;

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
