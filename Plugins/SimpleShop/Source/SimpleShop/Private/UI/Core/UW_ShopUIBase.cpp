//  Copyright CloudHu. All Rights Reserved.
#include "UI/Core/UW_ShopUIBase.h"

UUW_ShopUIBase::UUW_ShopUIBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

UShopActorComponent* UUW_ShopUIBase::GetShopAc() const
{
	return UShopActorComponent::FindShopActorComponent(GetOwningPlayerPawn());
}
