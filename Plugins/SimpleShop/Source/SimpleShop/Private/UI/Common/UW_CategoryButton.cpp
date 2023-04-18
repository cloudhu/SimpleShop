//  Copyright CloudHu. All Rights Reserved.
#include "UI/Common/UW_CategoryButton.h"
#include "Definition/TipItem.h"
#include "Message/GlobalNativeTags.h"

UUW_CategoryButton::UUW_CategoryButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_CategoryButton::OnWidgetClicked()
{
	Super::OnWidgetClicked();
	UCH_GameplayMessageSubsystem& MessageSubsystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	FCategoryClickMessage Message;
	Message.Character = GetOwningPlayerPawn();
	Message.Tag = Tag;
	Message.NewNum = CacheNum;
	MessageSubsystem.BroadcastMessage(TAG_Inventory_CategoryOnClick_Message, Message);
}

void UUW_CategoryButton::BindCategory(const FGameplayTag InTag)
{
	Super::BindCategory(InTag);
	// 监听词缀消息
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_Category_Message, this, &ThisClass::OnNumberChangeMessage);
	OnClickedListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_CategoryOnClick_Message, this, &ThisClass::ResumeCategory);
}

void UUW_CategoryButton::OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification)
{
	Super::OnNumberChangeMessage(Channel, Notification);
}

void UUW_CategoryButton::ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification)
{
	Super::ResumeCategory(Channel, Notification);
}

void UUW_CategoryButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);
}
