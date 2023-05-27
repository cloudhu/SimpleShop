// Copyright CloudHu. All Rights Reserved.
#include "UI/Core/UW_ItemSlotBase.h"
#include "UI/Tips/UW_ItemTips.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Core/ShopSubsystem.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "ActorComponents/QuickBarComponent.h"
#include "Components/TextBlock.h"
#include "Definition/ItemInstance.h"
#include "Message/GlobalNativeTags.h"

UUW_ItemSlotBase::UUW_ItemSlotBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , InstanceGuid(FGuid())
	  , ItemID(INDEX_NONE)
	  , InstanceIndex(INDEX_NONE)
	  , Price(9999)
	  , Amount(0)
	  , TipPtr(nullptr)
	  , DataTable(nullptr)
{
}

void UUW_ItemSlotBase::SetItemID(int32 InID)
{
	ItemID = InID;
}

void UUW_ItemSlotBase::SetInstanceIndex(int32 InInstanceID)
{
	InstanceIndex = InInstanceID;
}

void UUW_ItemSlotBase::SetItemPrice(int32 InPrice)
{
	Price = InPrice;
}


void UUW_ItemSlotBase::SetIconBrush(const FSlateBrush& Brush) const
{
	ItemIcon->SetBrush(Brush);
}

void UUW_ItemSlotBase::SetItemOwner(APawn* InOwner)
{
	ItemOwner = InOwner;
}

void UUW_ItemSlotBase::OnClickedWidget()
{
	SetItemIsEnabled(false);
}

void UUW_ItemSlotBase::UpdateSlot(const FItemTable* InTable)
{
	if (InTable == nullptr)
	{
		//如果传入的数据无效则重置缓存数据
		DataTable = nullptr;
		SetItemID(INDEX_NONE);
		SetItemPrice(9999);
		ItemButton->SetToolTip(nullptr);
		TipPtr = nullptr;
		SetItemIsEnabled(false);
		ShowIcon(false);
		return;
	}

	if (DataTable == InTable)
	{
		//相同的数据不需要更新
		ShowIcon(true);
		return;
	}

	if (InTable && InTable->IsValid())
	{
		//缓存数据
		DataTable = InTable;
		//更新UI
		ShowIcon(true);
		SetItemID(InTable->ID);
		SetItemPrice(InTable->Price);
		//更新提示信息
		if (!TipPtr)
		{
			ItemButton->SetToolTip(GetTip());
		}
		//获取物品定义的CDO对象
		if (const UObject* Obj = GetDefault<UObject>(InTable->ItemDefinition))
		{
			//如果是C++实现，则可以转化成接口
			if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				//通过接口获取物品定义的数据，根据数据更新UI显示
				SetIconBrush(ItemDef->Execute_GetIconBrush(Obj));
				TipPtr->SetDisplayNameText(ItemDef->Execute_GetDisplayName(Obj));
				TipPtr->SetDescribeText(ItemDef->Execute_GetIntroduction(Obj));
				TipPtr->DisplayStats(ItemDef->Execute_GetStats(Obj), GetGuid());
			}//蓝图实现则调用接口的静态方法
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				SetIconBrush(IItemDefinitionInterface::GetIconBrush(Obj));
				TipPtr->SetDisplayNameText(IItemDefinitionInterface::GetDisplayName(Obj));
				TipPtr->SetDescribeText(IItemDefinitionInterface::GetIntroduction(Obj));
				TipPtr->DisplayStats(IItemDefinitionInterface::GetStats(Obj), GetGuid());
			}
		}
		//设置价格
		TipPtr->SetPriceText(FText::Format(NSLOCTEXT("UUW_ItemSlotBase", "SetPriceText", "{0}"), Price));
		TipPtr->SetVisibility(ESlateVisibility::HitTestInvisible);
		SetItemIsEnabled(true);
	}
}

void UUW_ItemSlotBase::TransactionResult(const bool bSuccess, const int32 InAmount)
{
	if (!bSuccess)
	{
		SetItemIsEnabled(true);
	}
}

//这里不能添加const,尽管Rider提示给成员函数添加const修饰,但是加上之后编译就会报错,而且错误不会指向这里
void UUW_ItemSlotBase::OnNotificationTransactionResultMessage(FGameplayTag Channel, const FTransactionMessageResult& Notification)
{
	if (ItemID == Notification.ItemID) //如果购买的物品需要重写激活
	{
		TransactionResult(Notification.bSuccess, Notification.Amount);
	}
}

void UUW_ItemSlotBase::SetItemIsEnabled(const bool bEnabled) const
{
	ItemButton->SetIsEnabled(bEnabled);
	ItemIcon->SetIsEnabled(bEnabled);
}

void UUW_ItemSlotBase::ShowIcon(const bool bVisible) const
{
	if (bVisible)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_ItemSlotBase::BroadcastTransactionMessage() const
{
	//交易消息声明
	FTransactionMessage TransactionMessage;
	TransactionMessage.Buyer = GetItemOwner(); //交易时,物品所有者被设置成触发交易的角色
	TransactionMessage.Seller = GetOwningPlayerPawn();
	TransactionMessage.ItemID = GetItemID();
	TransactionMessage.InstanceID = GetInstanceIndex();
	TransactionMessage.Price = GetPrice() * 0.5f;
	if (const UObject* Src = GetDefault<UObject>(GetDataTable()->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Src))
		{
			TransactionMessage.DisplayName = ItemDef->Execute_GetDisplayName(Src);
		}
		else if (Src->Implements<UItemDefinitionInterface>())
		{
			TransactionMessage.DisplayName = IItemDefinitionInterface::GetDisplayName(Src);
		}
	}
	int32 MaxSellAmount = 0;

	if (IsQuickBarItem())
	{
		if (const UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
		{
			MaxSellAmount = QuickBar->GetItemAmountByIndex(GetInstanceIndex());
			TransactionMessage.bIsQuickBarItem = true;
		}
	}
	else
	{
		//获取背包组件
		if (const UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
		{
			MaxSellAmount = InventoryComponent->GetTotalItemCountByIndex(InstanceIndex);
		}
	}

	TransactionMessage.MaxCount = MaxSellAmount;
	//用游戏消息子系统广播交易消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Transaction_Message, TransactionMessage);
}

//PRAGMA_DISABLE_OPTIMIZATION
void UUW_ItemSlotBase::UpdateItemByInstance(const UItemInstance* Instance)
{
	if (Instance->GetItemID() != INDEX_NONE)
	{
		UShopSubsystem& ShopSystem = UShopSubsystem::Get(GetWorld());

		UpdateSlot(ShopSystem.GetSlotTableByID(Instance->GetItemID()));
	}
	else
	{
		UpdateSlot(nullptr);
	}
	SetInstanceIndex(Instance->GetIndex());
	SetInstanceGuid(Instance->GetGuid());
	if (Instance->HasStatTagExact(TAG_Inventory_Item_Count))
	{
		SetNumText(Instance->GetStatTagStackCount(TAG_Inventory_Item_Count));
	}
	else
	{
		CountText->SetVisibility(ESlateVisibility::Hidden);
	}
}

//PRAGMA_ENABLE_OPTIMIZATION

void UUW_ItemSlotBase::SetInstanceGuid(const FGuid& InGuid)
{
	InstanceGuid = InGuid;
}

void UUW_ItemSlotBase::SetNumText(const int32 InNum)
{
	if (InNum == INDEX_NONE)
	{
		CountText->SetVisibility(ESlateVisibility::Hidden);
		Amount = 0;
		return;
	}
	Amount = InNum;
	CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CountText->SetText(FText::Format(NSLOCTEXT("UUW_ItemSlotBase", "SetNumText", "{0}"), InNum));
}

void UUW_ItemSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedWidget);
	//监听交易消息: 0.获取游戏消息子系统
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//1.通过游戏消息子系统注册监听交易结果的消息
	TransactionMessageListenerHandle = MessageSystem.RegisterListener(TAG_Transaction_Message_Result, this, &ThisClass::OnNotificationTransactionResultMessage);
}

UWidget* UUW_ItemSlotBase::GetTip()
{
	//如果没有缓存的指针，就创建一个
	if (!TipPtr)
	{
		if (TipClass)//创建该部件必须要有对应的类
		{
			TipPtr = CreateWidget<UUW_ItemTips>(GetWorld(), TipClass);
		}
	}
	//返回提示窗口的指针
	return TipPtr;
}

void UUW_ItemSlotBase::NativeDestruct()
{
	Super::NativeDestruct();
	//取消监听
	if (TransactionMessageListenerHandle.IsValid())
	{
		TransactionMessageListenerHandle.Unregister();
	}
}

void UUW_ItemSlotBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	if (const UItemInstance* Instance = Cast<UItemInstance>(ListItemObject))
	{
		UpdateItemByInstance(Instance);
	}
}
