//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UW_CategoryBase.h"
#include "Message/UserInterfaceMessage.h"
#include "UW_CategoryButton.generated.h"

class UTextBlock;
/**
 * 目录按钮
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_CategoryButton : public UUW_CategoryBase
{
	GENERATED_BODY()

public:
	UUW_CategoryButton(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnWidgetClicked() override;

	/** 绑定词缀标签. */
	virtual void BindCategory(const FGameplayTag InTag) override;

	virtual void OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification) override;

	virtual void ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification) override;
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
};
