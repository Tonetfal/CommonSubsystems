// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "CommonWorldSubsystem.generated.h"

#define COMMON_SUBSYSTEMS_WORLD_BODY() \
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
			const auto* ThisSubsystem = World->GetSubsystem<ThisClass>(); \
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
			auto* ThisSubsystem = World->GetSubsystem<ThisClass>(); \
			check(IsValid(ThisSubsystem)); \
			 \
			auto& ThisSubsystemRef = *ThisSubsystem; \
			return ThisSubsystemRef; \
		} \

/**
 * World subsystem with commonly used functions.
 */
UCLASS(Abstract)
class COMMONSUBSYSTEMS_API UCommonWorldSubsystem
	: public UWorldSubsystem
{
	GENERATED_BODY()
	// COMMON_SUBSYSTEMS_WORLD_BODY()
	// ^^^ Include this in your override of the subsystem ^^^
};
