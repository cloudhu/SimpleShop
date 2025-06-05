//  Copyright CloudHu. All Rights Reserved.
#include "UI/Tips/UW_ConfirmWindow.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Message/GlobalNativeTags.h"
#include "Message/UserInterfaceMessage.h"

UUW_ConfirmWindow::UUW_ConfirmWindow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_ConfirmWindow::NativeConstruct()
{
	Super::NativeConstruct();
	if (CountSlider)
	{
		CountSlider->SetStepSize(1.0f);
		CountSlider->SetMinValue(1.0f);
	}

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(TAG_Item_Message_Compound, this, &ThisClass::OnNotificationCompoundMessage);
}

//PRAGMA_DISABLE_OPTIMIZATION
void UUW_ConfirmWindow::ConfirmAction()
{
	if (CountSlider)
	{
		if (const int32 Count = UKismetMathLibrary::Round(CountSlider->GetValue()); ConfirmActionDelegate.ExecuteIfBound(Count))
		{
			SetVisibility(ESlateVisibility::Hidden);
			CountSlider->SetValue(1.f);
		}
	}
}
//PRAGMA_ENABLE_OPTIMIZATION

void UUW_ConfirmWindow::CancelAction()
{
	if (CanceledDelegate.ExecuteIfBound())
	{
		SetVisibility(ESlateVisibility::Hidden);
		CountSlider->SetValue(1.f);
	}
}

void UUW_ConfirmWindow::SetSliderAmount(int32 MaxAmount) const
{
	if (CountSlider)
	{
		CountSlider->SetMaxValue(MaxAmount);
		TextMaxCount->SetText(FText::Format(NSLOCTEXT("ConfirmWindow", "MaxCount", "{0}"), MaxAmount));
	}
}

void UUW_ConfirmWindow::SetTitle(const FText& InText) const
{
	TitleText->SetText(InText);
}

void UUW_ConfirmWindow::SetItemName(const FText& InText) const
{
	ItemName->SetText(InText);
}

void UUW_ConfirmWindow::SetPrice(const FText& InText) const
{
	PriceText->SetText(InText);
}

void UUW_ConfirmWindow::SetPriceFromSlider(int32 Value)
{
	SetPrice(FText::Format(NSLOCTEXT("ConfirmWindow", "Price", "{0}"), Value * CachePrice));
}

void UUW_ConfirmWindow::SetCachePrice(const int32 InPrice)
{
	CachePrice = InPrice;
	CountSlider->SetFocus();
}

void UUW_ConfirmWindow::OnNotificationCompoundMessage(FGameplayTag Channel, const FUserInterfaceMessage& Notification)
{
	if (Notification.Owner == GetOwningPlayerPawn())
	{
		//如果物品编号是-1就隐藏起来
		if (Notification.ItemID == INDEX_NONE)
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
