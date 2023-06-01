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

	/**
	 * @brief 钱包交易消息监听句柄
	 */
	FGameplayMessageListenerHandle WalletListenerHandle2;
public:
	/**
	 * @brief 商品被鼠标左键点击时是购买
	 */
	virtual void OnClickedWidget() override;

	/**
	 * @brief 收到交易消息通知时
	 * @param Channel 频道
	 * @param Notification 通知消息 
	 */
	void OnNotificationWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification);

	/**
	 * @brief 获取合成费用
	 * @return 价格
	 */
	int32 GetCompoundPrice();
protected:
	/**
	 * @brief UI原生构造
	 */
	virtual void NativeConstruct() override;

	/**
	 * @brief UI原生析构
	 */
	virtual void NativeDestruct() override;

};
