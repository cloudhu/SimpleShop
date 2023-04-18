//  Copyright CloudHu. All Rights Reserved.
#include "UI/Tips/UW_AffixTip.h"
#include "Components/TextBlock.h"
#include "Definition/TipItem.h"
#include "Message/TransactionMessage.h"

UUW_AffixTip::UUW_AffixTip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_AffixTip::BindAffixTip(FGameplayTag Tag)
{
	//取消监听
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}

	// 监听词缀消息
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(Tag, this, &ThisClass::OnAffixChangeMessage);

	TArray<FString> stringArray;
	Tag.ToString().ParseIntoArray(stringArray, TEXT("."), false);
	SetDisplayNameText(FText::FromString(stringArray.Last()));
}

void UUW_AffixTip::SetDisplayNameText(const FText& InText) const
{
	if (InText.IsEmptyOrWhitespace())
	{
		DisplayName->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	DisplayName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DisplayName->SetText(InText);
}

void UUW_AffixTip::SetNumText(int32 InNum)
{
	if (InNum == INDEX_NONE)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	NumText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NumText->SetText(FText::Format(NSLOCTEXT("UUW_CategoryButton", "SetNumText", "{0}"), InNum));
}

void UUW_AffixTip::OnAffixChangeMessage(FGameplayTag Channel, const FAffixChangeMessage& Notification)
{
	if (Notification.Delta == 0 || Notification.Guid != Guid)
	{
		return;
	}

	if (Notification.Character == GetOwningPlayerPawn())
	{
		SetNumText(Notification.NewNum);
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUW_AffixTip::NativeDestruct()
{
	Super::NativeDestruct();
	//取消监听
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}
}

void UUW_AffixTip::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	if (const UTipItem* Item = Cast<UTipItem>(ListItemObject))
	{
		Guid = Item->Guid;
		SetNumText(Item->Delta);
		BindAffixTip(Item->Chanel);
	}
}
