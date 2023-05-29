//  Copyright CloudHu. All Rights Reserved.
#include "UI/Inventory/UW_InventoryItem.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/Inventory/UW_DragDropIcon.h"
#include "UI/QuickBar/UW_QuickBarItem.h"
#include "ActorComponents/QuickBarComponent.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "Data/ItemTable.h"
#include "Definition/ItemInstance.h"
#include "Message/GlobalNativeTags.h"

void UUW_InventoryItem::OnClickedWidget()
{
	Super::OnClickedWidget();//先调用基类的
	//背包物品被点击时发送物品出售的消息
	//交易消息声明
	FTransactionMessage TransactionMessage;
	//交易时,买家暂时是空，物品所有者被设置成卖家
	TransactionMessage.Buyer = nullptr; 
	TransactionMessage.Seller = GetItemOwner();
	TransactionMessage.ItemID = GetItemID();
	TransactionMessage.InstanceID = GetInstanceIndex();
	TransactionMessage.Price = GetPrice() * 0.5f;
	//通过物品定义接口获取物品显示名称
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
	//声明最大出售数量的临时变量
	int32 MaxSellAmount = 0;
	//如果是快捷栏的物品，则从快捷栏组件中获取
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
		//获取背包组件，背包中该物品的总数量就是其可以被出售的最大数量
		if (const UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
		{
			MaxSellAmount = InventoryComponent->GetTotalItemCountByIndex(GetInstanceIndex());
		}
	}

	TransactionMessage.MaxCount = MaxSellAmount;
	//用游戏消息子系统广播交易消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Transaction_Message, TransactionMessage);
}

void UUW_InventoryItem::OnItemStackChanged(FGameplayTag Chanel, const FInventoryChangeMessage& Message)
{
	if (Message.InventoryOwner == GetOwningPlayerPawn())
	{
		if (GetInstanceIndex() == Message.Instance->GetIndex())
		{
			UpdateItemByInstance(Message.Instance);
		}
	}
}

void UUW_InventoryItem::NativeConstruct()
{
	Super::NativeConstruct();
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_Inventory_Message_StackChanged, this, &ThisClass::OnItemStackChanged);
}

void UUW_InventoryItem::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UUW_InventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	//判断是否是鼠标右键 或 触摸事件
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		//如果是鼠标右键按住,说明玩家在拖拽背包格子
		if (GetDataTable() && GetDataTable()->IsValid()) //仅拖拽有数据的格子
		{
			FReply Reply = FReply::Handled();
			if (const TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget(); SlateWidgetDrag.IsValid())
			{
				//触发拖拽的事件
				Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
				return Reply;
			}
		}
	}

	return FReply::Handled();
}

void UUW_InventoryItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	//这个类生成拖拽的图标，所以需要判断其是否有效
	if (DragDropIconClass) 
	{
		//获取背包格子对应的数据缓存
		if (const FItemTable* InSlotData = GetDataTable()) 
		{
			if (InSlotData->IsValid())//判断数据是否有效
			{
				//生成拖拽的图标
				if (UUW_DragDropIcon* DragDropIcon = CreateWidget<UUW_DragDropIcon>(GetWorld(), DragDropIconClass))
				{
					//新建一个拖放操作对象
					if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass()))
					{
						//使用物品定义接口来获取物品的图标
						if (const UObject* Src = GetDefault<UObject>(InSlotData->ItemDefinition))
						{
							if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Src))
							{
								DragDropIcon->DrawIcon(ItemDef->Execute_GetIconBrush(Src)); //赋值给图标
							}
							else if (Src->Implements<UItemDefinitionInterface>())
							{
								DragDropIcon->DrawIcon(IItemDefinitionInterface::GetIconBrush(Src)); //赋值给图标
							}
						}
						InDropOperation->SetFlags(RF_StrongRefOnFrame);
						InDropOperation->DefaultDragVisual = DragDropIcon;
						InDropOperation->Payload = this;
						OutOperation = InDropOperation;
						//拖拽过程中关闭背包格子内的图标显示
						ShowIcon(false);
					}
				}
			}
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUW_InventoryItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	bool bDrop = false;
	//这里就是放下的操作了,如果是背包内的拖拽,那么可以理解成两个背包格子之间的交换位置
	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		//这里通过Cast转化操作判断其来源是否是背包，因为用户也可以将快捷栏的物品拖放到背包中
		if (const UUW_InventoryItem* DraggedInventorySlot = Cast<UUW_InventoryItem>(InDragDropOperation->Payload))
		{
			//获取背包管理组件
			if (UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(
				GetOwningPlayerPawn()))
			{
				//通过背包组件来交换其位置
				InventoryComponent->SwapItemPosition(GetInstanceIndex(), DraggedInventorySlot->GetInstanceIndex(), GetGuid(),
				                                     DraggedInventorySlot->GetGuid());
				bDrop = true;
			}
		}//判断是否是从快捷栏拖拽到背包的物品
		else if (const UUW_QuickBarItem* DraggedSlot = Cast<UUW_QuickBarItem>(InDragDropOperation->Payload))
		{
			if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
			{//获取快捷栏组件
				if (UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(
					GetOwningPlayerPawn()))
				{
					//获取背包组件,将该物品添加到背包中
					InventoryComponent->AddItemDefinition(DraggedSlot->GetDataTable()->ItemDefinition, DraggedSlot->GetItemID(),
					                                      QuickBar->GetItemAmountByIndex(DraggedSlot->GetInstanceIndex()));
				}
				//将该物品从背包移除
				QuickBar->RemoveItemFromSlot(DraggedSlot->GetInstanceIndex());
			}
			bDrop = true;
		}
	}

	return bDrop;
}