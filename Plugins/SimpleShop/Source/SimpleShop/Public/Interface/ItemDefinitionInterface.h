// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ItemDefinitionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UItemDefinitionInterface : public UInterface
{
	GENERATED_BODY()
};

class UItemFragment;
/**
 * 物品定义接口
 */
class SIMPLESHOP_API IItemDefinitionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//获取物品显示名称
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FText GetDisplayName() const;

	//获取物品简介
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FText GetIntroduction() const;

	//获取物品类型
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FGameplayTagContainer GetTypeTags() const;

	//获取图标显示
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FSlateBrush GetIconBrush() const;

	//获取物品简介
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const TArray<int32> GetChildrenIds() const;

	//获取物品最大数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const int32 GetMaxAmount() const;

	//获取物品可叠加存放的数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const int32 GetOverlyingAmount() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const TMap<FGameplayTag, int32> GetStats() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const float GetGravity() const;

	//实现接口的蓝图类无法转化Cast到接口,只有继承的C++类才能Cast,因此这里使用静态方法来调用实现的蓝图函数

	/**
	 * @brief 通过静态函数来调用蓝图实现
	 * @param Src 实现接口的蓝图类
	 * @return 标签容器
	 */
	static FGameplayTagContainer GetTypeTags(const UObject* Src);

	static FText GetDisplayName(const UObject* Src);

	static FText GetIntroduction(const UObject* Src);

	static FSlateBrush GetIconBrush(const UObject* Src);

	static TArray<int32> GetChildrenIds(const UObject* Src);

	static int32 GetMaxAmount(const UObject* Src);

	static int32 GetOverlyingAmount(const UObject* Src);

	static TMap<FGameplayTag, int32> GetStats(const UObject* Src);

	static float GetGravity(const UObject* Src);
};
