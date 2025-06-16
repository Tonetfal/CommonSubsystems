// Author: Antonio Sidenko (Tonetfal), July 2023

#include "Subsystems/Components/CommonTickComponent.h"

FCommonTickComponent::~FCommonTickComponent()
{
	Tick_Deinitialize();
}

void FCommonTickComponent::Tick_Initialize(const FTickSignature& Callback)
{
	TickDelegate = Callback;

	bIsTickEnabled = bStartWithTickEnabled;
	InternalTickInterval = TickInterval;

	if (bIsTickEnabled)
	{
		StartTicking();
	}
}

void FCommonTickComponent::Tick_Deinitialize()
{
	if (bIsTickEnabled)
	{
		StopTicking();
	}
}

void FCommonTickComponent::EnableTick(bool bEnable)
{
	if (bIsTickEnabled == bEnable)
	{
		return;
	}

	if (bEnable)
	{
		StartTicking();
	}
	else
	{
		StopTicking();
	}
}

void FCommonTickComponent::SetTickIntervalTime(float InTickInterval)
{
	if (InTickInterval < 0.f)
	{
		return;
	}

	InternalTickInterval = InTickInterval;
	if (bIsTickEnabled)
	{
		StopTicking();
		StartTicking();
	}
}

bool FCommonTickComponent::IsTickEnabled() const
{
	return bIsTickEnabled;
}

float FCommonTickComponent::GetTickIntervalTime() const
{
	return InternalTickInterval;
}

bool FCommonTickComponent::Tick_Implementation(float DeltaSeconds)
{
	TickDelegate.Execute(DeltaSeconds);
	return true;
}

void FCommonTickComponent::StartTicking()
{
	ensure(bIsTickEnabled);
	check(!TickHandle.IsValid());

	FTickerDelegate Delegate;
	Delegate.BindRaw(this, &FCommonTickComponent::Tick_Implementation);

	FTSTicker& Ticker = FTSTicker::GetCoreTicker();
	TickHandle = Ticker.AddTicker(Delegate, InternalTickInterval);

	bIsTickEnabled = true;
}

void FCommonTickComponent::StopTicking()
{
	ensure(bIsTickEnabled);
	check(TickHandle.IsValid());

	FTSTicker& Ticker = FTSTicker::GetCoreTicker();
	Ticker.RemoveTicker(TickHandle);

	bIsTickEnabled = false;
}
