# Common Subsystems

Collection of engine's subsystem with common utility functions.

## Installation

Clone the repository in your project's Plugin directory.

## Usage

The plugin comes with World, Game Instance, Local Player and Engine subsystems.

All of them have shorthands to access them in a cleaner and unified way

To inherit all these shorthanded expressions, you have to add the related macro below the GENERATED_BODY() like this:

```cpp
class UMyWorldSubsystem
	: public UCommonWorldSubsystem
{
	GENERATED_BODY()
	COMMON_SUBSYSTEMS_WORLD_BODY() // <<< This; note that other subsystems will substitute "WORLD" in the name to whatever they represent

public:
	UMyWorldSubsystem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
```

Getting a common subsystem:

```cpp
// Original
auto* Subsystem = GetWorld()->GetSubsystem<UMyWorldSubsystem>();
if (IsValid(Subsystem))
{
	Subsystem->Foo();
}

auto* Subsystem = GetGameInstance()->GetSubsystem<UMyGameInstanceSubsystem>();
if (IsValid(Subsystem))
{
	Subsystem->Bar();
}

// Plugin's
if (UMyWorldSubsystem::HasInstance(this))
{
	auto& Subsystem = UMyWorldSubsystem::Get(this);
	Subsystem.Foo();
}

if (UMyGameInstanceSubsystem::HasInstance(this))
{
	auto& Subsystem = UMyGameInstanceSubsystem::Get(this);
	Subsystem.Foo();
}
```

All the subsystems have an integrated Get() which accesses the subsystem in the appropriate way depending on its nature.

Furthermore, the UCommonWorldSubsystem has multiplayer support and map allow/block lists:

```cpp
UMyWorldSubsystem::UMyWorldSubsystem()
{
	// Don't create on main menu
	LevelBlocklist.Add("L_MainMenu");

	// Client-side only
	ClearSupportedNetMode();
	AddSupportedNetMode(ECommonNetMode::Client);
}
```

## Credits

- [Jambax's World Subsystem](https://github.com/TheJamsh/UnrealSnippets/tree/main/Code/World%20Subsystem)