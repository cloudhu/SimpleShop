// Copyright CloudHu. All Rights Reserved.
#include "ActorComponents/ShopActorComponent.h"
#include "UI/Shop/UW_ShopLayout.h"
#include "UI/Tips/UW_ConfirmWindow.h"
#include "NativeGameplayTags.h"
#include "Message/GlobalNativeTags.h"

// Sets default values for this component's properties
UShopActorComponent::UShopActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UShopActorComponent::OnNotificationTransactionMessage(FGameplayTag Channel, const FTransactionMessage& Notification)
{
	CacheNotification = Notification;
	//弹出确认窗口
	ConfirmWindow->SetVisibility(ESlateVisibility::Visible);
	
	if (Notification.Buyer == GetPawnChecked<APawn>())//如果是买家
	{
		if (Notification.bIsCompoundItem)//合成物品
		{
			ConfirmWindow->SetTitle(FText::Format(NSLOCTEXT("ConfirmWindow", "Title", "Confirm Compound at Price:{0}"), Notification.Price));
		}
		else
		{
			ConfirmWindow->SetTitle(FText::Format(NSLOCTEXT("ConfirmWindow", "Title", "Confirm Purchase at Price:{0}"), Notification.Price));
		}
	}
	else if (Notification.Seller == GetPawnChecked<APawn>())
	{
		ConfirmWindow->SetTitle(FText::Format(NSLOCTEXT("ConfirmWindow", "Title", "Confirm Sell at Price:{0}"), Notification.Price));
	}
	ConfirmWindow->SetCachePrice(Notification.Price);
	ConfirmWindow->SetItemName(Notification.DisplayName);
	ConfirmWindow->SetSliderAmount(Notification.MaxCount);
	ConfirmWindow->SetPrice(FText::Format(NSLOCTEXT("ConfirmWindow", "Price", "{0}"), Notification.Price));
}

void UShopActorComponent::ConfirmAction(const int32 Count) const
{
	FConfirmedTransactionMessage ConfirmedMessage;
	ConfirmedMessage.Buyer = CacheNotification.Buyer;
	ConfirmedMessage.Seller = CacheNotification.Seller;
	ConfirmedMessage.Count = Count;
	ConfirmedMessage.Price = CacheNotification.Price * Count;
	ConfirmedMessage.InstanceIndex = CacheNotification.InstanceID;
	ConfirmedMessage.ItemID = CacheNotification.ItemID;
	ConfirmedMessage.bIsQuickBarItem = CacheNotification.bIsQuickBarItem;
	ConfirmedMessage.bIsCompoundItem = CacheNotification.bIsCompoundItem;
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_ConfirmedTransaction_Message, ConfirmedMessage);
}

void UShopActorComponent::CancelAction() const
{
	FTransactionMessageResult ResultMessage;
	ResultMessage.Buyer = CacheNotification.Buyer;
	ResultMessage.Seller = CacheNotification.Seller;
	ResultMessage.ItemID = CacheNotification.ItemID;
	ResultMessage.bSuccess = false;
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Transaction_Message_Result, ResultMessage);
}

void UShopActorComponent::ShowShop(APawn* InOwner, bool bVisible)
{
	ShopLayoutPtr->ShowShop(InOwner, bVisible);
}

void UShopActorComponent::ChangeShopOwner(APawn* InOwner)
{
	ShopLayoutPtr->ChangeShopOwner(InOwner);
}

void UShopActorComponent::ShowInventory(bool bVisible /*= true*/)
{
	ShopLayoutPtr->ShowInventory(bVisible);
}

void UShopActorComponent::BeginPlay()
{
	Super::BeginPlay();
	//生成商店的缓存
	if (!ShopLayoutPtr)
	{
		if (ShopLayoutClass)
		{
			ShopLayoutPtr = CreateWidget<UUW_ShopLayout>(GetController<APlayerController>(), ShopLayoutClass);
			ShopLayoutPtr->AddToViewport(5);
			ShopLayoutPtr->ShowShop(GetPawnChecked<APawn>(),false);
			ConfirmWindow = ShopLayoutPtr->GetConfirmWindow();
			ConfirmWindow->ConfirmActionDelegate.BindUObject(this, &UShopActorComponent::ConfirmAction);
			ConfirmWindow->CanceledDelegate.BindUObject(this, &UShopActorComponent::CancelAction);
		}
	}

	// 监听交易消息
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	TransactionListenerHandle = MessageSystem.RegisterListener(TAG_Transaction_Message, this, &ThisClass::OnNotificationTransactionMessage);
}

void UShopActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//取消监听
	if (TransactionListenerHandle.IsValid())
	{
		TransactionListenerHandle.Unregister();
	}
}