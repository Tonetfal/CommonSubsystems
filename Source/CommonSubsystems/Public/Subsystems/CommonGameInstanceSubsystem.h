// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "CommonGameInstanceSubsystem.generated.h"

#define COMMON_SUBSYSTEMS_GAME_INSTANCE_BODY() \
	public: \
		static bool HasInstance(const UObject* ContextObject) \
		{ \
			check(IsValid(ContextObject)); \
			 \
			const UWorld* World = ContextObject->GetWorld(); \
			if (!IsValid(World)) \
			{ \
				return false; \
			} \
			\
			const UGameInstance* GameInstance = World->GetGameInstance(); \
			if (!IsValid(GameInstance)) \
			{ \
				return false; \
			} \
			 \
			const auto* ThisSubsystem = GameInstance->GetSubsystem<ThisClass>(); \
			if (!IsValid(ThisSubsystem)) \
			{ \
				return false; \
			} \
			\
			return true; \
		} \
		\
		static ThisClass& Get(const UObject* ContextObject) \
		{ \
			check(IsValid(ContextObject)); \
			\
			const UWorld* World = ContextObject->GetWorld(); \
			check(IsValid(World)); \
			\
			const UGameInstance* GameInstance = World->GetGameInstance(); \
			check(IsValid(GameInstance)); \
			\
			auto* ThisSubsystem = GameInstance->GetSubsystem<ThisClass>(); \
			check(IsValid(ThisSubsystem)); \
			\
			auto& ThisSubsystemRef = *ThisSubsystem; \
			return ThisSubsystemRef; \
		}

/**
 * Game instance subsystem with commonly used functions.
 */
UCLASS(Abstract)
class COMMONSUBSYSTEMS_API UCommonGameInstanceSubsystem
	: public UGameInstanceSubsystem
{
	GENERATED_BODY()
	// COMMON_SUBSYSTEMS_GAME_INSTANCE_BODY()
	// ^^^ Include this in your override of the subsystem ^^^
};
