// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Engine/LocalPlayer.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "CommonLocalPlayerSubsystem.generated.h"

#define COMMON_SUBSYSTEMS_LOCAL_PLAYER_BODY() \
	public: \
		static bool HasInstance(const UObject* ContextObject, int32 LocalPlayerNum = 0) \
		{ \
			if (!IsValid(ContextObject)) \
			{ \
				return false; \
			} \
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
			const TArray<ULocalPlayer*>& LocalPlayers = GameInstance->GetLocalPlayers(); \
			if (!LocalPlayers.IsValidIndex(LocalPlayerNum)) \
			{ \
				return false; \
			} \
			\
			const ULocalPlayer* LocalPlayer = LocalPlayers[LocalPlayerNum]; \
			const bool bHasInstance = HasInstance(LocalPlayer); \
			return bHasInstance; \
		} \
		\
		static bool HasInstance(const ULocalPlayer* LocalPlayer) \
		{ \
			if (!IsValid(LocalPlayer)) \
			{ \
				return false; \
			} \
			\
			const auto* ThisSubsystem = LocalPlayer->GetSubsystem<ThisClass>(); \
			if (!IsValid(ThisSubsystem)) \
			{ \
				return false; \
			} \
			\
			return true; \
		} \
		\
		static bool HasInstance(const ULocalPlayerSubsystem* LocalPlayerSubsystem) \
		{ \
			if (!IsValid(LocalPlayerSubsystem)) \
			{ \
				return false; \
			} \
			\
			const auto* LocalPlayer = LocalPlayerSubsystem->GetLocalPlayer<ULocalPlayer>(); \
			const bool bHasInstance = HasInstance(LocalPlayer); \
			return bHasInstance; \
		} \
		\
		static ThisClass& Get(const UObject* ContextObject, int32 LocalPlayerNum = 0) \
		{ \
			check(IsValid(ContextObject)); \
			\
			const UWorld* World = ContextObject->GetWorld(); \
			check(IsValid(World)); \
			\
			const UGameInstance* GameInstance = World->GetGameInstance(); \
			check(IsValid(GameInstance)); \
			\
			const TArray<ULocalPlayer*>& LocalPlayers = GameInstance->GetLocalPlayers(); \
			check(LocalPlayers.IsValidIndex(LocalPlayerNum)); \
			\
			const ULocalPlayer* LocalPlayer = LocalPlayers[LocalPlayerNum]; \
			check(IsValid(LocalPlayer)); \
			\
			auto& ThisSubsystemRef = Get(LocalPlayer); \
			return ThisSubsystemRef; \
		} \
		\
		static ThisClass& Get(const ULocalPlayer* LocalPlayer) \
		{ \
			check(IsValid(LocalPlayer)); \
			\
			auto* ThisSubsystem = LocalPlayer->GetSubsystem<ThisClass>(); \
			check(IsValid(ThisSubsystem)); \
			\
			auto& ThisSubsystemRef = *ThisSubsystem; \
			return ThisSubsystemRef; \
		} \
		\
		static ThisClass& Get(const ULocalPlayerSubsystem* LocalPlayerSubsystem) \
		{ \
			check(IsValid(LocalPlayerSubsystem)); \
			\
			const auto* LocalPlayer = LocalPlayerSubsystem->GetLocalPlayer<ULocalPlayer>(); \
			auto& ThisSubsystemRef = Get(LocalPlayer); \
			return ThisSubsystemRef; \
		}

/**
 * Local player subsystem with commonly used functions.
 */
UCLASS(Abstract)
class COMMONSUBSYSTEMS_API UCommonLocalPlayerSubsystem
	: public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	// COMMON_SUBSYSTEMS_LOCAL_PLAYER_BODY()
	// ^^^ Include this in your override of the subsystem ^^^

public:
	/**
	 * Get index associated with local player the subsystem is created on.
	 * @return	Local player index.
	 */
	int32 GetLocalPlayerIndex() const;
};
