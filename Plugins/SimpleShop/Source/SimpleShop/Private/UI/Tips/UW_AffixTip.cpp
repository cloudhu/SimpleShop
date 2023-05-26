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
	//取消之前的监听，避免重复监听
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}

	// 利用游戏消息子系统监听词缀消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(Tag, this, &ThisClass::OnAffixChangeMessage);

	//将游戏标签拆解，只显示最末尾的关键字，比如Item.Type.Crit暴击，这里只截取Crit关键字显示
	TArray<FString> stringArray;
	Tag.ToString().ParseIntoArray(stringArray, TEXT("."), false);
	SetDisplayNameText(FText::FromString(stringArray.Last()));
}

void UUW_AffixTip::SetDisplayNameText(const FText& InText) const
{
	//如果是空的，就没有必要显示了
	if (InText.IsEmptyOrWhitespace())
	{
		DisplayName->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	//把词缀的名称关键字显示到UI
	DisplayName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DisplayName->SetText(InText);
}

void UUW_AffixTip::SetNumText(int32 InNum)
{
	//显示词缀的数值，NSLOCTEXT是多语言文本支持的格式
	NumText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NumText->SetText(FText::Format(NSLOCTEXT("UUW_CategoryButton", "SetNumText", "{0}"), InNum));
}

void UUW_AffixTip::OnAffixChangeMessage(FGameplayTag Channel, const FAffixChangeMessage& Notification)
{
	if (Notification.Delta == 0 || Notification.Guid != Guid)
	{
		return;
	}
	//监听到变更消息后更新词缀数值
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
	//这个实现的接口函数会在物品被添加到ListView中时被自动调用。所以这个ListItemObject可以转化成指定的对象
	if (const UTipItem* Item = Cast<UTipItem>(ListItemObject))
	{
		//动态添加词缀的时候，根据传入对象的数据设置词缀的显示
		Guid = Item->Guid;
		SetNumText(Item->Delta);
		BindAffixTip(Item->Chanel);
	}
}