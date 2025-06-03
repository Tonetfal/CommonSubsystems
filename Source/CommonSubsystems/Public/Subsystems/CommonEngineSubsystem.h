// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Subsystems/EngineSubsystem.h"

#include "CommonEngineSubsystem.generated.h"

#define COMMON_SUBSYSTEMS_ENGINE_BODY() \
	public: \
		static bool HasInstance(const UObject* ContextObject) \
		{ \
			/** \
			 * We don't do anything with the given object, however we ask for it to keep the common interface, and to make it \
			 * easy to change the nature of a subsystem without changing the getters. \
			 */ \
			 \
			if (!IsValid(ContextObject)) \
			{ \
				return false; \
			} \
			\
			const auto* ThisSubsystem = GEngine->GetEngineSubsystem<ThisClass>(); \
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
			/** \
			 * We don't do anything with the given object, however we ask for it to keep the common interface, and to make it \
			 * easy to change the nature of a subsystem without changing the getters. \
			 */ \
			 \
			check(IsValid(ContextObject)); \
			 \
			auto* ThisSubsystem = GEngine->GetEngineSubsystem<ThisClass>(); \
			check(IsValid(ThisSubsystem)); \
			\
			auto& ThisSubsystemRef = *ThisSubsystem; \
			return ThisSubsystemRef; \
		}

/**
 * Engine subsystem with commonly used functions.
 *
 * This subsystem is not really required due to the ease of retrieving an engine subsystem, but it's here just in case
 * some new functionality will be added.
 */
UCLASS(Abstract)
class COMMONSUBSYSTEMS_API UCommonEngineSubsystem
	: public UEngineSubsystem
{
	GENERATED_BODY()
	// COMMON_SUBSYSTEMS_ENGINE_BODY()
	// ^^^ Include this in your override of the subsystem ^^^
};
