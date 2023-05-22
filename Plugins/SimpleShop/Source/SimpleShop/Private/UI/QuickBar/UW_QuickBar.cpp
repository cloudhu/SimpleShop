//  Copyright CloudHu. All Rights Reserved.
#include "UI/QuickBar/UW_QuickBar.h"
#include "Components/Button.h"
#include "Definition/ItemInstance.h"
#include "Message/GlobalNativeTags.h"

void UUW_QuickBar::NativeConstruct()
{
	Super::NativeConstruct();

	//监听快捷栏变化
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	OnSlotsChangeListenerHandle = MessageSubsystem.RegisterListener(TAG_QuickBar_Message_SlotsChanged, this, &ThisClass::OnNotificationSlotsChangeMessage);

	ButtonExpand->OnClicked.AddDynamic(this, &ThisClass::ExpandQuickBar);
}

void UUW_QuickBar::NativeDestruct()
{
	Super::NativeDestruct();

	//注销监听
	if (OnSlotsChangeListenerHandle.IsValid())
	{
		OnSlotsChangeListenerHandle.Unregister();
	}
}

void UUW_QuickBar::OnNotificationSlotsChangeMessage(FGameplayTag Channel, const FQuickBarSlotsChangedMessage& Notification)
{
	if (Notification.Owner == GetOwningPlayerPawn())
	{
		if (ListView_Slots->GetNumItems() != Notification.Slots.Num())
		{
			ListView_Slots->ClearListItems();
			for (UItemInstance* Instance : Notification.Slots)
			{
				ListView_Slots->AddItem(Instance);
			}
		}

		if (Notification.Slots.Num() == 9)
		{
			ButtonExpand->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUW_QuickBar::ExpandQuickBar()
{
	if (UQuickBarComponent* QuickBarComponent = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
	{
		if (QuickBarComponent->CanExpand())
		{
			QuickBarComponent->OnExpandQuickBar();
		}
	}
}

UUW_QuickBar::UUW_QuickBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
