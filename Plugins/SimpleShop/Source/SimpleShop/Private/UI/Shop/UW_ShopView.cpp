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
	// 监听目录点击消息，在商店目录被点击时更新商品
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	CategoryOnclickListenerHandle = MessageSystem.RegisterListener(TAG_Shop_CategoryOnClick_Message, this, &ThisClass::OnCategoryClicked);

	//获取ShopSubsystem商店子系统单例 Use ShopSubsystem
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	//1.统计商店目录对应的数量
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
				//统计该目录下的物品数量
				for (const FGameplayTag& TmpTag : ItemGameplayTags)
				{
					CategoryStatTags.AddStack(TmpTag, 1);
				}
			}
		}
	}
	//给目录进行排序
	CategoryStatTags.SortMap();
	//将目录数据广播出去
	for (const auto& Tmp : CategoryStatTags.GetTagMap())
	{
		BroadcastCategoryMessage(Tmp.Key, 0, Tmp.Value);
	}
	//日志记录
	UE_LOG(LogSimpleShop,Log,TEXT("UW_ShopView商店目录初始化"));
	//更新商店物品，显示所有物品类型
	UpdateItem(TAG_Item_Type_All);

	//日志记录
	UE_LOG(LogSimpleShop,Log,TEXT("UW_ShopView商店物品更新"));
}

void UUW_ShopView::NativeDestruct()
{
	Super::NativeDestruct();
	if (CategoryOnclickListenerHandle.IsValid())
	{
		CategoryOnclickListenerHandle.Unregister();
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
	//获取ShopSubsystem商店子系统单例 Use ShopSubsystem
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	//1.查找对应品类的物品
	if (const TArray<FItemTable*>* InSlotTableArray = ShopSubsystem.GetSlotTablesTemplate())
	{
		//声明一个临时的数组，用于保存那些拥有指定标签的数据
		TArray<const FItemTable*> InTableArray;
		//遍历数据，如果拥有指定的类型标签，说明就是我们要找的物品，将其添加到临时数组中
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
					if (i & 0x1)//奇数
					{
						GridSlot->SetColumn(1);
					}
					else//偶数
					{
						GridSlot->SetColumn(0);
					}

					GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					//把物品数据传递给格子，从而更新格子里面的物品
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
			DraggedSlot->BroadcastTransactionMessage();
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
	//声明传递数据的目录物品对象
	UItemCategory* Category = NewObject<UItemCategory>(GetOwningPlayerPawn());
	//填充数据：目录标签，该目录类型的物品数量
	Category->SetTag(InTag);
	Category->SetAmount(NewCount);
	//声明目录实例变化消息
	FCategoryInstanceChangeMessage Message;
	//给消息填充数据
	Message.Character = GetOwningPlayerPawn();
	Message.ItemCategory = Category;
	Message.NewNum = NewCount;
	Message.Delta = NewCount - OldCount;
	//获取游戏消息子系统，广播消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Shop_Category_Message, Message);

	//更新目录列表
	if (OldCount == 0) //原来没有=新增
	{
		CategoryList->AddItem(Category);
	}

	if (Message.NewNum == 0) //新的数量为零=删除
	{
		CategoryList->RemoveItem(Category);
	}
}