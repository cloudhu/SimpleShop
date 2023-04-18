// Copyright CloudHu. All Rights Reserved.
#include "Definition/TipItem.h"

UTipItem::UTipItem(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

void UTipItem::SetTag(FGameplayTag InTag)
{
	Chanel = InTag;
}

void UTipItem::SetAmount(int32 InAmount)
{
	Delta = InAmount;
}
