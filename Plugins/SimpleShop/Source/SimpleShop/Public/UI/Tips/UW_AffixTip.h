//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UW_AffixTip.generated.h"

struct FAffixChangeMessage;
class UTextBlock;
/**
 * 词缀提示
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_AffixTip : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UUW_AffixTip(const FObjectInitializer& ObjectInitializer);

	//显示名称
	UPROPERTY(BlueprintReadOnly, Category = AffixTip, meta = (BindWidget))
	UTextBlock* DisplayName;

	//数值
	UPROPERTY(BlueprintReadOnly, Category = AffixTip, meta = (BindWidget))
	UTextBlock* NumText;

	UPROPERTY(BlueprintReadOnly, Category=AffixTip)
	FGuid Guid;
public:
	/** 绑定词缀标签. */
	void BindAffixTip(FGameplayTag Tag);

	void SetNumText(int32 InNum);

	void SetDisplayNameText(const FText& InText) const;

	//消息通知
	void OnAffixChangeMessage(FGameplayTag Channel, const FAffixChangeMessage& Notification);
protected:
	virtual void NativeDestruct() override;


	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	//监听句柄
	FCH_GameplayMessageListenerHandle ListenerHandle;
};
