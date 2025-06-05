//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "UW_ConfirmWindow.generated.h"

struct FUserInterfaceMessage;
/// <summary>
/// 确认数量
/// </summary>
DECLARE_DELEGATE_OneParam(FConfirmActionDelegate, int32);
class USlider;
class UTextBlock;
class UButton;
/**
 * 交易确认窗口
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ConfirmWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ConfirmWindow(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//标题
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UTextBlock* TitleText;

	//显示名称
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UTextBlock* ItemName;

	//数量滑动条
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	USlider* CountSlider;

	//价格
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UTextBlock* PriceText;

	//价格
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UTextBlock* TextMaxCount;

	/// <summary>
	/// 确认按钮
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UButton* ConfirmButton;

	/// <summary>
	/// 取消按钮
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = ConfirmWindow, meta = (BindWidget))
	UButton* CancelButton;

	FConfirmActionDelegate ConfirmActionDelegate;
	FSimpleDelegate CanceledDelegate;

	/// <summary>
	/// 确认动作
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "ConfirmWindow")
	void ConfirmAction();


	/** 取消动作. */
	UFUNCTION(BlueprintCallable, Category = "ConfirmWindow")
	void CancelAction();

	/** 设置总价. */
	UFUNCTION(BlueprintCallable, Category = "ConfirmWindow")
	void SetPriceFromSlider(int32 Value);

	/// <summary>
	/// 设置数量滑动条的最大值
	/// </summary>
	/// <param name="MaxAmount">最大数</param>
	void SetSliderAmount(int32 MaxAmount) const;

	void SetTitle(const FText& InText) const;

	void SetItemName(const FText& InText) const;

	void SetPrice(const FText& InText) const;

	void SetCachePrice(const int32 InPrice);

	//交易消息通知
	void OnNotificationCompoundMessage(FGameplayTag Channel, const FUserInterfaceMessage& Notification);
private:
	int32 CachePrice = 0;
	
	//监听消息
	FGameplayMessageListenerHandle ListenerHandle;
};
