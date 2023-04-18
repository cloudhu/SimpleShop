// Copyright CloudHu. All Rights Reserved.
#include "Definition/ItemCategory.h"

UItemCategory::UItemCategory(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

void UItemCategory::SetTag(const FGameplayTag InTag)
{
	Chanel = InTag;
}

void UItemCategory::SetAmount(int32 InAmount)
{
	Delta = InAmount;
}
