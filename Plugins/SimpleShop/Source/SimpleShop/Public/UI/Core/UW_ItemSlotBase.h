// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Message/TransactionMessage.h"
#include "UW_ItemSlotBase.generated.h"

class UItemInstance;
class UTextBlock;
class UImage;
class UButton;
struct FItemTable;
class UUW_ItemTips;
struct FTransactionMessage;

/**
 * 物品UI基类
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ItemSlotBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	//缓存数据
private:
	/**
	 * @brief 实例唯一编号
	 */
	FGuid InstanceGuid;

	//物品编号
	int32 ItemID;

	//实例索引
	int32 InstanceIndex;

	//价格
	int32 Price;

	/**
	 * @brief 数量
	 */
	int32 Amount;

	//物品的拥有者
	UPROPERTY()
	APawn* ItemOwner;

	//提示UI
	UPROPERTY()
	UUW_ItemTips* TipPtr;

	//数据缓存
	const FItemTable* DataTable;

	FCH_GameplayMessageListenerHandle ListenerHandle;

public:
	UPROPERTY(BlueprintReadOnly, Category = Item, meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(BlueprintReadOnly, Category = Item, meta = (BindWidget))
	UButton* ItemButton;

	/** 数量 */
	UPROPERTY(BlueprintReadOnly, Category = CategoryButton, meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(EditDefaultsOnly, Category = Item)
	TSubclassOf<UUW_ItemTips> TipClass;
public:
	UUW_ItemSlotBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	/**
	 * @brief 判断是哪里的物品
	 * @return true:是快捷栏;false:背包
	 */
	virtual bool IsQuickBarItem() const{ return false; }
public: //Getters
	UWidget* GetTip();

	APawn* GetItemOwner() const { return ItemOwner; }

	int32 GetItemID() const { return ItemID; }

	int32 GetInstanceIndex() const { return InstanceIndex; }

	int32 GetPrice() const { return Price; }

	int32 GetAmount() const { return Amount; }

	const FItemTable* GetDataTable() const { return DataTable; }

	const FGuid& GetGuid() const { return InstanceGuid; }
public: //Setters
	void SetItemID(int32 InID);

	void SetItemPrice(int32 InPrice);

	void SetIconBrush(const FSlateBrush& Brush) const;

	void SetItemOwner(APawn* InOwner);

	void SetInstanceGuid(const FGuid& InGuid);

	void SetNumText(const int32 InNum);
public:
	virtual void UpdateSlot(const FItemTable* InTable);

	/**
	 * @brief 处理交易结果
	 * @param bSuccess 交易结果
	 * @param InAmount 交易数量
	 */
	virtual void TransactionResult(const bool bSuccess,const int32 InAmount);

	//交易消息通知
	void OnNotificationTransactionResultMessage(FGameplayTag Channel, const FTransactionMessageResult& Notification);

	//是否激活物品
	void SetItemIsEnabled(const bool bEnabled) const;

	/// <summary>
	/// 显示图标
	/// </summary>
	/// <param name="bVisible">是否显示</param>
	void ShowIcon(const bool bVisible) const;

	/// <summary>
	/// 出售物品
	/// </summary>
	void SellItem() const;

	void UpdateItemByInstance(const UItemInstance* Instance);

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
	void SetInstanceIndex(int32 InInstanceID);

	UFUNCTION(BlueprintCallable, Category = ItemSlot)
	virtual void OnClickedWidget();
};
