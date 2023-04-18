// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameplayTagContainer.h"
#include "ItemCategory.generated.h"

/**
 * 物品定义
 */
UCLASS(Blueprintable, BlueprintType, Const)
class SIMPLESHOP_API UItemCategory :public UObject
{
	GENERATED_BODY()
public:
	UItemCategory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//标签频道
	UPROPERTY(BlueprintReadOnly, Category = Item)
		FGameplayTag Chanel;

	//数目
	UPROPERTY(BlueprintReadOnly, Category = Item)
		int32 Delta = INDEX_NONE;

public:
	FGameplayTag GetTag() const { return Chanel; }

	int32 GetAmount() const { return Delta; }
public:
	/** 设置标签. */
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetTag(const FGameplayTag InTag);

	/** 设置标签. */
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetAmount(int32 InAmount);
};
