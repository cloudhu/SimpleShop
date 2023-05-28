//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemGameplayTagStack.h"
#include "GameplayTagContainer.h"
#include "Core/ActorComponentBase.h"
#include "Data/InventoryList.h"
#include "Message/GlobalNativeTags.h"
#include "UI/Common/UW_UpgradeConfirm.h"
#include "InventoryManagerActorComponent.generated.h"

class IItemDefinitionInterface;
class UItemInstance;
class UItemCategory;
//背包物品改变消息
USTRUCT(BlueprintType)
struct FInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	APawn* InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	UItemInstance* Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 Delta = 0;
};

//背包变化通用消息
USTRUCT(BlueprintType)
struct FInventoryDeltaMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	APawn* InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 Delta = 0;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	float DeltaFloat = 0.f;
};

///背包管理组件
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class SIMPLESHOP_API UInventoryManagerActorComponent : public UActorComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerActorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* @brief 升级确认窗口类
	*/
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UUW_UpgradeConfirm> UpgradeConfirmClass;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static UInventoryManagerActorComponent* FindInventoryManagerComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UInventoryManagerActorComponent>() : nullptr);
	}

	/**
	 * @brief 是否可以添加物品定义
	 * @param ItemClass 物品类
	 * @param StackCount 数量
	 * @return true 可以； false 不可以
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	bool CanAddItemDefinition(const TSubclassOf<UObject> ItemClass, const int32 StackCount = 1);

	/**
	 * @brief 添加物品定义
	 * @param ItemClass 物品类
	 * @param InItemID 物品编号
	 * @param StackCount 数量
	 * @return 物品实例
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	UItemInstance* AddItemDefinition(const TSubclassOf<UObject> ItemClass, const int32 InItemID, const int32 StackCount = 1);

	/**
	 * @brief 从背包中移除指定索引的物品
	 * @param InInstanceIndex 物品编号
	 * @param InCount 数量
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItemByIndex(const int32 InInstanceIndex, const int32 InCount = 1);

	/**
	 * @brief 更新有标签的物品
	 * @param InTag 标签
	 */
	void UpdateItemsHasTag(const FGameplayTag& InTag) const;

	/**
	 * @brief 获取所有有效物品
	 * @return 物品实例数组
	 */
	TArray<UItemInstance*> GetAllValidItems() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	TArray<UItemCategory*> GetCategoryArray() const { return CategoryArray; }

	/**
	 * @brief 获取指定物品定义的总数
	 * @param ItemDef 物品定义
	 * @return 总数
	 */
	int32 GetTotalItemCountByDefinition(const TSubclassOf<UObject> ItemDef) const;

	/**
	 * @brief 获取指定实例编号的物品数量
	 * @param InInstanceIndex 实例编号
	 * @return 总数
	 */
	int32 GetTotalItemCountByIndex(const int32 InInstanceIndex) const;

	//~UObject interface
	//复制子物体
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	/**
	 * @brief 扩展背包容量
	 * @param InCount 扩容的数量
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void ExpandVolume(const int32 InCount);

	/**
	 * @brief 交换物品位置
	 * @param IndexA 索引A
	 * @param IndexB 索引B
	 * @param GuidA 全局唯一编号A
	 * @param GuidB 全局唯一编号B
	 */
	UFUNCTION(Server, Reliable, Category=Inventory)
	void SwapItemPosition(const int32 IndexA, const int32 IndexB, const FGuid GuidA, const FGuid GuidB);

	/**
	 * @brief 通过物品编号找到实例
	 * @param InInstanceID 实例编号
	 * @return 物品实例
	 */
	UItemInstance* GetItemByIndex(const int32 InInstanceID);

	/**
	 * @brief 获取指定物品在背包中的最大容量
	 * @param ItemClass 物品定义
	 * @return 可以容纳的数量
	 */
	int32 GetInventoryCapacity(const TSubclassOf<UObject> ItemClass);

	/**
	 * @brief 增加背包的负重
	 * @param Delta 变量
	 */
	void AddGravity(const float Delta);

	/**
	 * @brief 增加背包的负重
	 * @param Delta 变量
	 */
	void AddMaxGravity(const float Delta);

	FORCEINLINE int32 GetMaxVolume() const { return MaxVolume; }
	FORCEINLINE float GetMaxGravity() const { return MaxGravity; }
	FORCEINLINE float GetCurrentGravity() const { return CurrentGravity; }

	/**
	 * @brief 获取剩余的负重
	 * @return 负重
	 */
	float GetRemainingGravity() const;

	/**
	 * @brief 初始化背包
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory();

	/**
	 * @brief 升级背包
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpgradeInventory();

	/**
	 * @brief 确认升级背包
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConfirmUpgradeInventory();

	/**
	 * @brief 是否可以扩容背包
	 * @return 背包等级未满就可以
	 */
	bool CanExpandInventory() const { return Level + 1 < MaxLevel; }
private:
	/**
	 * @brief 广播背包目录消息
	 * @param InTag 标签
	 * @param OldCount 旧的数量
	 * @param NewCount 新的数量
	 */
	void BroadcastCategoryMessage(const FGameplayTag InTag, const int32 OldCount, const int32 NewCount);

	/**
	 * @brief 广播背包变化消息
	 * @param InTag 标签
	 * @param Delta 变化
	 * @param DeltaFloat 变化的浮点数 
	 */
	void BroadcastDeltaMessage(const FGameplayTag& InTag, const int32 Delta, const float DeltaFloat = 0.f) const;

	/**
	 * @brief 移除物品实例目录标签
	 * @param ItemInstance 物品实例
	 * @param StackCount 数量
	 */
	void RemoveItemInstanceCategoryTag(const UItemInstance* ItemInstance, const int32 StackCount);

	/**
	 * @brief 更新背包配置
	 */
	void UpdateInventoryConfig();
	
	/**
	 * @brief 物品列表(同步）
	 */
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	/**
	 * @brief 最大格子数量
	 */
	UPROPERTY(Replicated)
	int32 MaxVolume;

	/**
	 * @brief 升级消耗
	 */
	UPROPERTY(Replicated)
	int32 UpgradeCost;

	/**
	 * @brief 背包等级
	 */
	UPROPERTY(Replicated)
	int32 Level;

	/**
	 * @brief 背包的最大负重
	 */
	UPROPERTY(Replicated)
	float MaxGravity;

	/**
	 * @brief 背包的当前负重
	 */
	UPROPERTY(Replicated)
	float CurrentGravity;

	/** 目录标签  */
	UPROPERTY()
	FItemGameplayTagStackContainer CategoryStatTags;

	/** 目录实例数组 */
	UPROPERTY()
	TArray<UItemCategory*> CategoryArray;

	/**
	 * @brief 确认窗口指针
	 */
	UPROPERTY()
	UUW_UpgradeConfirm* ConfirmWindow;

	/**
	 * @brief 最大背包等级
	 */
	UPROPERTY()
	int32 MaxLevel = 6;
};
