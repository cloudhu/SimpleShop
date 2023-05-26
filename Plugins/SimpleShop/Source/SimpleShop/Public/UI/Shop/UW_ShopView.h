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
	 * @brief 物品目录列表
	 */
	UPROPERTY(BlueprintReadOnly, Category = ShopView, meta = (BindWidget))
	UListView* CategoryList;

	/**
	 * @brief 物品列表
	 */
	UPROPERTY(BlueprintReadOnly, Category = ShopView, meta = (BindWidget))
	UUniformGridPanel* ItemGrid;

	/**
	 * @brief 物品类
	 */
	UPROPERTY(EditDefaultsOnly, Category = ShopView)
	TSubclassOf<UUW_ShopItem> ItemClass;
public:
	/**
	 * @brief 当收到物品分类目录被点击消息时调用
	 * @param Channel 消息频道
	 * @param Notification 消息
	 */
	void OnCategoryClicked(FGameplayTag Channel, const FCategoryClickMessage& Notification);

	/**
	 * @brief UI原生构造
	 */
	virtual void NativeConstruct() override;

	/**
	 * @brief UI原生析构
	 */
	virtual void NativeDestruct() override;

	/**
	 * @brief 设置商店的所有者,可能是玩家,可能是NPC, nullptr 就算是系统
	 * @param InShopOwner 所有者
	 */
	void SetShopOwner(APawn* InShopOwner);

	/**
	 * @brief 更新物品列表
	 * @param TypeTag 物品类型
	 */
	void UpdateItem(const FGameplayTag& TypeTag) const; 
protected:
	/**
	 * @brief 当物品被放置时调用
	 * @param InGeometry 图像
	 * @param InDragDropEvent 拖放事件
	 * @param InOperation 操作
	 * @return 
	 */
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/**
	 * @brief 关闭商店界面
	 */
	UFUNCTION(BlueprintCallable, Category = Shop)
	void OnCloseWidget(); 

	/// <summary>
	/// 关闭合成面板
	/// </summary>
	void CloseCompoundPanel() const;

	/**
	 * @brief 广播目录消息
	 * @param InTag 标签
	 * @param OldCount 原来的数量
	 * @param NewCount 新的数量
	 */
	void BroadcastCategoryMessage(const FGameplayTag InTag, const int32 OldCount, const int32 NewCount) const;
private:
	/**
	 * @brief 商店所有者
	 */
	UPROPERTY()
	APawn* ShopOwner = nullptr;

	/**
	 * @brief 物品目录点击消息监听句柄
	 */
	FGameplayMessageListenerHandle CategoryOnclickListenerHandle;

	/**
	 * @brief 目录标签
	 */
	UPROPERTY()
	FItemGameplayTagStackContainer CategoryStatTags;
};
