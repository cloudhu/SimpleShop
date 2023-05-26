//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UW_AffixTip.generated.h"

/**
 * @brief 前置声明:词缀数量变化消息结构体
 */
struct FAffixChangeMessage;
class UTextBlock;
/**
 * 词缀提示
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_AffixTip : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * @brief 构造函数
	 * @param ObjectInitializer 对象初始化器
	 */
	UUW_AffixTip(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief 显示名称
	 */
	UPROPERTY(BlueprintReadOnly, Category = AffixTip, meta = (BindWidget))
	UTextBlock* DisplayName;

	/**
	 * @brief 数值文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = AffixTip, meta = (BindWidget))
	UTextBlock* NumText;

	/**
	 * @brief 全局唯一编号
	 */
	UPROPERTY(BlueprintReadOnly, Category=AffixTip)
	FGuid Guid;
public:
	/**
	 * @brief 绑定词缀标签
	 * @param Tag 游戏标签
	 */
	void BindAffixTip(FGameplayTag Tag);

	/**
	 * @brief 设置数值文本
	 * @param InNum 数值
	 */
	void SetNumText(int32 InNum);

	/**
	 * @brief 设置显示名称文本
	 * @param InText 文本
	 */
	void SetDisplayNameText(const FText& InText) const;

	/**
	 * @brief 消息通知
	 * @param Channel 消息频道 
	 * @param Notification 消息
	 */
	void OnAffixChangeMessage(FGameplayTag Channel, const FAffixChangeMessage& Notification);
protected:
	/**
	 * @brief 析构函数
	 */
	virtual void NativeDestruct() override;

	/**
	 * @brief IUserObjectListEntry接口函数实现
	 * @param ListItemObject 列表物品对象
	 */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	/**
	 * @brief 词缀变化监听句柄
	 */
	FGameplayMessageListenerHandle ListenerHandle;
};