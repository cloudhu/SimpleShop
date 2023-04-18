//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ShopLayout.generated.h"

class UUW_QuickBar;
class UUW_ShopView;
class UUW_ConfirmWindow;
class UUW_InventoryPanel;
class UItemCompoundPanel;
/**
 *UI布局
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ShopLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ShopLayout(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadOnly, Category = ShopLayout, meta = (BindWidget))
		UUW_ShopView* ShopPtr;//商店面板

	UPROPERTY(BlueprintReadOnly, Category = ShopLayout, meta = (BindWidget))
		UUW_ConfirmWindow* ConfirmWindow;//确认窗口

	UPROPERTY(BlueprintReadOnly, Category = ShopLayout, meta = (BindWidget))
		UUW_InventoryPanel* InventoryPanel;//背包面板

	UPROPERTY(BlueprintReadOnly, Category = ShopLayout, meta = (BindWidget))
		UUW_QuickBar* QuickBar;//快捷栏

	UPROPERTY(BlueprintReadOnly, Category = ShopLayout, meta = (BindWidget))
		UItemCompoundPanel* CompoundPanel;

public:
	void ShowShop(APawn* InOwner, bool bVisible = true);

	void ShowInventory(bool bVisible = true);

	void ChangeShopOwner(APawn* InOwner);

	UUW_ConfirmWindow* GetConfirmWindow() { return ConfirmWindow; }
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
