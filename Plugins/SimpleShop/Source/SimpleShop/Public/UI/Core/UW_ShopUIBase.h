//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponents/ShopActorComponent.h"
#include "UW_ShopUIBase.generated.h"

/**
 * 商店UI基类
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ShopUIBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUW_ShopUIBase(const FObjectInitializer& ObjectInitializer);

	//获取商店在角色上的组件
	UShopActorComponent* GetShopAc() const;
};
