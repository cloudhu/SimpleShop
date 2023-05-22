//  Copyright CloudHu. All Rights Reserved.
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "Core/ShopSubsystem.h"
#include "Definition/ItemInstance.h"
#include "Definition/ItemCategory.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Interface/ItemDefinitionInterface.h"
#include "Message/UserInterfaceMessage.h"
#include "Message/GlobalNativeTags.h"

// Sets default values for this component's properties
UInventoryManagerActorComponent::UInventoryManagerActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InventoryList(this)
	  , MaxVolume(0)
	  , UpgradeCost(800)
	  , Level(0)
	  , MaxGravity(0.f)
	  , CurrentGravity(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UInventoryManagerActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, MaxVolume);
	DOREPLIFETIME(ThisClass, UpgradeCost);
	DOREPLIFETIME(ThisClass, Level);
	DOREPLIFETIME(ThisClass, MaxGravity);
	DOREPLIFETIME(ThisClass, CurrentGravity);
}

bool UInventoryManagerActorComponent::CanAddItemDefinition(const TSubclassOf<UObject> ItemClass, const int32 StackCount /*= 1*/)
{
	check(GetOwner()->HasAuthority());
	if (const UObject* Obj = GetDefault<UObject>(ItemClass))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
		{
			if ((GetTotalItemCountByDefinition(ItemClass) + StackCount) < ItemDef->Execute_GetMaxAmount(Obj) && GetRemainingGravity() >= ItemDef->Execute_GetGravity(Obj) *
				StackCount)
			{
				return InventoryList.CanAddItem(ItemClass, StackCount);
			}
		}
		else if (Obj->Implements<UItemDefinitionInterface>())
		{
			if ((GetTotalItemCountByDefinition(ItemClass) + StackCount) < IItemDefinitionInterface::GetMaxAmount(Obj) && GetRemainingGravity() >=
				IItemDefinitionInterface::GetGravity(Obj) * StackCount)
			{
				return InventoryList.CanAddItem(ItemClass, StackCount);
			}
		}
	}
	return false;
}

UItemInstance* UInventoryManagerActorComponent::AddItemDefinition(const TSubclassOf<UObject> ItemClass, const int32 InItemID, const int32 StackCount /*= 1*/)
{
	UItemInstance* Result = nullptr;
	if (ItemClass != nullptr && CanAddItemDefinition(ItemClass, StackCount))
	{
		//获取物品类的默认对象CDO
		if (const UObject* Obj = GetDefault<UObject>(ItemClass))
		{
			//0.获取物品标签,将物品添加的类目中
			TArray<FGameplayTag> ItemGameplayTags;
			int32 OverlyingCount = 0;
			float ItemGravity = 0.f;
			//如果是C++实现,则可以直接将其Cast转化成接口
			if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				//C++实现了接口,这里直接调用
				ItemDef->Execute_GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				//1.获取物品可叠加的数量
				OverlyingCount = ItemDef->Execute_GetOverlyingAmount(Obj);
				ItemGravity = ItemDef->Execute_GetGravity(Obj);
			}//否则就需要判断其是否为蓝图实现
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				//使用接口的静态方法来调用接口函数
				IItemDefinitionInterface::GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				//1.获取物品可叠加的数量
				OverlyingCount = IItemDefinitionInterface::GetOverlyingAmount(Obj);
				ItemGravity = IItemDefinitionInterface::GetGravity(Obj);
			}
			else
			{
				return Result;
			}

			ItemGameplayTags.StableSort();
			for (const FGameplayTag& TmpTag : ItemGameplayTags)
			{
				int32 OldCount = 0;
				if (CategoryStatTags.ContainsTag(TmpTag))
				{
					OldCount = CategoryStatTags.GetStackCount(TmpTag);
				}
				CategoryStatTags.AddStack(TmpTag, StackCount);
				BroadcastCategoryMessage(TmpTag, OldCount, OldCount + StackCount);
			}

			int32 Total = StackCount; //总共需要添加物品数量
			while (Total > 0)
			{
				if (Total >= OverlyingCount)
				{
					Result = InventoryList.AddEntry(ItemClass, InItemID, OverlyingCount);
					AddGravity(OverlyingCount * ItemGravity);
					Total -= OverlyingCount; //减去已经添加的数量
				}
				else //不足的数量一次性添加
				{
					Result = InventoryList.AddEntry(ItemClass, InItemID, Total);
					AddGravity(Total * ItemGravity);
					Total = 0;
				}
			}
		}
	}
	return Result;
}

void UInventoryManagerActorComponent::RemoveItemByIndex(const int32 InInstanceIndex, const int32 InCount)
{
	check(GetOwner()->HasAuthority());
	RemoveItemInstanceCategoryTag(InventoryList.GetItemInstanceByIndex(InInstanceIndex), InCount);
	InventoryList.RemoveEntryByIndex(InInstanceIndex, InCount);
}

void UInventoryManagerActorComponent::UpdateItemsHasTag(const FGameplayTag& InTag) const
{
	InventoryList.UpdateTaggedItems(InTag);
}

TArray<UItemInstance*> UInventoryManagerActorComponent::GetAllValidItems() const
{
	return InventoryList.GetAllValidItems();
}

int32 UInventoryManagerActorComponent::GetTotalItemCountByDefinition(const TSubclassOf<UObject> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		if (const UItemInstance* Instance = Entry.Instance; IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				TotalCount += Entry.StackCount;
			}
		}
	}

	return TotalCount;
}

int32 UInventoryManagerActorComponent::GetTotalItemCountByIndex(const int32 InInstanceIndex) const
{
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		if (const UItemInstance* Instance = Entry.Instance; IsValid(Instance))
		{
			if (Instance->GetIndex() == InInstanceIndex)
			{
				return Entry.StackCount;
			}
		}
	}

	return 0;
}

bool UInventoryManagerActorComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		if (UItemInstance* Instance = Entry.Instance; Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

float UInventoryManagerActorComponent::GetRemainingGravity() const
{
	return GetMaxGravity() - GetCurrentGravity();
}

void UInventoryManagerActorComponent::InitializeInventory()
{
	UpdateInventoryConfig();
}

void UInventoryManagerActorComponent::UpgradeInventory()
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
			ConfirmWindow->ConfirmDelegate.BindUObject(this, &UInventoryManagerActorComponent::ConfirmUpgradeInventory);
			ConfirmWindow->bIsFocusable = true;
		}
		else
		{
			return;
		}
	}
	ConfirmWindow->SetCurrentLevel(Level);
	ConfirmWindow->SetPrice(UpgradeCost);
	ConfirmWindow->SetVisibility(ESlateVisibility::Visible);
	ConfirmWindow->SetFocus();
}

void UInventoryManagerActorComponent::ConfirmUpgradeInventory()
{
	if (UWalletActorComponent* WalletActorComponent = UWalletActorComponent::FindWalletActorComponent(GetPawn<APawn>()))
	{
		if (WalletActorComponent->CanAfford(UpgradeCost))
		{
			if (WalletActorComponent->Transaction(UpgradeCost, false))
			{
				Level++;
				UpdateInventoryConfig();
			}
		}
	}
}


void UInventoryManagerActorComponent::UpdateInventoryConfig()
{
	//Use ShopSubsystem 读取配置表 更新背包数据
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());
	const FInventoryConfiguration* Configuration = ShopSubsystem.GetInventoryConfigurationByLevel(Level);
	ExpandVolume(Configuration->AddedVolume);
	AddMaxGravity(Configuration->MaxGravity);
	UpgradeCost = Configuration->UpgradeCost;
	MaxLevel = ShopSubsystem.GetMaxInventoryLevel();
}

void UInventoryManagerActorComponent::ExpandVolume(const int32 InCount)
{
	check(GetOwner()->HasAuthority());
	if (GetMaxVolume() + InCount > 0)
	{
		MaxVolume += InCount;

		//背包扩展后发消息通知UI更新
		BroadcastDeltaMessage(TAG_Inventory_Expanded_Message, InCount);
	}
}

void UInventoryManagerActorComponent::SwapItemPosition_Implementation(const int32 IndexA, const int32 IndexB, const FGuid GuidA, const FGuid GuidB)
{
	InventoryList.SwapEntry(IndexA, IndexB, GuidA, GuidB);
}

UItemInstance* UInventoryManagerActorComponent::GetItemByIndex(const int32 InInstanceID)
{
	return InventoryList.GetItemInstanceByIndex(InInstanceID);
}

int32 UInventoryManagerActorComponent::GetInventoryCapacity(const TSubclassOf<UObject> ItemClass)
{
	return InventoryList.GetInventoryCapacity(ItemClass);
}

void UInventoryManagerActorComponent::AddGravity(const float Delta)
{
	if (CurrentGravity + Delta <= MaxGravity)
	{
		CurrentGravity += Delta;
		//背包负重变化后发消息通知UI更新
		BroadcastDeltaMessage(TAG_Inventory_Gravity_Message, 0, CurrentGravity);
	}
}

void UInventoryManagerActorComponent::AddMaxGravity(const float Delta)
{
	MaxGravity += Delta;
	//背包负重变化后发消息通知UI更新
	BroadcastDeltaMessage(TAG_Inventory_MaxGravity_Message, 0, MaxGravity);
}

void UInventoryManagerActorComponent::BroadcastCategoryMessage(const FGameplayTag InTag, const int32 OldCount, const int32 NewCount)
{
	FCategoryInstanceChangeMessage Message;
	Message.Character = GetOwner<APawn>();
	UItemCategory* Category = nullptr;

	if (OldCount == 0) //新增
	{
		Category = NewObject<UItemCategory>(GetOwner<APawn>());
		Category->SetTag(InTag);
		Category->SetAmount(NewCount);
		CategoryArray.Add(Category);
	}
	else //修改
	{
		for (auto EntryIt = CategoryArray.CreateIterator(); EntryIt; ++EntryIt)
		{
			Category = *EntryIt;
			if (Category->GetTag() == InTag)
			{
				break;
			}
		}
	}
	Message.ItemCategory = Category;
	Message.NewNum = NewCount;
	Message.Delta = NewCount - OldCount;

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Inventory_Category_Message, Message);
	if (Message.NewNum == 0) //删除
	{
		CategoryArray.Remove(Category);
	}
}

void UInventoryManagerActorComponent::BroadcastDeltaMessage(const FGameplayTag& InTag, const int32 Delta, const float DeltaFloat) const
{
	FInventoryDeltaMessage Message;
	Message.InventoryOwner = GetOwner<APawn>();
	Message.Delta = Delta;
	Message.DeltaFloat = DeltaFloat;
	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(InTag, Message);
}

void UInventoryManagerActorComponent::RemoveItemInstanceCategoryTag(const UItemInstance* ItemInstance, const int32 StackCount)
{
	if (ItemInstance)
	{
		if (const UObject* Obj = GetDefault<UObject>(ItemInstance->GetItemDef()))
		{
			TArray<FGameplayTag> ItemGameplayTags;
			if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				ItemDef->Execute_GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				//移除物品后,减去背包负重
				AddGravity(-StackCount * ItemDef->Execute_GetGravity(Obj));
			}
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				IItemDefinitionInterface::GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				//移除物品后,减去背包负重
				AddGravity(-StackCount * IItemDefinitionInterface::GetGravity(Obj));
			}
			//移除物品的标签
			for (const FGameplayTag& TmpTag : ItemGameplayTags)
			{
				const int32 OldCount = CategoryStatTags.GetStackCount(TmpTag);
				CategoryStatTags.RemoveStack(TmpTag, StackCount);
				BroadcastCategoryMessage(TmpTag, OldCount, OldCount - StackCount);
			}
		}
	}
}
