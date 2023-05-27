//  Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_ShopItem.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "ActorComponents/WalletActorComponent.h"
#include "Data/ItemTable.h"
#include "Interface/ItemDefinitionInterface.h"
#include "Message/GlobalNativeTags.h"
#include "Message/UserInterfaceMessage.h"

void UUW_ShopItem::OnClickedWidget()
{
	Super::OnClickedWidget();
	if (StockAmount <= 0)
	{
		return;
	}
	//发送交易消息
	FTransactionMessage TransactionMessage;
	TransactionMessage.Buyer = GetOwningPlayerPawn();
	TransactionMessage.Seller = GetItemOwner();
	TransactionMessage.ItemID = GetItemID();
	TransactionMessage.Price = GetPrice();
	int32 MaxCountTmp = 1;
	if (const UObject* Obj = GetDefault<UObject>(GetDataTable()->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
		{
			MaxCountTmp = ItemDef->Execute_GetMaxAmount(Obj); //物品拥有上限
			TransactionMessage.DisplayName = ItemDef->Execute_GetDisplayName(Obj);
		}
		else if (Obj->Implements<UItemDefinitionInterface>())
		{
			MaxCountTmp = IItemDefinitionInterface::GetMaxAmount(Obj); //物品拥有上限
			TransactionMessage.DisplayName = IItemDefinitionInterface::GetDisplayName(Obj);
		}
	}

	if (UWalletActorComponent* Wallet = UWalletActorComponent::FindWalletActorComponent(GetOwningPlayerPawn()))
	{
		if (const int32 CanBuyCount = Wallet->GetCurrentGoldNum() / GetPrice(); CanBuyCount < MaxCountTmp)
		{
			MaxCountTmp = CanBuyCount; //玩家支付上限
		}
	}
	if (UInventoryManagerActorComponent* Inventory = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
	{
		if (const int32 CanHaveCount = Inventory->GetInventoryCapacity(GetDataTable()->ItemDefinition); CanHaveCount < MaxCountTmp)
		{
			MaxCountTmp = CanHaveCount; //背包容量上限
		}
	}
	if (MaxCountTmp > StockAmount) //库存上限
	{
		MaxCountTmp = StockAmount;
	}
	TransactionMessage.MaxCount = MaxCountTmp;
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Transaction_Message, TransactionMessage);
}

void UUW_ShopItem::UpdateSlot(const FItemTable* InTable)
{
	//更新基类
	Super::UpdateSlot(InTable);
	//更新库存数量
	StockAmount = InTable->Stock;
	SetNumText(StockAmount);
}

void UUW_ShopItem::TransactionResult(const bool bSuccess, const int32 InAmount)
{
	Super::TransactionResult(bSuccess, InAmount);
	if (bSuccess)
	{
		StockAmount -= InAmount;
		SetNumText(StockAmount);
	}
}

FReply UUW_ShopItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//右键查看合成图
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		FUserInterfaceMessage Message;
		Message.Owner = GetOwningPlayerPawn();
		if (GetDataTable()->ItemTypeHasTag(TAG_Item_Type_Compoundable))
		{
			Message.ItemID = GetItemID();
		}
		else
		{
			Message.ItemID = INDEX_NONE; //不可合成的物品
		}
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSystem.BroadcastMessage(TAG_Item_Message_Compound, Message);
	}

	return FReply::Handled();
}

void UUW_ShopItem::OnNotificationWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification)
{
	if (GetOwningPlayerPawn() == Notification.Character)
	{
		bool bCanBuy = StockAmount > 0;
		if (UWalletActorComponent* Wallet = UWalletActorComponent::FindWalletActorComponent(Notification.Character))
		{
			bCanBuy = Wallet->CanAfford(GetPrice());
		}
		//先判断钱够不够,钱够了判断背包是否装得下
		if (bCanBuy)
		{
			if (UInventoryManagerActorComponent* Inventory = UInventoryManagerActorComponent::FindInventoryManagerComponent(Notification.Character))
			{
				bCanBuy = Inventory->CanAddItemDefinition(GetDataTable()->ItemDefinition);
			}
		}

		SetItemIsEnabled(bCanBuy);
	}
}

void UUW_ShopItem::NativeConstruct()
{
	Super::NativeConstruct();
	//监听钱包发出的货币变更消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	WalletListenerHandle = MessageSystem.RegisterListener(TAG_Wallet_Message_GoldChanged, this, &ThisClass::OnNotificationWalletChangeMessage);
}

void UUW_ShopItem::NativeDestruct()
{
	Super::NativeDestruct();
	if (WalletListenerHandle.IsValid())
	{
		WalletListenerHandle.Unregister();
	}
}
