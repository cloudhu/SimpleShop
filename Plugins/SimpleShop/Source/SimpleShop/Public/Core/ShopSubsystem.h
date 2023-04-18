// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ItemTable.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "ActorComponents/WalletActorComponent.h"
#include "Data/ShopTypes.h"
#include "Message/TransactionMessage.h"
#include "ShopSubsystem.generated.h"

/**
 * 商店子系统
 */
UCLASS()
class SIMPLESHOP_API UShopSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UShopSubsystem();
public:
	static UShopSubsystem& Get(const UObject* WorldContextObject);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief 获取物品数据
	 * @return 物品配置数据
	 */
	const TArray<FItemTable*>* GetSlotTablesTemplate();

	/**
	 * @brief 获取背包配置数据
	 * @return 背包配置数据
	 */
	const TArray<FInventoryConfiguration*>* GetInventoryConfigurations();

	/**
	 * @brief 读取快捷栏配置表
	 * @return 快捷栏配置
	 */
	const TArray<FQuickBarConfiguration*>* GetQuickBarConfigurations();

	/**
	 * @brief 根据物品编号获取数据
	 * @param InID 物品编号
	 * @return 物品数据
	 */
	const FItemTable* GetSlotTableByID(const int32 InID);

	/**
	 * @brief 获取指定背包等级的配置数据
	 * @param InLevel 等级
	 * @return 数据
	 */
	const FInventoryConfiguration* GetInventoryConfigurationByLevel(const int32 InLevel);

	/**
	 * @brief 读取指定等级的快捷栏配置数据
	 * @param InLevel 等级
	 * @return 数据
	 */
	const FQuickBarConfiguration* GetQuickBarConfigurationByLevel(const int32 InLevel);

	virtual void Deinitialize() override;

	/** 交易消息通知. */
	void OnNotificationTransactionMessage(FGameplayTag Channel, const FConfirmedTransactionMessage& Notification);

	/// <summary>
	/// 计算购买物品所需消耗
	/// </summary>
	/// <param name="InTable">物品数据</param>
	/// <param name="bIsServer">是否是服务器调用</param>
	/// <returns>消耗数目</returns>
	int32 CalculateCompoundItemCost(const FItemTable* InTable, const APawn* InPawn, bool bIsServer = false);

	//递归获取合成商品所需的物品编号
	void RecursivelyGetItemID(const FItemTable* InTable, TArray<int32>& ItemIds);

	int32 GetMaxInventoryLevel() const { return InventoryTables.Num(); }
	int32 GetMaxQuickBarLevel() const { return QuickBarConfigurations.Num(); }
public:
	UFUNCTION(BlueprintPure, Category = "Shop Subsystem")
	static UWalletActorComponent* FindWalletActorComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UWalletActorComponent>() : nullptr); }

	UFUNCTION(BlueprintPure, Category = "Shop Subsystem")
	static UInventoryManagerActorComponent* FindInventoryManagerActorComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UInventoryManagerActorComponent>() : nullptr);
	}

protected:
	//缓存配置表指针
	UPROPERTY()
	UDataTable* SlotTablePtr;

	/**
	 * @brief 背包配置数据缓存
	 */
	UPROPERTY()
	UDataTable* InventoryTablePtr;

	/**
	 * @brief 快捷栏配置数据缓存
	 */
	UPROPERTY()
	UDataTable* QuickBarTablePtr;
private:
	//缓存数据
	TArray<FItemTable*> CacheSlotTables;

	/**
	 * @brief 背包数据
	 */
	TArray<FInventoryConfiguration*> InventoryTables;

	/**
	 * @brief 快捷栏配置数据
	 */
	TArray<FQuickBarConfiguration*> QuickBarConfigurations;

	//交易监听句柄
	FCH_GameplayMessageListenerHandle TransactionListenerHandle;
};
