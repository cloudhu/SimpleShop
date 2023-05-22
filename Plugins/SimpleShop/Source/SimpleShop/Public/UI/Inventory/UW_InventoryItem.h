//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "UI/Core/UW_ItemSlotBase.h"
#include "UW_InventoryItem.generated.h"

class UUW_DragDropIcon;
/**
 * 背包物品
 */
UCLASS()
class SIMPLESHOP_API UUW_InventoryItem : public UUW_ItemSlotBase
{
	GENERATED_BODY()
public:
		//生成的拖拽图标
		UPROPERTY(EditDefaultsOnly, Category = InventorySlot)
		TSubclassOf<UUW_DragDropIcon> DragDropIconClass;
public:
	/// <summary>
	/// 背包物品被点击时是出售
	/// </summary>
	virtual void OnClickedWidget() override;

	void OnItemStackChanged(FGameplayTag Chanel,const FInventoryChangeMessage& Message);
protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	//下面的都是UI的原生事件,根据名称就知道是什么事件了
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//检测到背包格子被拖拽
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	//检测到背包格子被放下
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	//监听句柄
	FGameplayMessageListenerHandle ListenerHandle;
};
