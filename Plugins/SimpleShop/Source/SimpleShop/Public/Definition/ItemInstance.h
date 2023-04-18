//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "Data/ItemGameplayTagStack.h"
#include "Interface/ItemDefinitionInterface.h"
#include "ItemInstance.generated.h"

/**
 * 物品实例
 */
UCLASS(BlueprintType, DefaultToInstanced)
class SIMPLESHOP_API UItemInstance final : public UObject
{
	GENERATED_BODY()

public:
	UItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	//为标签添加堆栈数量,仅限服务器
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddStatTagStack(const FGameplayTag Tag, const int32 StackCount);

	//移除标签的堆栈数量
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveStatTagStack(const FGameplayTag Tag, const int32 StackCount = 0);

	//获取标签的堆栈
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetStatTagStackCount(const FGameplayTag Tag) const;

	//查询是否有指定标签的
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool HasTagExact(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool HasStatTagExact(const FGameplayTag Tag) const;

	/// <summary>
	/// 获取物品实例对应的物品定义
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = Inventory)
	TSubclassOf<UObject> GetItemDef() const
	{
		return ItemDef;
	}

	//拿到物品名称
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	FText GetItemName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	FSlateBrush GetIconBrush() const;

	void SetItemID(const int32 InID);

	void SetIndex(const int32 InIndex);
	
	void SetGuid(const FGuid InGuid);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ItemInstance)
	int32 GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ItemInstance)
	int32 GetIndex() const { return Index; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=ItemInstance)
	FGuid GetGuid() const { return Guid; }

	/// <summary>
	/// 设置物品定义
	/// </summary>
	/// <param name="InDef">物品定义</param>
	void SetItemDef(const TSubclassOf<UObject> InDef);
private:
	//标签栈
	UPROPERTY(Replicated)
	FItemGameplayTagStackContainer StatTags;

	// The item definition 物品定义
	UPROPERTY(Replicated)
	TSubclassOf<UObject> ItemDef;

	//物品编号:对应物品配置表
	UPROPERTY(Replicated)
	int32 ItemID = INDEX_NONE;

	//物品实例的索引
	UPROPERTY(Replicated)
	int32 Index = INDEX_NONE;

	/**
	 * @brief 实例的唯一编号
	 */
	UPROPERTY(Replicated)
	FGuid Guid;
	
	UPROPERTY(Replicated)
	FGameplayTagContainer Tags;
};
