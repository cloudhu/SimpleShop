//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_ItemSlotBase.h"
#include "UW_CompoundItem.generated.h"

/**
 * 合成物品
 */
UCLASS(BlueprintType,Blueprintable)
class SIMPLESHOP_API UUW_CompoundItem : public UUW_ItemSlotBase
{
	GENERATED_BODY()

public:
	UUW_CompoundItem(const FObjectInitializer& ObjectInitializer);

	FGameplayMessageListenerHandle WalletListenerHandle2;
public:
	/// <summary>
	/// 商品被鼠标左键点击时是购买
	/// </summary>
	virtual void OnClickedWidget() override;

	//交易消息通知
	void OnNotificationWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification);

	int32 GetCompoundPrice();
protected:
	virtual void NativeConstruct() override;


	virtual void NativeDestruct() override;

};
