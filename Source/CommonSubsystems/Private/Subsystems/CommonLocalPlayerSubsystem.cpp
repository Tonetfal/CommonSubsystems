// Author: Antonio Sidenko (Tonetfal), July 2023

#include "Subsystems/CommonLocalPlayerSubsystem.h"

int32 UCommonLocalPlayerSubsystem::GetLocalPlayerIndex() const
{
	const auto* LocalPlayer = GetLocalPlayer<ULocalPlayer>();
	check(IsValid(LocalPlayer));

	const int32 Index = LocalPlayer->GetLocalPlayerIndex();
	return Index;
}
