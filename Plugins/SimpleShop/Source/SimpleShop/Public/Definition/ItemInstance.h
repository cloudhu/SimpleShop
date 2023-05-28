//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	/**
	 * @brief 为标签添加堆栈数量,仅限服务器
	 * @param Tag 标签
	 * @param StackCount 数量 
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddStatTagStack(const FGameplayTag Tag, const int32 StackCount);

	/**
	 * @brief 移除标签的堆栈数量
	 * @param Tag 标签
	 * @param StackCount 数量 
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveStatTagStack(const FGameplayTag Tag, const int32 StackCount = 0);

	/**
	 * @brief 获取标签的堆栈
	 * @param Tag 标签
	 * @return 数量
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetStatTagStackCount(const FGameplayTag Tag) const;

	/**
	 * @brief 查询是否有指定标签的
	 * @param Tag 标签
	 * @return true 有；false 无
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool HasTagExact(const FGameplayTag Tag) const;

	/**
	 * @brief 是否有指定堆栈标签
	 * @param Tag 标签
	 * @return true 有；false 无
	 */
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

	/**
	 * @brief 拿到物品名称
	 * @return 物品名称
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	FText GetItemName() const;

	/**
	 * @brief 获取物品图标
	 * @return 图标刷子
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	FSlateBrush GetIconBrush() const;

	/**
	 * @brief 设置物品编号
	 * @param InID 
	 */
	void SetItemID(const int32 InID);

	/**
	 * @brief 设置索引
	 * @param InIndex 索引 
	 */
	void SetIndex(const int32 InIndex);

	/**
	 * @brief 设置全局唯一编号
	 * @param InGuid 
	 */
	void SetGuid(const FGuid InGuid);

public:
	/**
	 * @brief 获取物品编号
	 * @return 物品编号
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ItemInstance)
	int32 GetItemID() const { return ItemID; }

	/**
	 * @brief 获取在背包中的索引
	 * @return 索引
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ItemInstance)
	int32 GetIndex() const { return Index; }

	/**
	 * @brief 获取全局唯一编号
	 * @return 全局唯一编号
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=ItemInstance)
	FGuid GetGuid() const { return Guid; }

	/// <summary>
	/// 设置物品定义
	/// </summary>
	/// <param name="InDef">物品定义</param>
	void SetItemDef(const TSubclassOf<UObject> InDef);
private:
	/**
	 * @brief 标签栈
	 */
	UPROPERTY(Replicated)
	FItemGameplayTagStackContainer StatTags;

	/**
	 * @brief The item definition 物品定义
	 */
	UPROPERTY(Replicated)
	TSubclassOf<UObject> ItemDef;

	/**
	 * @brief 物品编号:对应物品配置表
	 */
	UPROPERTY(Replicated)
	int32 ItemID = INDEX_NONE;

	/**
	 * @brief 物品实例的索引
	 */
	UPROPERTY(Replicated)
	int32 Index = INDEX_NONE;

	/**
	 * @brief 实例的唯一编号
	 */
	UPROPERTY(Replicated)
	FGuid Guid;

	/**
	 * @brief 物品所属类型
	 */
	UPROPERTY(Replicated)
	FGameplayTagContainer Tags;
};
