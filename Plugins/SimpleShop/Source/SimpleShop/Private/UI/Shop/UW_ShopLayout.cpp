//  Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/UW_ShopLayout.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/Core/UW_ItemSlotBase.h"
#include "UI/Shop/UW_ShopView.h"
#include "UI/Tips/UW_ConfirmWindow.h"
#include "UI/Inventory/UW_InventoryPanel.h"
#include "UI/Shop/ItemCompoundPanel.h"

UUW_ShopLayout::UUW_ShopLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_ShopLayout::NativeConstruct()
{
	Super::NativeConstruct();

	if (CompoundPanel)
	{
		CompoundPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_ShopLayout::ShowShop(APawn* InOwner, bool bVisible /*= true*/)
{
	if (ShopPtr)
	{
		ShopPtr->SetShopOwner(InOwner);

		if (bVisible)
		{
			ShopPtr->SetVisibility(ESlateVisibility::Visible);
			InventoryPanel->SetVisibility(ESlateVisibility::Visible);
			SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ShopPtr->SetVisibility(ESlateVisibility::Hidden);
			ConfirmWindow->SetVisibility(ESlateVisibility::Hidden);
			CompoundPanel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUW_ShopLayout::ShowInventory(bool bVisible /*= true*/)
{
	if (InventoryPanel)
	{
		if (bVisible)
		{
			InventoryPanel->SetVisibility(ESlateVisibility::Visible);
			SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InventoryPanel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUW_ShopLayout::ChangeShopOwner(APawn* InOwner)
{
	if (ShopPtr)
	{
		ShopPtr->SetShopOwner(InOwner);
	}
	else
	{
		ShowShop(InOwner, false);
	}
}

bool UUW_ShopLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	bool bDrop = false;
	//这里就是放下的操作了
	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (const UUW_ItemSlotBase* DraggedSlot = Cast<UUW_ItemSlotBase>(InDragDropOperation->Payload)) {
			DraggedSlot->ShowIcon(true);
			bDrop = true;
		}
	}
	return bDrop;
}
