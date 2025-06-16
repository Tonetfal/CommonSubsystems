// Author: Antonio Sidenko (Tonetfal), July 2023

#include "Subsystems/CommonWorldSubsystem.h"

#include "Engine/NetDriver.h"
#include "GameMapsSettings.h"
#include "LogCategories.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#endif

DEFINE_LOG_CATEGORY(LogCommonSubsystems);

static uint8 GetNetModeInteger(ECommonNetMode NetMode)
{
	const uint8 NetModeInteger = static_cast<uint8>(1) << static_cast<uint8>(NetMode);
	return NetModeInteger;
}

UCommonWorldSubsystem::UCommonWorldSubsystem()
{
	// Skip 'Entry' and 'MainMenu' levels by default
	LevelBlocklist.Add("L_Entry");
	LevelBlocklist.Add("L_MainMenu");

	bEnableInUntitledLevel = false;
	bEnableInTransitionLevel = false;

	// Initialise in all network domains by default
	ClearSupportedNetMode();
	AddSupportedNetMode(ECommonNetMode::Standalone);
	AddSupportedNetMode(ECommonNetMode::DedicatedServer);
	AddSupportedNetMode(ECommonNetMode::ListenServer);
	AddSupportedNetMode(ECommonNetMode::Client);
}

void UCommonWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Tick_Initialize(FTickSignature::CreateUObject(this, &ThisClass::Tick));

	// We can't do safe initialization until much later
	PostInitWorldDelegateHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
		this, &ThisClass::PostInitWorldInternal);

#if WITH_EDITOR
	// Editor environment is added
	PostInitPieWorldDelegateHandle = FEditorDelegates::PostPIEStarted.AddUObject(
		this, &ThisClass::PostInitPieWorldInternal);
#endif
}

void UCommonWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	Tick_Deinitialize();
}

bool UCommonWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	const auto* World = CastChecked<UWorld>(Outer);
	if (!World->IsGameWorld())
	{
		return false;
	}

#if WITH_EDITOR
	// PIE creates temporary worlds for Net Clients which should really never initialize subsystems
	if (World->IsPlayInEditor())
	{
		const UPackage* Package = CastChecked<UPackage>(World->GetOuter());
		const int32 PieInstanceId = Package ? Package->GetPIEInstanceID() : INDEX_NONE;
		if (PieInstanceId == INDEX_NONE)
		{
			return false;
		}
	}
#endif

	if (!CheckNetMode(World))
	{
		ENetMode MyNetMode = NM_MAX;
		GetSafeNetMode(OUT MyNetMode, World);

		UE_LOG(LogCommonSubsystems, Log, TEXT("Subsystem [%s] has not been created because net mode [%s] is not "
			"supported."), *GetName(), *UEnum::GetValueAsString(static_cast<ECommonNetMode>(MyNetMode)));

		return false;
	}

	if (!CheckLevelName(World))
	{
		UE_LOG(LogCommonSubsystems, Log, TEXT("Subsystem [%s] has not been created because level [%s] is not "
			"supported."), *GetName(), *World->GetMapName());

		return false;
	}

	return true;
}

bool UCommonWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return Super::DoesSupportWorldType(WorldType);
}

void UCommonWorldSubsystem::Tick(float DeltaSeconds)
{
	// Empty
}

bool UCommonWorldSubsystem::IsNetModeSupported(ECommonNetMode NetMode) const
{
	const bool bIsNetModeSupported = InitializationNetModeMask | GetNetModeInteger(NetMode);
	return bIsNetModeSupported;
}

bool UCommonWorldSubsystem::GetSafeNetMode(ENetMode& OutNetMode, const UWorld* OverrideWorld /*nullptr*/) const
{
	const UWorld* World = OverrideWorld ? OverrideWorld : GetWorld();
	check(World);

	OutNetMode = World->GetNetMode();

#if WITH_EDITOR
	// PIE creates some obscure UWorld lifetimes that make NetMode access more difficult
	// See https://udn.unrealengine.com/s/question/0D54z00007DW1CQCA1/why-are-uworldsubsystems-initialized-before-uworld-is-initialized
	if (!World->IsPlayInEditor())
	{
		return true;
	}

	UObject* Outer = World->GetOuter();
	const auto* Package = Cast<UPackage>(Outer);
	const int32 PieInstanceId = Package ? Package->GetPIEInstanceID() : INDEX_NONE;
	if (PieInstanceId == INDEX_NONE)
	{
		// This is a Transient UPackage used for PIE Client worlds
		OutNetMode = NM_MAX;
		return false;
	}

	// We can get the UGameInstance/UNetDriver from here, *before* the UWorld properties are set
	const auto* EditorEngine = CastChecked<UEditorEngine>(GEngine);
	const FWorldContext* PieWorldContext = EditorEngine->GetWorldContextFromPIEInstance(PieInstanceId);
	if (ensureAlways(PieWorldContext))
	{
		if (!PieWorldContext->PendingNetGame)
		{
			OutNetMode = PieWorldContext->RunAsDedicated ? NM_DedicatedServer : NM_Standalone;
		}
		else if (ensureAlways(PieWorldContext->PendingNetGame->NetDriver))
		{
			OutNetMode = PieWorldContext->PendingNetGame->NetDriver->GetNetMode();
		}
	}
#endif

	return true;
}

FTimerManager& UCommonWorldSubsystem::GetTimerManager() const
{
	return GetWorld()->GetTimerManager();
}

void UCommonWorldSubsystem::OnWorldInitialized()
{
	// Empty
}

void UCommonWorldSubsystem::AddSupportedNetMode(ECommonNetMode NetMode)
{
	InitializationNetModeMask |= GetNetModeInteger(NetMode);
}

void UCommonWorldSubsystem::RemoveSupportedNetMode(ECommonNetMode NetMode)
{
	InitializationNetModeMask ^= GetNetModeInteger(NetMode);
}

void UCommonWorldSubsystem::ClearSupportedNetMode()
{
	InitializationNetModeMask = 0;
}

bool UCommonWorldSubsystem::CheckNetMode(const UWorld* World) const
{
	ENetMode MyNetMode = NM_MAX;
	if (GetSafeNetMode(OUT MyNetMode, World))
	{
		const uint8 NetModeMask = GetNetModeInteger(static_cast<ECommonNetMode>(MyNetMode));
		const uint8 MatchingModes = InitializationNetModeMask & NetModeMask;
		const bool bMatchesAnything = MatchingModes != 0;
		return bMatchesAnything;
	}

	return false;
}

bool UCommonWorldSubsystem::CheckLevelName(const UWorld* World) const
{
	checkSlow(World);

	// Check Level Names
	FString LevelName = World->GetMapName();
	LevelName.RemoveFromStart(World->StreamingLevelsPrefix);

	// Optionally Block in the 'Untitled' Level
	// These are special levels the engine sometimes creates for intermediate UWorlds.
	if (!bEnableInUntitledLevel && LevelName.Contains(TEXT("Untitled")))
	{
		return false;
	}

	// Optionally Block in the 'Transition' Level
	// Read the Transition Level from project settings.
	const auto* MapSettings = GetDefault<UGameMapsSettings>();
	check(MapSettings);

	if (!bEnableInTransitionLevel && !MapSettings->TransitionMap.IsNull() &&
		MapSettings->TransitionMap.GetAssetName() == LevelName)
	{
		return false;
	}

	if (!LevelAllowlist.IsEmpty() && !LevelAllowlist.Contains(LevelName))
	{
		return false;
	}

	if (LevelBlocklist.Contains(LevelName))
	{
		return false;
	}

	return true;
}

void UCommonWorldSubsystem::PostInitWorldInternal(UWorld* NewWorld)
{
	const UWorld* World = GetWorld();
	if (IsValid(World) && World == NewWorld && !bHasPostWorldInitialized)
	{
		bHasPostWorldInitialized = true;

		FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostInitWorldDelegateHandle);
#if WITH_EDITOR
		FEditorDelegates::PostPIEStarted.Remove(PostInitPieWorldDelegateHandle);
#endif

		OnWorldInitialized();
	}
}

#if WITH_EDITOR
void UCommonWorldSubsystem::PostInitPieWorldInternal(bool bSimulating)
{
	PostInitWorldInternal(GetWorld());
}
#endif
