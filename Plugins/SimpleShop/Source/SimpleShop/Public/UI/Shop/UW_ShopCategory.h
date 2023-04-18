//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/UW_CategoryBase.h"
#include "UW_ShopCategory.generated.h"

/**
 * 商店目录
 */
UCLASS(Blueprintable,BlueprintType)
class SIMPLESHOP_API UUW_ShopCategory : public UUW_CategoryBase
{
	GENERATED_BODY()
public:
	virtual void OnWidgetClicked() override;
	
	/** 绑定词缀标签. */
	virtual void BindCategory(const FGameplayTag InTag) override;

	virtual void OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification) override;

	virtual void ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification) override;
};
