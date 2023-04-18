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
 * 提示信息
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ItemTips : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ItemTips(const FObjectInitializer& ObjectInitializer);
public:
	//显示名称
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* DisplayName;

	//描述
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* DescribeText;

	//价格
	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UTextBlock* PriceText;

	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UHorizontalBox* PriceBox;

	UPROPERTY(BlueprintReadOnly, Category = Tips, meta = (BindWidget))
		UListView* ListTips;
public:
	void SetDisplayNameText(const FText& InText) const;

	void SetDescribeText(const FText& InText) const;

	void SetPriceText(const FText& InText) const;

public:
	/** 监听词缀状态变化 */
	void ListenForStats(const TMap<FGameplayTag, int32> Stats,const FGuid Guid) const;

};
