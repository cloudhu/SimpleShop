// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameplayTagContainer.h"
#include "Interface/ItemDefinitionInterface.h"
#include "ItemDefinition.generated.h"

/**
 * 物品定义
 */
UCLASS(Blueprintable, BlueprintType, Const, Abstract)
class SIMPLESHOP_API UItemDefinition : public UObject, public IItemDefinitionInterface
{
	GENERATED_BODY()
public:
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	 * @brief 显示名称
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	FText DisplayName;

	/**
	 * @brief 简介
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	FText Introduction;

	/**
	 * @brief 类型标签
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemDisplay)
	FGameplayTagContainer TypeTags;

	/**
	 * @brief 图标显示
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	FSlateBrush IconBrush;

	/**
	 * @brief 子类编号数组
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	TArray<int32> Children;

	/**
	 * @brief 最大数量,默认无上限
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	int32 MaxCount = 100;

	/**
	 * @brief 一个背包格子中可叠加的数量,默认一个格子放一个物品,消耗品可以叠加
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemDisplay)
	int32 OverlyingCount = 1;

	/**
	 * @brief 重量
	 */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	float ItemGravity = 0.1f;

	/**
	 * @brief 初始物品数据
	 */
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

	/// <summary>
	/// IItemDefinitionInterface接口实现,语法就是在函数名后面加后缀:_Implementation
	/// </summary>
public:
	virtual const TArray<int32> GetChildrenIds_Implementation() const override;

	virtual const FText GetDisplayName_Implementation() const override;
	
	virtual const FSlateBrush GetIconBrush_Implementation() const override;
	
	virtual const FText GetIntroduction_Implementation() const override;
	
	virtual const int32 GetMaxAmount_Implementation() const override;
	
	virtual const int32 GetOverlyingAmount_Implementation() const override;
	
	virtual const FGameplayTagContainer GetTypeTags_Implementation() const override;

	virtual const TMap<FGameplayTag, int32> GetStats_Implementation() const override;

	virtual const float GetGravity_Implementation() const override;
};
