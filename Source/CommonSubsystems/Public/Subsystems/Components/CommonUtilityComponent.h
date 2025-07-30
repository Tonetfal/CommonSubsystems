// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#define COMMON_SUBSYSTEMS_UTILITY_COMPONENT_BODY() \
	public: \
		FTimerManager& GetTimerManager() const \
		{ \
			const UWorld* World = GetWorld(); \
			check(IsValid(World)); \
			\
			FTimerManager& TimerManager = World->GetTimerManager(); \
			return TimerManager; \
		}

struct COMMONSUBSYSTEMS_API FCommonUtilityComponent
{
};
