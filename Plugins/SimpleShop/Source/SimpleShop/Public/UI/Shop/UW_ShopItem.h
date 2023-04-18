//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UW_ItemSlotBase.h"
#include "UW_ShopItem.generated.h"

/**
 * 商品
 */
UCLASS(Blueprintable,BlueprintType)
class SIMPLESHOP_API UUW_ShopItem : public UUW_ItemSlotBase
{
	GENERATED_BODY()

private:
	//监听钱包交易
	FCH_GameplayMessageListenerHandle WalletListenerHandle;

	/**
	 * @brief 库存数量
	 */
	int32 StockAmount;
public:
	/// <summary>
	/// 商品被鼠标左键点击时是购买
	/// </summary>
	virtual void OnClickedWidget() override;

	virtual void UpdateSlot(const FItemTable* InTable) override;

	virtual  void TransactionResult(const bool bSuccess,const int32 InAmount) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//交易消息通知
	void OnNotificationWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification);
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

};
