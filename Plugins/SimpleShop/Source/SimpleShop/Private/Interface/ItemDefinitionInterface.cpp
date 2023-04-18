// Copyright CloudHu. All Rights Reserved.
#include "Interface/ItemDefinitionInterface.h"
// Add default functionality here for any IItemDefinitionInterface functions that are not pure virtual.
FGameplayTagContainer IItemDefinitionInterface::GetTypeTags(const UObject* Src)
{
	return Execute_GetTypeTags(Src);
}

FText IItemDefinitionInterface::GetDisplayName(const UObject* Src)
{
	return Execute_GetDisplayName(Src);
}

FText IItemDefinitionInterface::GetIntroduction(const UObject* Src)
{
	return Execute_GetIntroduction(Src);
}

FSlateBrush IItemDefinitionInterface::GetIconBrush(const UObject* Src)
{
	return Execute_GetIconBrush(Src);
}

TArray<int32> IItemDefinitionInterface::GetChildrenIds(const UObject* Src)
{
	return Execute_GetChildrenIds(Src);
}

int32 IItemDefinitionInterface::GetMaxAmount(const UObject* Src)
{
	return Execute_GetMaxAmount(Src);
}

int32 IItemDefinitionInterface::GetOverlyingAmount(const UObject* Src)
{
	return Execute_GetOverlyingAmount(Src);
}

TMap<FGameplayTag, int32> IItemDefinitionInterface::GetStats(const UObject* Src)
{
	return Execute_GetStats(Src);
}

float IItemDefinitionInterface::GetGravity(const UObject* Src)
{
	return Execute_GetGravity(Src);
}