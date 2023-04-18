//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UW_UpgradeConfirm.generated.h"

class UTextBlock;
/**
 * 升级确认
 */
UCLASS()
class SIMPLESHOP_API UUW_UpgradeConfirm : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	* @brief 当前等级文本
	*/
	UPROPERTY(BlueprintReadOnly, Category = UpgradeConfirm, meta = (BindWidget))
	UTextBlock* CurrentLevel;

	/**
	 * @brief 下一级文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = UpgradeConfirm, meta = (BindWidget))
	UTextBlock* NextLevel;

	/**
	 * @brief 价格文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = UpgradeConfirm, meta = (BindWidget))
	UTextBlock* PriceText;

	/**
	 * @brief 确认按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = UpgradeConfirm, meta = (BindWidget))
	UButton* ConfirmButton;

	/**
	 * @brief 取消按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = UpgradeConfirm, meta = (BindWidget))
	UButton* CancelButton;

	/**
	 * @brief 确认代理
	 */
	FSimpleDelegate ConfirmDelegate;
	
	UFUNCTION(BlueprintCallable,Category=UpgradeConfirm)
	void ConfirmAction();
	
	UFUNCTION(BlueprintCallable,Category=UpgradeConfirm)
	void CancelAction();

	virtual void NativeConstruct() override;

	void SetCurrentLevel(const int32 InLevel) const;
	
	void SetPrice(const int32 InCost) const;
};
