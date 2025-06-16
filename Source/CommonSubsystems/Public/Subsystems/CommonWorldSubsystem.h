// Author: Antonio Sidenko (Tonetfal), July 2023

#pragma once

#include "Subsystems/Components/CommonTickComponent.h"
#include "Subsystems/WorldSubsystem.h"

#include "CommonWorldSubsystem.generated.h"

#define COMMON_SUBSYSTEMS_WORLD_BODY() \
	public: \
		static bool HasInstance(const UObject* ContextObject) \
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
		}

/**
 * @see		ENetMode
 */
UENUM()
enum class ECommonNetMode : uint8
{
	/** Standalone: a game without networking, with one or more local players. Still considered a server because it has all server functionality. */
	Standalone,

	/** Dedicated server: server with no local players. */
	DedicatedServer,

	/** Listen server: a server that also has a local player who is hosting the game, available to other players on the network. */
	ListenServer,

	/**
	 * Network client: client connected to a remote server.
	 * Note that every mode less than this value is a kind of server, so checking NetMode < NM_Client is always some variety of server.
	 */
	Client,

	MAX,
};

/**
 * World subsystem with commonly used functions.
 */
UCLASS(Abstract)
class COMMONSUBSYSTEMS_API UCommonWorldSubsystem
	: public UWorldSubsystem
	, public FCommonTickComponent
{
	GENERATED_BODY()
	// COMMON_SUBSYSTEMS_WORLD_BODY()
	// ^^^ Include this in your override of the subsystem ^^^

public:
	UCommonWorldSubsystem();

	//~UWorldSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	//~End of UWorldSubsystem Interface

protected:
	/**
	 * Called each tick interval.
	 * @param	DeltaSeconds time in seconds since last tick.
	 */
	virtual void Tick(float DeltaSeconds);

public:
	/**
	 * Check whether a given net mode is supported.
	 * @param	NetMode net mode to check.
	 * @return	If true, net mode is supported, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsNetModeSupported(ECommonNetMode NetMode) const;

	/**
	 * Get net mode.
	 * @param	OutNetMode output parameter. The net mode.
	 * @param	OverrideWorld world to use. If nullptr, the one associated with this subsystem will be used instead.
	 * @return	If true, net mode has been assigned, false otherwise.
	 */
	bool GetSafeNetMode(ENetMode& OutNetMode, const UWorld* OverrideWorld = nullptr) const;

	/**
	 * Get timer manager instance for this world.
	 * @return Timer manager for this world.
	 */
	FTimerManager& GetTimerManager() const;

protected:
	/**
	 * Called on world initialization
	 */
	virtual void OnWorldInitialized();

	/**
	 * Add given net mode in supported list.
	 * @param	NetMode net mode to add.
	 */
	void AddSupportedNetMode(ECommonNetMode NetMode);

	/**
	 * Remove given net mode from supported list.
	 * @param	NetMode net mode to remove.
	 */
	void RemoveSupportedNetMode(ECommonNetMode NetMode);

	/**
	 * Clear supported net modes.
	 */
	void ClearSupportedNetMode();

	/**
	 * Check whether this subsystem has to be create with the current net mode.
	 * @param	World world to retrieve information from.
	 * @return	If true, we do want to create this subsystem with the current net mode, false otherwise.
	 */
	bool CheckNetMode(const UWorld* World) const;

	/**
	 * Check whether this subsystem has to be created with the current level.
	 * @param	World world to retrieve information from.
	 * @return	If true, we do want to create this subsystem with the current level, false otherwise.
	 */
	bool CheckLevelName(const UWorld* World) const;

private:
	/**
	 * Called on post world initialization.
	 * @param	NewWorld initialized world.
	 */
	void PostInitWorldInternal(UWorld* NewWorld);

#if WITH_EDITOR
	/**
	 * Called on post PIE world initialization.
	 * @param	bSimulating true if world is simulating, false otherwise.
	 */
	void PostInitPieWorldInternal(bool bSimulating);
#endif

protected:
	/** If non-empty, the subsystem will only be initialized if the level name is in this list. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Initialization")
	TArray<FString> LevelAllowlist;

	/** If non-empty, the subsystem will *NOT* be initialized if the level name is in this list. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Initialization")
	TArray<FString> LevelBlocklist;

private:
	/** Don't initialize the subsystem unless the world matches the given Net Modes */
	UPROPERTY(EditDefaultsOnly, Category="Initialization", meta=(Bitmask, BitmaskEnum="ECommonNetMode"))
	uint8 InitializationNetModeMask = 0;

protected:
	/**
	 * If true, enables in an "Untitled" level. These are usually special intermediate/transient levels created by the
	 * engine.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Initialization", AdvancedDisplay)
	bool bEnableInUntitledLevel = false;

	/** If true, enables in the Transition Level */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Initialization", AdvancedDisplay)
	bool bEnableInTransitionLevel = false;

private:
	/** Delegate associated with UCommonWorldSubsystem::PostInitWorldInternal(). */
	FDelegateHandle PostInitWorldDelegateHandle;

	/** If true, we've already called UCommonWorldSubsystem::PostInitWorldInternal(), false otherwise. */
	bool bHasPostWorldInitialized = false;

#if WITH_EDITOR
	/** Delegate associated with UCommonWorldSubsystem::PostInitPieWorldInternal(). */
	FDelegateHandle PostInitPieWorldDelegateHandle;
#endif
};
