//  Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_CompoundItem.h"
#include "ActorComponents/WalletActorComponent.h"
#include "Core/ShopSubsystem.h"
#include "Data/ItemTable.h"
#include "Interface/ItemDefinitionInterface.h"
#include "Message/GlobalNativeTags.h"

UUW_CompoundItem::UUW_CompoundItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_CompoundItem::OnClickedWidget()
{
	Super::OnClickedWidget();
	//发送交易消息
	FTransactionMessage TransactionMessage;
	TransactionMessage.Buyer = GetOwningPlayerPawn();
	TransactionMessage.Seller = GetItemOwner();
	TransactionMessage.ItemID = GetItemID();
	TransactionMessage.Price = GetCompoundPrice();
	TransactionMessage.bIsCompoundItem = true;
	TransactionMessage.MaxCount = 1;
	if (const UObject* Obj = GetDefault<UObject>(GetDataTable()->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
		{
			TransactionMessage.DisplayName = ItemDef->Execute_GetDisplayName(Obj);
		}
		else if (Obj->Implements<UItemDefinitionInterface>())
		{
			TransactionMessage.DisplayName = IItemDefinitionInterface::GetDisplayName(Obj);
		}
	}

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Transaction_Message, TransactionMessage);
}

void UUW_CompoundItem::OnNotificationWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification)
{
	if (GetOwningPlayerPawn() == Notification.Character)
	{
		if (UWalletActorComponent* Wallet = UWalletActorComponent::FindWalletActorComponent(Notification.Character))
		{
			SetIsEnabled(Wallet->CanAfford(GetCompoundPrice()));
		}
	}
}

int32 UUW_CompoundItem::GetCompoundPrice()
{
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());
	const int32 CompoundPrice = ShopSubsystem.CalculateCompoundItemCost(GetDataTable(), GetOwningPlayerPawn());
	SetItemPrice(CompoundPrice);
	return CompoundPrice;
}

void UUW_CompoundItem::NativeConstruct()
{
	Super::NativeConstruct();
	//监听钱包发出的货币变更消息
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	WalletListenerHandle2 = MessageSystem.RegisterListener(TAG_Wallet_Message_GoldChanged, this, &ThisClass::OnNotificationWalletChangeMessage);
}

void UUW_CompoundItem::NativeDestruct()
{
	Super::NativeDestruct();
	if (WalletListenerHandle2.IsValid())
	{
		WalletListenerHandle2.Unregister();
	}
}
