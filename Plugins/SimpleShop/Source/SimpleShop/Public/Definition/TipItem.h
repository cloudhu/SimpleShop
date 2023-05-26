// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TipItem.generated.h"

/**
 * 传递物品提示数据的对象
 */
UCLASS(Blueprintable, BlueprintType, Const)
class SIMPLESHOP_API UTipItem :public UObject
{
	GENERATED_BODY()
public:
	UTipItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * @brief 物品的标签
	 */
	UPROPERTY(BlueprintReadOnly, Category = Item)
		FGameplayTag Chanel;

	/**
	 * @brief 数目
	 */
	UPROPERTY(BlueprintReadOnly, Category = Item)
		int32 Delta = INDEX_NONE;

	/**
	 * @brief 全局唯一编号
	 */
	FGuid Guid;

	/**
	 * @brief 设置标签
	 * @param InTag 标签
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetTag(FGameplayTag InTag);

	/**
	 * @brief 设置数量
	 * @param InAmount 数值
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetAmount(int32 InAmount);
};
