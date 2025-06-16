// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Containers/Ticker.h"

/**
 * Tick component.
 */
struct COMMONSUBSYSTEMS_API FCommonTickComponent
{
public:
	DECLARE_DELEGATE_OneParam(
		FTickSignature,
		float DeltaSeconds);

public:
	virtual ~FCommonTickComponent();

protected:
	/**
	 * Custom initilization function.
	 * @param	Callback callback to execute each tick.
	 */
	void Tick_Initialize(const FTickSignature& Callback);

	/**
	 * Custom deinitilization function.
	 */
	void Tick_Deinitialize();

	/**
	 * Change ticking state of this subsystem.
	 * @param	bEnable if true, subsystem will tick, if false, it won't.
	 */
	void EnableTick(bool bEnable);

	/**
	 * Set tick interval time in seconds.
	 * @param	InTickInterval interval between ticks. 0 means every frame.
	 */
	void SetTickIntervalTime(float InTickInterval);

	/**
	 * Check whether this subsystem is ticking.
	 * @return	True if subsystem is ticking, false otherwise.
	 */
	bool IsTickEnabled() const;

	/**
	 * Get tick interval time in seconds.
	 * @return	Interval between ticks. 0 means one frame of interval.
	 */
	float GetTickIntervalTime() const;

private:
	/**
	 * Wrapper of Tick function.
	 * @param	DeltaSeconds time since last tick.
	 */
	bool Tick_Implementation(float DeltaSeconds);

	/**
	 * Start ticking.
	 */
	void StartTicking();

	/**
	 * Stop ticking.
	 */
	void StopTicking();

private:
	/** Fired each tick. */
	FTickSignature TickDelegate;

protected:
	/** If true, component will start with tick enabled, false otherwise. */
	bool bStartWithTickEnabled = false;

	/** Initial time between ticks. 0 means one frame of interval. */
	float TickInterval = 0.f;

private:
	/** Delegate handle for custom tick function. */
	FTSTicker::FDelegateHandle TickHandle;

	/** If true, subsystem is ticking, false otherwise. */
	bool bIsTickEnabled = false;

	/** Time between ticks. 0 means one frame of interval. */
	float InternalTickInterval = 0.f;
};
