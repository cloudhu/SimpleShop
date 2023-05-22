// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ItemTable.h"
#include "GameplayTagContainer.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "ActorComponents/WalletActorComponent.h"
#include "Data/ShopTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/TransactionMessage.h"
#include "ShopSubsystem.generated.h"

/**
 * 商店子系统
 */
UCLASS()
class SIMPLESHOP_API UShopSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/**
	 * @brief 构造函数
	 */
	UShopSubsystem();
public:
	/**
	 * @brief 获取
	 * @param WorldContextObject 世界上下文对象
	 * @return 商店子系统静态实例
	 */
	static UShopSubsystem& Get(const UObject* WorldContextObject);

	/**
	 * @brief 初始化
	 * @param Collection 集合 
	 */
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

	/**
	 * @brief 析化
	 */
	virtual void Deinitialize() override;

	/**
	 * @brief 交易消息通知
	 * @param Channel 消息频道标签
	 * @param Notification 通知的交易消息 
	 */
	void OnNotificationTransactionMessage(FGameplayTag Channel, const FConfirmedTransactionMessage& Notification);

	/**
	 * @brief 计算购买物品所需消耗
	 * @param InTable 物品数据
	 * @param InPawn 使用的角色
	 * @param bIsServer 是否是服务器调用
	 * @return 消耗数目
	 */
	int32 CalculateCompoundItemCost(const FItemTable* InTable, const APawn* InPawn, bool bIsServer = false);

	/**
	 * @brief 递归获取合成商品所需的物品编号
	 * @param InTable 物品配置表
	 * @param ItemIds 物品编号
	 */
	void RecursivelyGetItemID(const FItemTable* InTable, TArray<int32>& ItemIds);

	/**
	 * @brief 获取
	 * @return 最大背包等级
	 */
	int32 GetMaxInventoryLevel() const { return InventoryTables.Num(); }

	/**
	 * @brief 获取
	 * @return 最大快捷栏等级
	 */
	int32 GetMaxQuickBarLevel() const { return QuickBarConfigurations.Num(); }
public:
	/**
	 * @brief 获取钱包组件
	 * @param Actor 含有该组件的角色
	 * @return 钱包组件
	 */
	UFUNCTION(BlueprintPure, Category = "Shop Subsystem")
	static UWalletActorComponent* FindWalletActorComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UWalletActorComponent>() : nullptr); }

	/**
	 * @brief 获取
	 * @param Actor 拥有背包管理组件的角色
	 * @return 背包管理组件
	 */
	UFUNCTION(BlueprintPure, Category = "Shop Subsystem")
	static UInventoryManagerActorComponent* FindInventoryManagerActorComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UInventoryManagerActorComponent>() : nullptr);
	}

protected:
	/**
	 * @brief 缓存配置表指针
	 */
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
	/**
	 * @brief 缓存数据
	 */
	TArray<FItemTable*> CacheSlotTables;

	/**
	 * @brief 背包数据
	 */
	TArray<FInventoryConfiguration*> InventoryTables;

	/**
	 * @brief 快捷栏配置数据
	 */
	TArray<FQuickBarConfiguration*> QuickBarConfigurations;

	/**
	 * @brief 交易监听句柄
	 */
	FGameplayMessageListenerHandle TransactionListenerHandle;
};
