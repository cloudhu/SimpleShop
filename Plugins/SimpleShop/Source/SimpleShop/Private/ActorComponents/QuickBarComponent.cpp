// Copyright Epic Games, Inc. All Rights Reserved.
#include "ActorComponents/QuickBarComponent.h"
#include "NativeGameplayTags.h"

#include "ActorComponents/InventoryManagerActorComponent.h"

#include "Blueprint/UserWidget.h"

#include "Core/ShopSubsystem.h"

#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Definition/ItemInstance.h"
#include "Message/GlobalNativeTags.h"

UQuickBarComponent::UQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UQuickBarComponent::BeginPlay()
{
	UpdateSlots();

	Super::BeginPlay();
}

void UQuickBarComponent::SwapItemSlot(int32 SlotIndexA, int32 SlotIndexB)
{
	//1.确定索引在有效范围
	if (Slots.IsValidIndex(SlotIndexA) && Slots.IsValidIndex(SlotIndexB))
	{
		//2.交换数据
		Slots[SlotIndexA]->SetIndex(SlotIndexB);
		UItemInstance* ItemA = Slots[SlotIndexA];
		Slots[SlotIndexB]->SetIndex(SlotIndexA);
		Slots[SlotIndexA] = Slots[SlotIndexB];
		Slots[SlotIndexB] = ItemA;

		//3.更新UI
		OnSlotChanged(Slots[SlotIndexA]);
		OnSlotChanged(Slots[SlotIndexB]);

		//4.如果是激活的索引,则替换激活的索引
		if (ActiveSlotIndex == SlotIndexA)
		{
			SetActiveSlotIndex(SlotIndexB);
		}
		else if (ActiveSlotIndex == SlotIndexB)
		{
			SetActiveSlotIndex(SlotIndexA);
		}
	}
}

void UQuickBarComponent::SendEquipItemMessage(const UItemInstance* SlotItem, const bool bEquip) const
{
	if (SlotItem)
	{
		FEquipChangedMessage Message;
		Message.Owner = GetOwner();
		Message.bEquip = bEquip;
		Message.ItemDef = SlotItem->GetItemDef();
		Message.Guid = SlotItem->GetGuid();
		UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(this);
		MessageSystem.BroadcastMessage(TAG_QuickBar_Message_EquipChanged, Message);
	}
}

void UQuickBarComponent::UpdateSlots()
{
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());
	const FQuickBarConfiguration* Configuration = ShopSubsystem.GetQuickBarConfigurationByLevel(Level);
	NumSlots = Configuration->MaxVolume;
	MaxGravity = Configuration->MaxGravity;
	UpgradeCost = Configuration->UpgradeCost;
	MaxLevel = ShopSubsystem.GetMaxQuickBarLevel();
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	for (int32 i = 0; i < NumSlots; i++)
	{
		if (Slots[i] == nullptr)
		{
			Slots[i] = NewObject<UItemInstance>(GetOwner());
			Slots[i]->SetIndex(i);
		}
	}
	//3.更新UI
	OnRep_Slots();
}

void UQuickBarComponent::OnSlotChanged(UItemInstance* SlotItem) const
{
	FSlotChangedMessage Message;
	Message.Owner = GetOwner<APawn>();
	Message.SlotInstance = SlotItem;

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_QuickBar_Message_OneSlotChanged, Message);
}

int32 UQuickBarComponent::GetItemAmountByIndex(int32 ItemIndex) const
{
	if (Slots.IsValidIndex(ItemIndex))
	{
		return Slots[ItemIndex]->GetStatTagStackCount(TAG_Inventory_Item_Count);
	}
	return 0;
}

void UQuickBarComponent::OnExpandQuickBar()
{
	if (!ConfirmWindow)
	{
		if (UpgradeConfirmClass)
		{
			ConfirmWindow = CreateWidget<UUW_UpgradeConfirm>(GetController<APlayerController>(), UpgradeConfirmClass);
			ConfirmWindow->AddToViewport(10);
			ConfirmWindow->SetDesiredSizeInViewport(ConfirmWindow->GetDesiredSize());
			FAnchors Anchors;
			Anchors.Minimum = FVector2d(0.5f);
			Anchors.Maximum = FVector2d(0.5f);
			ConfirmWindow->SetAnchorsInViewport(Anchors);
			ConfirmWindow->SetAlignmentInViewport(FVector2d(0.5f));
			//ConfirmWindow->SetPositionInViewport(FVector2d(0.f));
			ConfirmWindow->ConfirmDelegate.BindUObject(this, &UQuickBarComponent::ConfirmUpgradeQuickBar);
			ConfirmWindow->bIsFocusable = true;
		}
	}
	ConfirmWindow->SetCurrentLevel(Level);
	ConfirmWindow->SetPrice(UpgradeCost);
	ConfirmWindow->SetVisibility(ESlateVisibility::Visible);
	ConfirmWindow->SetFocus();
}

void UQuickBarComponent::ConfirmUpgradeQuickBar()
{
	if (UWalletActorComponent* WalletActorComponent = UWalletActorComponent::FindWalletActorComponent(GetPawn<APawn>()))
	{
		if (WalletActorComponent->CanAfford(UpgradeCost))
		{
			if (WalletActorComponent->Transaction(UpgradeCost, false))
			{
				Level++;
				UpdateSlots();
			}
		}
	}
}

void UQuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	}
	while (NewIndex != OldIndex);
}

void UQuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	}
	while (NewIndex != OldIndex);
}

void UQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));

	if (UItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		SendEquipItemMessage(SlotItem,true);
	}
}

void UQuickBarComponent::UnEquipItemInSlot()
{
	if (Slots.IsValidIndex(ActiveSlotIndex))
	{
		if (UItemInstance* SlotItem = Slots[ActiveSlotIndex])
		{
			SendEquipItemMessage(SlotItem,false);
		}
	}
}

void UQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnEquipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

UItemInstance* UQuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 UQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (TObjectPtr<UItemInstance> ItemPtr : Slots)
	{
		if (ItemPtr->GetItemID() == INDEX_NONE)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

void UQuickBarComponent::AddItemToSlot(int32 SlotIndex, UItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		Slots[SlotIndex]->SetIndex(SlotIndex);
		Slots[SlotIndex]->SetItemID(Item->GetItemID());
		Slots[SlotIndex]->SetItemDef(Item->GetItemDef());
		Slots[SlotIndex]->AddStatTagStack(TAG_Inventory_Item_Count, Item->GetStatTagStackCount(TAG_Inventory_Item_Count));
		OnSlotChanged(Slots[SlotIndex]);
	}
}

UItemInstance* UQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnEquipItemInSlot();
		ActiveSlotIndex = -1;
		OnRep_ActiveSlotIndex();
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex]->SetItemDef(nullptr);
			Slots[SlotIndex]->SetItemID(INDEX_NONE);
			Slots[SlotIndex]->RemoveStatTagStack(TAG_Inventory_Item_Count);
			OnSlotChanged(Slots[SlotIndex]);
		}
	}

	return Result;
}

void UQuickBarComponent::SetNumSlots(int32 InNumSlots)
{
	NumSlots = InNumSlots;
	if (NumSlots > 9)
	{
		NumSlots = 9;
	}
	UpdateSlots();
}

void UQuickBarComponent::OnRep_Slots() const
{
	FQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner<APawn>();
	Message.Slots = Slots;

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_QuickBar_Message_SlotsChanged, Message);
}

void UQuickBarComponent::OnRep_ActiveSlotIndex() const
{
	FQuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner<APawn>();
	Message.ActiveIndex = ActiveSlotIndex;

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_QuickBar_Message_ActiveIndexChanged, Message);
}
