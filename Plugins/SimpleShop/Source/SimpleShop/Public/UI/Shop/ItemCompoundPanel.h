// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/UserInterfaceMessage.h"
#include "ItemCompoundPanel.generated.h"

class UCanvasPanel;
class UUW_CompoundItem;
struct FItemTable;
typedef uint8 FSlotLayer;
//合成图线条
USTRUCT(BlueprintType)
struct FDrawItemLine
{
	GENERATED_USTRUCT_BODY()

		// | 垂直线
		UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* VerticalLine;

	// _ 水平线
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* HorizontalLine;

	//_|_
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeftAndRightVertically;

	//|-
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnRight;

	//-|
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TurnLeft;

	//T 
	UPROPERTY(EditDefaultsOnly, Category = UI)
		UTexture2D* TLine;
};

/**
 * 合成面板
 */
UCLASS(Blueprintable, BlueprintType)
class SIMPLESHOP_API UItemCompoundPanel : public UUserWidget
{
	GENERATED_BODY()

		//装备面板
		UPROPERTY(meta = (BindWidget))
		UCanvasPanel* EquipmentPanel;

	//起始位置
	UPROPERTY(EditDefaultsOnly, Category = CompoundPanel)
		FVector2D StartPostion;

	//图标尺寸
	UPROPERTY(EditDefaultsOnly, Category = CompoundPanel)
		FVector2D IconSize;

	//物品类
	UPROPERTY(EditDefaultsOnly, Category = CompoundPanel)
		TSubclassOf<UUW_CompoundItem> ItemSlotClass;

	//线条贴图
	UPROPERTY(EditDefaultsOnly, Category = CompoundPanel)
		FDrawItemLine LineTexture;
public:
	UItemCompoundPanel(const FObjectInitializer& ObjectInitializer);//构造函数

	virtual void NativeConstruct() override;

	//根据数据更新显示
	void UpdateSlot(const FItemTable* InTable);

	//交易消息通知
	void OnNotificationCompoundMessage(FGameplayTag Channel, const FUserInterfaceMessage& Notification);
protected:
	//获取深度
	int32 GetLayerDepth(const FItemTable* InTable, int32 InDepth = 1);

	//递归更新
	void RecursiveUpdateSlot(const FItemTable* InTable, const FVector2D& InStartPosition, FSlotLayer InSlotLayer);

	virtual void NativeDestruct() override;
private:
	//监听合成物品的消息
	FGameplayMessageListenerHandle ListenerHandle;
};
