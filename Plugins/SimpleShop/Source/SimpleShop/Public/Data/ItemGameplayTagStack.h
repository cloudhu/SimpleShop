// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagContainer.h"
#include "ItemGameplayTagStack.generated.h"

struct FItemGameplayTagStackContainer;

/**
 * Represents one stack of a gameplay tag (tag + count)
 * 游戏标签的堆栈(标签+数量)
 */
USTRUCT(BlueprintType)
struct FItemGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

		FItemGameplayTagStack()
	{}

	FItemGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{
	}

	FString GetDebugString() const;

private:
	friend FItemGameplayTagStackContainer;

	UPROPERTY()
		FGameplayTag Tag;

	UPROPERTY()
		int32 StackCount = 0;
};

/** Container of gameplay tag stacks 游戏标签栈容器*/
USTRUCT(BlueprintType)
struct FItemGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

		FItemGameplayTagStackContainer()
		//	: Owner(nullptr)
	{
	}

public:
	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1) 增
	void AddStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1) 删
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present) 拿
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// Returns true if there is at least one stack of the specified tag 查
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FItemGameplayTagStack, FItemGameplayTagStackContainer>(Stacks, DeltaParms, *this);
	}

	TMap<FGameplayTag, int32> GetTagMap() const { return TagToCountMap; }

	void SortMap();
private:
	// Replicated list of gameplay tag stacks 栈
	UPROPERTY()
		TArray<FItemGameplayTagStack> Stacks;

	// Accelerated list of tag stacks for queries
	TMap<FGameplayTag, int32> TagToCountMap;
};

template<>
struct TStructOpsTypeTraits<FItemGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FItemGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
