//  Copyright CloudHu. All Rights Reserved.
#include "UI/QuickBar/UW_QuickBarItem.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/Inventory/UW_DragDropIcon.h"
#include "UI/Inventory/UW_InventoryItem.h"
#include "ActorComponents/QuickBarComponent.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/ItemTable.h"
#include "Definition/ItemInstance.h"


void UUW_QuickBarItem::OnClickedWidget()
{
	if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
	{
		QuickBar->SetActiveSlotIndex(GetInstanceIndex());
	}
}

void UUW_QuickBarItem::SetQuickKeyText(const int32 InNum) const
{
	if (InNum == INDEX_NONE)
	{
		QuickKey->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	QuickKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	QuickKey->SetText(FText::Format(NSLOCTEXT("UUW_ItemSlotBase", "SetNumText", "{0}"), InNum));
}

bool UUW_QuickBarItem::IsQuickBarItem() const
{
	return true;
}

void UUW_QuickBarItem::OnNotificationSlotChangeMessage(FGameplayTag Channel, const FSlotChangedMessage& Notification)
{
	if (Notification.Owner == GetOwningPlayerPawn())
	{
		if (Notification.SlotInstance->GetIndex() == GetInstanceIndex())
		{
			UpdateItemByInstance(Notification.SlotInstance);
		}
	}
}

void UUW_QuickBarItem::OnNotificationActiveIndexChangeMessage(FGameplayTag Channel, const FQuickBarActiveIndexChangedMessage& Notification)
{
	if (Notification.Owner == GetOwningPlayerPawn())
	{
		if (Notification.ActiveIndex == GetInstanceIndex())
		{
			OnChosen->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			OnChosen->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUW_QuickBarItem::NativeConstruct()
{
	Super::NativeConstruct();

	//初始化格子
	OnChosen->SetVisibility(ESlateVisibility::Hidden);

	//注册监听消息
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	OneSlotChangeHandle = MessageSubsystem.RegisterListener(TAG_QuickBar_Message_OneSlotChanged, this, &ThisClass::OnNotificationSlotChangeMessage);

	ActiveIndexChangeHandle = MessageSubsystem.RegisterListener(TAG_QuickBar_Message_ActiveIndexChanged, this, &ThisClass::OnNotificationActiveIndexChangeMessage);
}

void UUW_QuickBarItem::NativeDestruct()
{
	Super::NativeDestruct();
	//注销监听
	if (OneSlotChangeHandle.IsValid())
	{
		OneSlotChangeHandle.Unregister();
	}

	if (ActiveIndexChangeHandle.IsValid())
	{
		ActiveIndexChangeHandle.Unregister();
	}
}

FReply UUW_QuickBarItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UUW_QuickBarItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUW_QuickBarItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	bool bDrop = false;
	//处理物品放置操作
	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (const UUW_InventoryItem* DraggedInventorySlot = Cast<UUW_InventoryItem>(InDragDropOperation->Payload))
		{
			//上述判断表明物品是从背包拖拽放到快捷栏的
			if (UInventoryManagerActorComponent* InventoryComponent = UInventoryManagerActorComponent::FindInventoryManagerComponent(GetOwningPlayerPawn()))
			{
				//获取背包组件和快捷栏组件
				if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
				{
					//将物品从背包移除，然后添加到快捷栏中
					UItemInstance* Result = InventoryComponent->GetItemByIndex(DraggedInventorySlot->GetInstanceIndex());
					InventoryComponent->RemoveItemByIndex(DraggedInventorySlot->GetInstanceIndex(), DraggedInventorySlot->GetAmount());
					QuickBar->AddItemToSlot(GetInstanceIndex(), Result);
				}

				bDrop = true;
			}
		}
		else if (const UUW_QuickBarItem* DraggedSlot = Cast<UUW_QuickBarItem>(InDragDropOperation->Payload))
		{
			//如果是从快捷栏的一个格子拖放到另外一个格子，只需要交换两者之间的数据即可
			if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(GetOwningPlayerPawn()))
			{
				QuickBar->SwapItemSlot(GetInstanceIndex(), DraggedSlot->GetInstanceIndex());
				bDrop = true;
			}
		}
	}

	return bDrop;
}

void UUW_QuickBarItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);
	SetQuickKeyText(GetInstanceIndex() + 1);
}
