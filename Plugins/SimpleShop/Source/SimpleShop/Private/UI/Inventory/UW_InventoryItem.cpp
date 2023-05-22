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
	Super::OnClickedWidget();
	//发送交易消息
	SellItem();
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
	if (DragDropIconClass) //这个类生成拖拽的图标
	{
		if (const FItemTable* InSlotData = GetDataTable()) //获取背包格子对应的数据缓存
		{
			if (InSlotData->IsValid())
			{
				if (UUW_DragDropIcon* DragDropIcon = CreateWidget<UUW_DragDropIcon>(GetWorld(), DragDropIconClass))
				{
					//生成拖拽的图标
					if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass()))
					{
						//生成拖放的操作
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
		if (const UUW_InventoryItem* DraggedInventorySlot = Cast<UUW_InventoryItem>(InDragDropOperation->Payload))
		{
			//现在只支持背包格子之间的拖拽
			if (UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(
				GetOwningPlayerPawn()))
			{
				//获取背包组件
				InventoryComponent->SwapItemPosition(GetInstanceIndex(), DraggedInventorySlot->GetInstanceIndex(), GetGuid(),
				                                     DraggedInventorySlot->GetGuid());
				bDrop = true;
			}
		}
		else if (const UUW_QuickBarItem* DraggedSlot = Cast<UUW_QuickBarItem>(InDragDropOperation->Payload))
		{
			if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
			{
				if (UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(
					GetOwningPlayerPawn()))
				{
					//获取背包组件
					InventoryComponent->AddItemDefinition(DraggedSlot->GetDataTable()->ItemDefinition, DraggedSlot->GetItemID(),
					                                      QuickBar->GetItemAmountByIndex(DraggedSlot->GetInstanceIndex()));
				}
				QuickBar->RemoveItemFromSlot(DraggedSlot->GetInstanceIndex());
			}
			bDrop = true;
		}
	}

	return bDrop;
}
