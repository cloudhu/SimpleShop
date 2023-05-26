// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemTips.generated.h"

struct FGameplayTag;
class UHorizontalBox;
class UTextBlock;
class UListView;
class UUW_AffixTip;
/**
 * 物品提示信息窗口
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ItemTips : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ItemTips(const FObjectInitializer& ObjectInitializer);
public:
	/**
	 * @brief 显示名称
	 */
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* DisplayName;

	/**
	 * @brief 描述文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* DescribeText;

	/**
	 * @brief 价格文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* PriceText;

	/**
	 * @brief 价格存放的盒子
	 */
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UHorizontalBox* PriceBox;

	/**
	 * @brief 词缀列表视图
	 */
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UListView* ListAffix;
public:
	/**
	 * @brief 设置显示名称文本
	 * @param InText 文本
	 */
	void SetDisplayNameText(const FText& InText) const;

	/**
	 * @brief 设置描述文本
	 * @param InText 文本
	 */
	void SetDescribeText(const FText& InText) const;

	/**
	 * @brief 设置价格文本
	 * @param InText 价格
	 */
	void SetPriceText(const FText& InText) const;

public:
	/**
	 * @brief 显示词缀
	 * @param Stats 词缀
	 * @param Guid 全局唯一编号
	 */
	void DisplayStats(const TMap<FGameplayTag, int32> Stats,const FGuid Guid) const;

};
