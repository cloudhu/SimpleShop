//  Copyright CloudHu. All Rights Reserved.
#include "UI/Common/UW_UpgradeConfirm.h"
#include "Components/TextBlock.h"

void UUW_UpgradeConfirm::ConfirmAction()
{
	//如果确认代理绑定了就执行，然后隐藏确认窗口
	if (ConfirmDelegate.ExecuteIfBound())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUW_UpgradeConfirm::CancelAction()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUW_UpgradeConfirm::NativeConstruct()
{
	Super::NativeConstruct();
	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::ConfirmAction);
	CancelButton->OnClicked.AddDynamic(this, &ThisClass::CancelAction);
}

void UUW_UpgradeConfirm::SetCurrentLevel(const int32 InLevel) const
{
	CurrentLevel->SetText(FText::Format(NSLOCTEXT("UpgradeConfirm", "CurrentLv", "{0}"), InLevel));
	NextLevel->SetText(FText::Format(NSLOCTEXT("UpgradeConfirm", "NextLv", "{0}"), InLevel+1));
}

void UUW_UpgradeConfirm::SetPrice(const int32 InCost) const
{
	PriceText->SetText(FText::Format(NSLOCTEXT("UpgradeConfirm", "Price", "{0}"), InCost));
}
