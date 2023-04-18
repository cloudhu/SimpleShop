//  Copyright CloudHu. All Rights Reserved.
#include "UI/Common/UW_CategoryBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Definition/ItemCategory.h"
#include "Message/GlobalNativeTags.h"
#include "Message/UserInterfaceMessage.h"

UUW_CategoryBase::UUW_CategoryBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_CategoryBase::OnWidgetClicked()
{
	SetIsEnabled(false);
}

void UUW_CategoryBase::BindCategory(const FGameplayTag InTag)
{
	//取消监听
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}
	Tag = InTag;

	TArray<FString> TagNameArray;
	InTag.ToString().ParseIntoArray(TagNameArray, TEXT("."), false);
	SetDisplayNameText(FText::FromString(TagNameArray.Last()));
}

void UUW_CategoryBase::SetNumText(const int32 InNum)
{
	if (InNum == INDEX_NONE)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	CacheNum = InNum;
	NumText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NumText->SetText(FText::Format(NSLOCTEXT("UUW_CategoryButton", "SetNumText", "{0}"), InNum));
}

void UUW_CategoryBase::SetDisplayNameText(const FText& InText) const
{
	if (InText.IsEmptyOrWhitespace())
	{
		DisplayName->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	DisplayName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DisplayName->SetText(InText);
}

void UUW_CategoryBase::OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification)
{
	if (Notification.ItemCategory->GetTag() != Tag)
	{
		SetIsEnabled(true);
		return;
	}

	if (Notification.Character == GetOwningPlayerPawn())
	{
		SetNumText(Notification.NewNum);
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UUW_CategoryBase::NativeDestruct()
{
	Super::NativeDestruct();
	//取消监听
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}

	if (OnClickedListenerHandle.IsValid())
	{
		OnClickedListenerHandle.Unregister();
	}
}

void UUW_CategoryBase::NativeConstruct()
{
	Super::NativeConstruct();
	ItemButton->OnClicked.AddDynamic(this, &ThisClass::OnWidgetClicked);
}

void UUW_CategoryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (const UItemCategory* Item = Cast<UItemCategory>(ListItemObject))
	{
		SetNumText(Item->GetAmount());
		BindCategory(Item->GetTag());

		IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	}
}

void UUW_CategoryBase::ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification)
{
	if (Notification.Character == GetOwningPlayerPawn() && Notification.Tag != Tag)
	{
		SetIsEnabled(true);
	}
}
