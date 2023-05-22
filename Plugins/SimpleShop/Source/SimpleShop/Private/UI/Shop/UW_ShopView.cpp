// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_ShopView.h"
#include "Blueprint/DragDropOperation.h"
#include "Data/ItemTable.h"
#include "Message/GlobalNativeTags.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Core/ShopSubsystem.h"
#include "Message/UserInterfaceMessage.h"
#include "UI/Shop/UW_ShopItem.h"

UUW_ShopView::UUW_ShopView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_ShopView::OnCategoryClicked(FGameplayTag Channel, const FCategoryClickMessage& Notification)
{
	if (Notification.Character == GetOwningPlayerPawn())
	{
		UpdateItem(Notification.Tag);
	}
}

void UUW_ShopView::NativeConstruct()
{
	Super::NativeConstruct();
	// 监听目录点击消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(TAG_Shop_CategoryOnClick_Message, this, &ThisClass::OnCategoryClicked);

	//获取ShopSubsystem商店子系统单例 Use ShopSubsystem
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	//1.查找对应品类的物品
	if (const TArray<FItemTable*>* InSlotTableArray = ShopSubsystem.GetSlotTablesTemplate())
	{
		for (const FItemTable* Tmp : *InSlotTableArray)
		{
			if (const UObject* Obj = GetDefault<UObject>(Tmp->ItemDefinition))
			{
				//获取物品标签,将物品添加的类目中
				TArray<FGameplayTag> ItemGameplayTags;
				if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
				{
					ItemDef->Execute_GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				}
				else if (Obj->Implements<UItemDefinitionInterface>())
				{
					IItemDefinitionInterface::GetTypeTags(Obj).GetGameplayTagArray(ItemGameplayTags);
				}
				
				for (const FGameplayTag& TmpTag : ItemGameplayTags)
				{
					CategoryStatTags.AddStack(TmpTag, 1);
				}
			}
		}
	}

	CategoryStatTags.SortMap();

	for (const auto& Tmp : CategoryStatTags.GetTagMap())
	{
		BroadcastCategoryMessage(Tmp.Key, 0, Tmp.Value);
	}

	UpdateItem(TAG_Item_Type_All);
}

void UUW_ShopView::NativeDestruct()
{
	Super::NativeDestruct();
	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}
}

void UUW_ShopView::SetShopOwner(APawn* InShopOwner)
{
	ShopOwner = InShopOwner;
}

//PRAGMA_DISABLE_OPTIMIZATION
void UUW_ShopView::UpdateItem(const FGameplayTag& TypeTag) const
{
	ItemGrid->ClearChildren();
	//Use ShopSubsystem
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	//1.查找对应品类的物品
	if (const TArray<FItemTable*>* InSlotTableArray = ShopSubsystem.GetSlotTablesTemplate())
	{
		TArray<const FItemTable*> InTableArray;

		for (const auto& Tmp : *InSlotTableArray)
		{
			if (Tmp->ItemTypeHasTag(TypeTag))
			{
				InTableArray.Add(Tmp);
			}
		}

		//2.生成物品
		for (int32 i = 0; i < InTableArray.Num(); i++)
		{
			if (UUW_ShopItem* InSlotWidget = CreateWidget<UUW_ShopItem>(GetWorld(), ItemClass))
			{
				if (UUniformGridSlot* GridSlot = ItemGrid->AddChildToUniformGrid(InSlotWidget))
				{
					//i = 0 , 1  2 3  ,4,5
					//(0,0) (0,1)
					//(1,0) (1,1)
					//(2,0) (2,1)
					GridSlot->SetRow(FMath::FloorToInt(static_cast<float>(i) / 2.f));
					if (i & 0x1)
					{
						GridSlot->SetColumn(1);
					}
					else
					{
						GridSlot->SetColumn(0);
					}

					GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

					InSlotWidget->UpdateSlot(InTableArray[i]);
					//店主应该是商品的主人
					InSlotWidget->SetItemOwner(ShopOwner);
				}
			}
		}
	}
}

bool UUW_ShopView::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	bool bDrop = false;
	//这里就是放下的操作了
	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (const UUW_ItemSlotBase* DraggedSlot = Cast<UUW_ItemSlotBase>(InDragDropOperation->Payload))
		{
			DraggedSlot->SellItem();
			bDrop = true;
		}
	}
	return bDrop;
}

//PRAGMA_ENABLE_OPTIMIZATION

void UUW_ShopView::OnCloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);

	CloseCompoundPanel();
}


void UUW_ShopView::CloseCompoundPanel() const
{
	FUserInterfaceMessage Message;
	Message.Owner = GetOwningPlayerPawn();
	Message.ItemID = INDEX_NONE; //收到INDEX_NONE时关闭面板
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Item_Message_Compound, Message);
}

void UUW_ShopView::BroadcastCategoryMessage(const FGameplayTag InTag, const int32 OldCount, const int32 NewCount) const
{
	FCategoryInstanceChangeMessage Message;
	Message.Character = GetOwningPlayerPawn();
	UItemCategory* Category = NewObject<UItemCategory>(GetOwningPlayerPawn());
	Category->SetTag(InTag);
	Category->SetAmount(NewCount);
	Message.ItemCategory = Category;
	Message.NewNum = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Shop_Category_Message, Message);

	if (OldCount == 0) //新增
	{
		CategoryList->AddItem(Category);
	}

	if (Message.NewNum == 0) //删除
	{
		CategoryList->RemoveItem(Category);
	}
}
