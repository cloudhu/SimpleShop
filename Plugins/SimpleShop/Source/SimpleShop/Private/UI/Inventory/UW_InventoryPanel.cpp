//  Copyright CloudHu. All Rights Reserved.
#include "UI/Inventory/UW_InventoryPanel.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "ActorComponents/WalletActorComponent.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "Definition/ItemInstance.h"
#include "Message/GlobalNativeTags.h"

//PRAGMA_DISABLE_OPTIMIZATION
UUW_InventoryPanel::UUW_InventoryPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_InventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
	//消息监听
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	CategoryClickedListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_CategoryOnClick_Message, this, &ThisClass::OnCategoryClickMessage);
	CategoryUpdateListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_Category_Message, this, &ThisClass::OnCategoryChangeMessage);
	WalletListenerHandle = MessageSystem.RegisterListener(TAG_Wallet_Message_GoldChanged, this, &ThisClass::OnWalletChangeMessage);
	GravityListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_Gravity_Message, this, &ThisClass::OnInventoryGravityMessage);
	MaxGravityListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_MaxGravity_Message, this, &ThisClass::OnInventoryMaxGravityMessage);
	//监听背包已扩展的消息
	ExpandListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_Expanded_Message, this, &ThisClass::OnInventoryExpandMessage);
	//ItemChangeListenerHandle = MessageSystem.RegisterListener(TAG_Inventory_Message_StackChanged, this, &ThisClass::OnItemStackChanged);

	if (UWalletActorComponent* Wallet = UWalletActorComponent::FindWalletActorComponent(GetOwningPlayerPawn()))
	{
		SetGoldText(Wallet->GetCurrentGoldNum()); //初始化金币数值显示
	}

	//背包数据初始化UI
	if (UInventoryManagerActorComponent* InventoryManager = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
	{
		SetTextNum(Text_MaxGravity, InventoryManager->GetMaxGravity()); //初始化背包负重数值显示
		SetTextNum(Text_Gravity, InventoryManager->GetCurrentGravity());
		//初始化背包
		InventoryManager->InitializeInventory();
		for (UItemCategory* Category : InventoryManager->GetCategoryArray()) //初始化目录
		{
			ListCategory->AddItem(Category);
		}
	}

	Button_Close->OnClicked.AddDynamic(this, &ThisClass::OnCloseInventory);
	//绑定扩展背包按钮点击事件
	ButtonExpand->OnClicked.AddDynamic(this,&ThisClass::ExpandInventory);
	if (ListCategory)
	{
		ListCategory->OnItemClicked().AddUObject(this, &UUW_InventoryPanel::OnItemClicked); //Todo:这个OnItemClicked触发有问题,十次也许可以触发一次,这是什么原因???
	}
}


void UUW_InventoryPanel::UpdateItemListByTag(const FGameplayTag& InTag) const
{
	if (const UInventoryManagerActorComponent* InventoryManager = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
	{
		InventoryManager->UpdateItemsHasTag(InTag);
	}
}

//这里不能添加const,尽管Rider提示给成员函数添加const修饰,但是加上之后编译就会报错,而且错误不会指向这里
void UUW_InventoryPanel::OnCategoryClickMessage(FGameplayTag Channel, const FCategoryClickMessage& Notification)
{
	if (Notification.Character == GetOwningPlayerPawn())
	{
		CacheTag = Notification.Tag;
		UpdateItemListByTag(Notification.Tag);
	}
}

void UUW_InventoryPanel::OnCategoryChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification)
{
	if (Notification.Character == GetOwningPlayerPawn())
	{
		if (Notification.NewNum == 0) //删除
		{
			ListCategory->RemoveItem(Notification.ItemCategory);
		}

		if (Notification.Delta == Notification.NewNum) //新增
		{
			ListCategory->AddItem(Notification.ItemCategory);
		}
	}
}

void UUW_InventoryPanel::OnWalletChangeMessage(FGameplayTag Channel, const FWalletChangeMessage& Notification)
{
	if (Notification.Character == GetOwningPlayerPawn())
	{
		SetGoldText(Notification.NewNum);
	}
}


void UUW_InventoryPanel::OnInventoryGravityMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification)
{
	if (Notification.InventoryOwner == GetOwningPlayerPawn())
	{
		SetTextNum(Text_Gravity, Notification.DeltaFloat);
	}
}

void UUW_InventoryPanel::OnInventoryMaxGravityMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification)
{
	if (Notification.InventoryOwner == GetOwningPlayerPawn())
	{
		SetTextNum(Text_MaxGravity, Notification.DeltaFloat);
	}
}

void UUW_InventoryPanel::OnInventoryExpandMessage(FGameplayTag Channel, const FInventoryDeltaMessage& Notification)
{
	if (Notification.InventoryOwner == GetOwningPlayerPawn())
	{
		const int32 Index = TileView_ItemList->GetNumItems();
		//循环生成对应数量的格子，从而达到扩容的目标
		for (int i = Index; i < Index + Notification.Delta; ++i)
		{
			UItemInstance* Instance = NewObject<UItemInstance>(GetOwningPlayerPawn());
			Instance->SetIndex(i);
			TileView_ItemList->AddItem(Instance);
		}
	}
	//更新物品列表
	UpdateItemListByTag(CacheTag);
}

void UUW_InventoryPanel::SetGoldText(const int32 InNum) const
{
	Text_Gold->SetText(FText::Format(NSLOCTEXT("UUW_InventoryPanel", "SetGoldText", "{0}"), InNum));
}

void UUW_InventoryPanel::SetTextNum(UTextBlock* TextSrc, const float InNum) const
{
	TextSrc->SetText(FText::Format(NSLOCTEXT("UUW_InventoryPanel", "SetTextNum", "{0}"), InNum));
}

void UUW_InventoryPanel::OnCloseInventory()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UUW_InventoryPanel::ExpandInventory()
{
	//获取背包管理组件
	if (UInventoryManagerActorComponent* InventoryManager = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
	{
		//判断背包是否满足扩容的条件
		if (InventoryManager->CanExpandInventory())
		{
			InventoryManager->UpgradeInventory();
			//背包升级之后，再次判断能否扩容，无法扩容则隐藏扩展按钮
			if (!InventoryManager->CanExpandInventory())
			{
				ButtonExpand->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{//无法扩容则隐藏扩展按钮
			ButtonExpand->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

//PRAGMA_ENABLE_OPTIMIZATION

void UUW_InventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();
	if (CategoryClickedListenerHandle.IsValid())
	{
		CategoryClickedListenerHandle.Unregister();
	}

	if (CategoryUpdateListenerHandle.IsValid())
	{
		CategoryUpdateListenerHandle.Unregister();
	}

	if (WalletListenerHandle.IsValid())
	{
		WalletListenerHandle.Unregister();
	}

	if (GravityListenerHandle.IsValid())
	{
		GravityListenerHandle.Unregister();
	}

	if (MaxGravityListenerHandle.IsValid())
	{
		MaxGravityListenerHandle.Unregister();
	}

	if (ExpandListenerHandle.IsValid())
	{
		ExpandListenerHandle.Unregister();
	}

	// if (ItemChangeListenerHandle.IsValid())
	// {
	// 	ItemChangeListenerHandle.Unregister();
	// }
}

void UUW_InventoryPanel::OnItemClicked(UObject* Item)
{
	if (const UItemCategory* Category = Cast<UItemCategory>(Item))
	{
		UpdateItemListByTag(Category->GetTag());
	}
}

// void UUW_InventoryPanel::OnItemStackChanged(FGameplayTag Channel, const FInventoryChangeMessage& Notification)
// {
// 	if (Notification.InventoryOwner == GetOwningPlayerPawn())
// 	{
// 		if (Notification.NewCount == 0)
// 		{
// 			TileView_ItemList->RemoveItem(Notification.Instance);
// 		}
//
// 		if (Notification.NewCount == Notification.Delta)
// 		{
// 			TileView_ItemList->AddItem(Notification.Instance);
// 		}
// 	}
// }