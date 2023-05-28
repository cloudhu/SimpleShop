// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryList.generated.h"

class UItemInstance;
class UInventoryManagerActorComponent;
struct FInventoryList;
//背包中的一个物品
USTRUCT(BlueprintType)
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry()
	{
	}

	FString GetDebugString() const;

private:
	//友元类
	friend FInventoryList;
	friend UInventoryManagerActorComponent;

	/**
	 * @brief 物品实例
	 */
	UPROPERTY()
	UItemInstance* Instance = nullptr;

	/**
	 * @brief 物品数量
	 */
	UPROPERTY()
	int32 StackCount = 0;

	/**
	 * @brief 最后观测的数量
	 */
	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

//背包物品列表
USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	/**
	 * @brief 获取有效的所有物品数据
	 * @return 实例数组
	 */
	TArray<UItemInstance*> GetAllValidItems() const;

	/**
	 * @brief 更新那些有指定标签的物品
	 * @param InTag 目标物品的标签
	 */
	void UpdateTaggedItems(const FGameplayTag& InTag) const;
public:
	//~FFastArraySerializer contract 数组序列化加速器 合约
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(
			Entries, DeltaParams, *this);
	}

	/**
	 * @brief 新增条目
	 * @param ItemClass 物品类
	 * @param InItemID 物品编号
	 * @param StackCount 数量
	 * @return 物品实例
	 */
	UItemInstance* AddEntry(const TSubclassOf<UObject> ItemClass, const int32 InItemID, const int32 StackCount);

	/**
	 * @brief 移除条目
	 * @param ItemClass 物品类
	 * @param InCount 数量
	 */
	void RemoveEntry(const TSubclassOf<UObject> ItemClass, const int32 InCount);

	/**
	 * @brief 移除指定编号的数据
	 * @param InInstanceIndex 物品实例编号
	 * @param InCount 数量
	 */
	void RemoveEntryByIndex(const int32 InInstanceIndex,const int32 InCount = INDEX_NONE);

	/**
	 * @brief 获取指定索引的实例
	 * @param InInstanceIndex 实例的背包索引
	 * @return 实例
	 */
	UItemInstance* GetItemInstanceByIndex(int32 InInstanceIndex); //const;

	/**
	 * @brief 交换物品位置
	 * @param IndexA 索引A
	 * @param IndexB 索引B
	 * @param GuidA 全局唯一编号A
	 * @param GuidB 全局唯一编号B
	 */
	void SwapEntry(const int32 IndexA, const int32 IndexB,const FGuid GuidA,const FGuid GuidB);

	/**
	 * @brief 将物品实例移动到指定的索引
	 * @param Guid 物品唯一编号
	 * @param Index 在背包UI中的索引
	 */
	void MoveInstanceToIndex(const FGuid Guid,const int32 Index);

	/**
	 * @brief 是否可以添加物品
	 * @param ItemClass 物品类
	 * @param StackCount 数量
	 * @return true:可以添加；false:背包满了
	 */
	bool CanAddItem(const TSubclassOf<UObject> ItemClass, const int32 StackCount);

	/**
	 * @brief 获取指定物品在背包中的最大容量
	 * @param ItemClass 物品定义
	 * @return 可以容纳的数量
	 */
	int32 GetInventoryCapacity(const TSubclassOf<UObject> ItemClass);

	/**
	 * @brief 获取空格子
	 * @param ItemClass 物品类 
	 * @param StackCount 数量
	 * @return 空的条目
	 */
	FInventoryEntry& GetEmptyEntry(const TSubclassOf<UObject> ItemClass, const int32 StackCount);

	/**
	 * @brief 获取有效的数量
	 * @return 数量
	 */
	int32 GetValidDataAmount() const { return Entries.Num(); }

	/**
	 * @brief 获取新的条目
	 * @return 背包条目
	 */
	FInventoryEntry& GetNewEntry();

	/**
	 * @brief 根据全局唯一编号查找
	 * @param InGuid 全局唯一编号
	 * @return 目标物品
	 */
	FInventoryEntry& FindEntryByGuid(const FGuid InGuid);

	/**
	 * @brief 获取背包最大容量
	 * @return 容量
	 */
	int32 GetInventoryMaxVolume() const;
private:
	/**
	 * @brief 广播背包变化消息
	 * @param Entry 变化的条目
	 * @param OldCount 之前的数量
	 * @param NewCount 新的数量
	 */
	void BroadcastChangeMessage(const FInventoryEntry& Entry, const int32 OldCount, const int32 NewCount) const;

	/**
	 * @brief 广播空实例消息
	 * @param InIndex 索引
	 */
	void BroadcastEmptyInstanceMessage(const int32 InIndex) const;

private:
	friend UInventoryManagerActorComponent;

private:
	/**
	 * @brief Replicated list of items 同步物品列表
	 */
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	/**
	 * @brief 所属组件
	 */
	UPROPERTY()
	UActorComponent* OwnerComponent;
};

template <>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
