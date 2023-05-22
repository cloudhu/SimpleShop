// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ItemDefinitionInterface.generated.h"

// This class does not need to be modified.
// 这个类是UE自动生成的,无需修改
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
	// 接口函数在这个类里面添加,其他类需要实现这里的函数
public:
	/**
	 * @brief 获取物品显示名称
	 * @return 名称文本
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FText GetDisplayName() const;

	/**
	 * @brief 获取物品简介
	 * @return 简介文本
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FText GetIntroduction() const;

	/**
	 * @brief 获取物品类型
	 * @return 类型标签容器
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FGameplayTagContainer GetTypeTags() const;

	/**
	 * @brief 获取图标显示
	 * @return 图标涂刷
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const FSlateBrush GetIconBrush() const;

	/**
	 * @brief 获取物品子类编号
	 * @return 编号数组
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const TArray<int32> GetChildrenIds() const;

	/**
	 * @brief 获取物品最大数量
	 * @return 最大值
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const int32 GetMaxAmount() const;

	/**
	 * @brief 获取物品可叠加存放的数量
	 * @return 可叠加数
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const int32 GetOverlyingAmount() const;

	/**
	 * @brief 获取数据
	 * @return 标签数量映射表
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const TMap<FGameplayTag, int32> GetStats() const;

	/**
	 * @brief 获取重量
	 * @return 重量
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ItemDefinitionInterface)
	const float GetGravity() const;
	
	///////////////////////////////////////////////////////////////////下面针对蓝图实现的静态函数
	//实现接口的蓝图类无法转化Cast到接口,只有继承的C++类才能Cast,因此这里使用静态方法来调用实现的蓝图函数
	//////////////////////////////////////////////////////////////////纯C++则无需做这些静态实现
	
	/**
	 * @brief 通过静态函数来调用蓝图实现
	 * @param Src 实现接口的蓝图类
	 * @return 标签容器
	 */
	static FGameplayTagContainer GetTypeTags(const UObject* Src);

	/**
	 * @brief 获取显示名称
	 * @param Src 实现接口的蓝图类
	 * @return 名称文本
	 */
	static FText GetDisplayName(const UObject* Src);

	/**
	 * @brief 获取简介
	 * @param Src 实现接口的蓝图类
	 * @return 简介文本
	 */
	static FText GetIntroduction(const UObject* Src);


	/**
	 * @brief 获取图标刷子
	 * @param Src 实现接口的蓝图类
	 * @return 图标
	 */
	static FSlateBrush GetIconBrush(const UObject* Src);

	/**
	 * @brief 获取子类编号
	 * @param Src 实现接口的蓝图类
	 * @return 编号数组
	 */
	static TArray<int32> GetChildrenIds(const UObject* Src);

	/**
	 * @brief 获取最大数量
	 * @param Src 实现接口的蓝图类
	 * @return 最大数量
	 */
	static int32 GetMaxAmount(const UObject* Src);

	/**
	 * @brief 获取叠加数量
	 * @param Src 实现接口的蓝图类
	 * @return 可叠加数
	 */
	static int32 GetOverlyingAmount(const UObject* Src);

	/**
	 * @brief 获取数据
	 * @param Src 实现接口的蓝图类
	 * @return 标签数量映射表
	 */
	static TMap<FGameplayTag, int32> GetStats(const UObject* Src);

	/**
	 * @brief 获取重量
	 * @param Src 实现接口的蓝图类
	 * @return 重量
	 */
	static float GetGravity(const UObject* Src);
};