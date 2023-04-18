// Copyright CloudHu. All Rights Reserved.
#include "Definition/ItemDefinition.h"

UItemDefinition::UItemDefinition(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
	,DisplayName(FText::GetEmpty())
	,Introduction(FText::GetEmpty())
{

}

const TMap<FGameplayTag, int32> UItemDefinition::GetStats_Implementation() const
{
	return InitialItemStats;
}

const float UItemDefinition::GetGravity_Implementation() const
{
	return ItemGravity;
}

const TArray<int32> UItemDefinition::GetChildrenIds_Implementation() const
{
	return Children;
}

const FText UItemDefinition::GetDisplayName_Implementation() const
{
	return DisplayName;
}

const FSlateBrush UItemDefinition::GetIconBrush_Implementation() const
{
	return IconBrush;
}

const FText UItemDefinition::GetIntroduction_Implementation() const
{
	return Introduction;
}

const int32 UItemDefinition::GetMaxAmount_Implementation() const
{
	return MaxCount;
}

const int32 UItemDefinition::GetOverlyingAmount_Implementation() const
{
	return OverlyingCount;
}

const FGameplayTagContainer UItemDefinition::GetTypeTags_Implementation() const
{
	return TypeTags;
}

