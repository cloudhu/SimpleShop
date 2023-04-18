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
	friend FInventoryList;
	friend UInventoryManagerActorComponent;

	UPROPERTY()
	UItemInstance* Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

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
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(
			Entries, DeltaParams, *this);
	}

	UItemInstance* AddEntry(const TSubclassOf<UObject> ItemClass, const int32 InItemID, const int32 StackCount);

	void RemoveEntry(const TSubclassOf<UObject> ItemClass, const int32 InCount);

	/// <summary>
	/// 移除指定编号的数据
	/// </summary>
	/// <param name="InInstanceIndex">物品实例编号</param>
	/// <param name="InCount">数量</param>
	void RemoveEntryByIndex(const int32 InInstanceIndex,const int32 InCount = INDEX_NONE);

	/**
	 * @brief 获取指定索引的实例
	 * @param InInstanceIndex 实例的背包索引
	 * @return 实例
	 */
	UItemInstance* GetItemInstanceByIndex(int32 InInstanceIndex); //const;

	/// <summary>
	/// 交换物品位置
	/// </summary>
	void SwapEntry(const int32 IndexA, const int32 IndexB,const FGuid GuidA,const FGuid GuidB);

	/**
	 * @brief 将物品实例移动到指定的索引
	 * @param Guid 物品唯一编号
	 * @param Index 在背包UI中的索引
	 */
	void MoveInstanceToIndex(const FGuid Guid,const int32 Index);
	
	bool CanAddItem(const TSubclassOf<UObject> ItemClass, const int32 StackCount);

	/**
	 * @brief 获取指定物品在背包中的最大容量
	 * @param ItemClass 物品定义
	 * @return 可以容纳的数量
	 */
	int32 GetInventoryCapacity(const TSubclassOf<UObject> ItemClass);

	FInventoryEntry& GetEmptyEntry(const TSubclassOf<UObject> ItemClass, const int32 StackCount);

	int32 GetValidDataAmount() const { return Entries.Num(); }

	FInventoryEntry& GetNewEntry();
	
	FInventoryEntry& FindEntryByGuid(const FGuid InGuid);

	int32 GetInventoryMaxVolume() const;
private:
	void BroadcastChangeMessage(const FInventoryEntry& Entry, const int32 OldCount, const int32 NewCount) const;
	
	void BroadcastEmptyInstanceMessage(const int32 InIndex) const;

private:
	friend UInventoryManagerActorComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	UPROPERTY()
	UActorComponent* OwnerComponent;
};

template <>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
