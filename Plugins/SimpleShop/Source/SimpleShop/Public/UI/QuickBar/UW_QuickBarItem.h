//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/QuickBarComponent.h"

#include "UI/Core/UW_ItemSlotBase.h"
#include "UW_QuickBarItem.generated.h"
class UUW_DragDropIcon;
/**
 * 快捷栏物品
 */
UCLASS(Blueprintable,BlueprintType)
class SIMPLESHOP_API UUW_QuickBarItem : public UUW_ItemSlotBase
{
	GENERATED_BODY()

public:
	//生成的拖拽图标
	UPROPERTY(EditDefaultsOnly, Category = QuickBarItem)
	TSubclassOf<UUW_DragDropIcon> DragDropIconClass;

	/**
	 * @brief 快捷键UI显示
	 */
	UPROPERTY(BlueprintReadOnly, Category=QuickBarItem, meta=(BindWidget))
	UTextBlock* QuickKey;

	UPROPERTY(BlueprintReadOnly, Category=QuickBarItem, meta=(BindWidget))
	UImage* OnChosen;

	/// <summary>
	/// 背包物品被点击时是出售
	/// </summary>
	virtual void OnClickedWidget() override;

	void SetQuickKeyText(const int32 InNum) const;
	
	virtual bool IsQuickBarItem()const override;
	/**
	 * @brief 快捷栏格子变化处理
	 * @param Channel 频道
	 * @param Notification 消息数据
	 */
	void OnNotificationSlotChangeMessage(FGameplayTag Channel, const FSlotChangedMessage& Notification);

	/**
	 * @brief 快捷栏格子激活变化处理
	 * @param Channel 频道
	 * @param Notification 消息数据
	 */
	void OnNotificationActiveIndexChangeMessage(FGameplayTag Channel, const FQuickBarActiveIndexChangedMessage& Notification);
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	//下面的都是UI的原生事件,根据名称就知道是什么事件了
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//检测到背包格子被拖拽
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	//检测到背包格子被放下
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
private:
	/**
	 * @brief 监听单独的格子发生变化
	 */
	FGameplayMessageListenerHandle OneSlotChangeHandle;
	
	/**
	 * @brief 激活的索引变化监听句柄
	 */
	FGameplayMessageListenerHandle ActiveIndexChangeHandle;
};
