//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TileView.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/GlobalNativeTags.h"
#include "Message/TransactionMessage.h"
#include "Message/UserInterfaceMessage.h"
#include "UW_InventoryPanel.generated.h"

class UButton;
class UTextBlock;
/**
 * 背包面板
 */
UCLASS(Blueprintable, BlueprintType)
class SIMPLESHOP_API UUW_InventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_InventoryPanel(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief 物品列表
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UTileView* TileView_ItemList;

	/**
	 * @brief 目录列表
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UListView* ListCategory;

	/**
	 * @brief 关闭按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UButton* Button_Close;

	/**
	 * @brief 扩展按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UButton* ButtonExpand;
	
	/**
	 * @brief 金币文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UTextBlock* Text_Gold;

	/**
	 * @brief 负重文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UTextBlock* Text_Gravity;

	/**
	 * @brief 最大负重文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = InventoryPanel, meta = (BindWidget))
	UTextBlock* Text_MaxGravity;
public:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	 /**
	 * @brief 目录点击监听消息处理
	 * @param Channel 频道
	 * @param Notification 通知数据
	 */
	void OnCategoryClickMessage(FGameplayTag Channel, const FCategoryClickMessage& Notification);

	/**
	 * @brief 目录更新监听消息处理
	 * @param Channel 频道
	 * @param Notification 通知数据
	 */
	void OnCategoryChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification);

	/**
	 * @brief 钱包监听消息处理
	 * @param Channel 频道
	 * @param Notification 通知数据
	 */
	void OnWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification);

	/**
	 * @brief 背包负重变化
	 * @param Channel 频道
	 * @param Notification 数据
	 */
	void OnInventoryGravityMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification);

	/**
	 * @brief 背包最大负重变化
	 * @param Channel 频道
	 * @param Notification 数据
	 */
	void OnInventoryMaxGravityMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification);

	/**
	 * @brief 背包扩充
	 * @param Channel 频道
	 * @param Notification 数据
	 */
	void OnInventoryExpandMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification);

	/**
	* @brief 背包物品更新
	 */
	//void OnItemStackChanged(FGameplayTag Channel,const FInventoryChangeMessage& Notification);
	
	/**
	 * @brief 设置金币
	 * @param InNum 数量
	 */
	void SetGoldText(const int32 InNum) const;

	/**
	 * @brief 设置文本数量
	 * @param TextSrc 文本
	 * @param InNum 数量
	 */
	void SetTextNum(UTextBlock* TextSrc, const float InNum) const;


public:
	/**
	 * @brief 在物品被点击的时候调用
	 */
	UFUNCTION(BlueprintCallable, Category=InventoryPanel, BlueprintPure = false)
	void UpdateItemListByTag(const FGameplayTag& InTag) const;

	/**
	 * @brief 关闭背包
	 */
	UFUNCTION(BlueprintCallable,Category=InventoryPanel, BlueprintPure = false)
	void OnCloseInventory();
	
	/**
	 * @brief 扩展背包
	 */
	UFUNCTION(BlueprintCallable,Category=InventoryPanel)
	void ExpandInventory();
	
	UFUNCTION(BlueprintCallable,Category=InventoryPanel, BlueprintPure = false)
	void OnItemClicked(UObject* Item);
private:
	/**
	 * @brief 目录点击消息监听句柄
	 */
	FGameplayMessageListenerHandle CategoryClickedListenerHandle;

	/**
	 * @brief 目录更新消息监听句柄
	 */
	FGameplayMessageListenerHandle CategoryUpdateListenerHandle;
	
	/**
	 * @brief 钱包监听句柄
	 */
	FGameplayMessageListenerHandle WalletListenerHandle;
	
	/**
	 * @brief 当前背包负重监听
	 */
	FGameplayMessageListenerHandle GravityListenerHandle;

	/**
	 * @brief 当前背包负重监听
	 */
	FGameplayMessageListenerHandle MaxGravityListenerHandle;

	/**
	 * @brief 当前背包扩容监听
	 */
	FGameplayMessageListenerHandle ExpandListenerHandle;

	/**
	 * @brief 当前背包扩容监听
	 */
	//FGameplayMessageListenerHandle ItemChangeListenerHandle;

	/**
	 * @brief 缓存标签
	 */
	FGameplayTag CacheTag=TAG_Item_Type_All;
};