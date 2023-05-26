// Copyright CloudHu. All Rights Reserved.
#include "UI/Tips/UW_ItemTips.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/ListView.h"
#include "Definition/TipItem.h"

UUW_ItemTips::UUW_ItemTips(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_ItemTips::SetDisplayNameText(const FText& InText) const
{
	if (InText.IsEmptyOrWhitespace())
	{
		DisplayName->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	DisplayName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DisplayName->SetText(InText);
}

void UUW_ItemTips::SetDescribeText(const FText& InText) const
{
	if (InText.IsEmptyOrWhitespace())
	{
		DescribeText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	DescribeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DescribeText->SetText(InText);
}

void UUW_ItemTips::SetPriceText(const FText& InText) const
{
	if (InText.IsEmptyOrWhitespace())
	{
		PriceBox->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	PriceBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	PriceText->SetText(InText);
}

void UUW_ItemTips::DisplayStats(const TMap<FGameplayTag, int32> Stats, const FGuid Guid) const
{
	if (ListAffix)
	{
		if (Stats.Num() > 0)
		{
			ListAffix->SetVisibility(ESlateVisibility::Visible);
			//遍历词缀数据
			for (auto& Stat : Stats)
			{
				//新建词缀数据传输的对象，对其进行赋值
				UTipItem* Item = NewObject<UTipItem>(GetOwningPlayerPawn());
				Item->Chanel = Stat.Key;
				Item->Delta = Stat.Value;
				Item->Guid = Guid;
				//添加到ListView列表视图中显示
				ListAffix->AddItem(Item);
			}
		}
		else
		{
			ListAffix->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
