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

void UUW_ItemTips::ListenForStats(const TMap<FGameplayTag, int32> Stats, const FGuid Guid) const
{
	if (ListTips)
	{
		if (Stats.Num() > 0)
		{
			ListTips->SetVisibility(ESlateVisibility::Visible);
			for (auto& Stat : Stats)
			{
				UTipItem* Item = NewObject<UTipItem>(GetOwningPlayerPawn());
				Item->Chanel = Stat.Key;
				Item->Delta = Stat.Value;
				Item->Guid = Guid;
				ListTips->AddItem(Item);
			}
		}
		else
		{
			ListTips->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
