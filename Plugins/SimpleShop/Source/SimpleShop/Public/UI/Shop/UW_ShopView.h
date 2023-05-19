// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ListView.h"

#include "Data/ItemGameplayTagStack.h"

#include "UI/Core/UW_ShopUIBase.h"
#include "UW_ShopView.generated.h"

class UItemCategory;
struct FCategoryClickMessage;
class UCheckBox;
class UUniformGridPanel;
class UUW_ShopItem;
/**
 * 商店UI展示
 */
UCLASS(Abstract, Blueprintable)
class SIMPLESHOP_API UUW_ShopView : public UUW_ShopUIBase
{
	GENERATED_BODY()
public:
	UUW_ShopView(const FObjectInitializer& ObjectInitializer); //构造函数

	/**
	 * @brief 商店目录列表
	 */
	UPROPERTY(BlueprintReadOnly, Category = ShopView, meta = (BindWidget))
	UListView* CategoryList;

	UPROPERTY(BlueprintReadOnly, Category = ShopView, meta = (BindWidget))
	UUniformGridPanel* ItemGrid; //物品列表

	UPROPERTY(EditDefaultsOnly, Category = ShopView)
	TSubclassOf<UUW_ShopItem> ItemClass; //物品类

	void OnCategoryClicked(FGameplayTag Channel, const FCategoryClickMessage& Notification);
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//设置商店的所有者,可能是玩家,可能是NPC, nullptr 就算是系统
	void SetShopOwner(APawn* InShopOwner);
	
	void UpdateItem(const FGameplayTag& TypeTag) const; //更新物品列表
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	UFUNCTION(BlueprintCallable, Category = Shop)
	void OnCloseWidget(); //关闭

	/// <summary>
	/// 关闭合成面板
	/// </summary>
	void CloseCompoundPanel() const;

	void BroadcastCategoryMessage(const FGameplayTag InTag, const int32 OldCount, const int32 NewCount) const;
private:

	//商店所有者
	UPROPERTY()
	APawn* ShopOwner = nullptr;

	//监听句柄
	FCH_GameplayMessageListenerHandle ListenerHandle;

	/** 目录标签  */
	UPROPERTY()
	FItemGameplayTagStackContainer CategoryStatTags;
};
