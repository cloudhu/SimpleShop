//  Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_ShopCategory.h"
#include "Message/GlobalNativeTags.h"
#include "Message/UserInterfaceMessage.h"

void UUW_ShopCategory::OnWidgetClicked()
{
	Super::OnWidgetClicked();
	UCH_GameplayMessageSubsystem& MessageSubsystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	FCategoryClickMessage Message;
	Message.Character = GetOwningPlayerPawn();
	Message.Tag = Tag;
	Message.NewNum = CacheNum;
	MessageSubsystem.BroadcastMessage(TAG_Shop_CategoryOnClick_Message, Message);
}

void UUW_ShopCategory::BindCategory(const FGameplayTag InTag)
{
	Super::BindCategory(InTag);
	// 监听词缀消息
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(TAG_Shop_Category_Message, this, &ThisClass::OnNumberChangeMessage);
	OnClickedListenerHandle = MessageSystem.RegisterListener(TAG_Shop_CategoryOnClick_Message, this, &ThisClass::ResumeCategory);
}

void UUW_ShopCategory::OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification)
{
	Super::OnNumberChangeMessage(Channel, Notification);
}

void UUW_ShopCategory::ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification)
{
	Super::ResumeCategory(Channel, Notification);
}
