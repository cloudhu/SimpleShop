//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/QuickBarComponent.h"

#include "Blueprint/UserWidget.h"

#include "Components/ListView.h"

#include "GameFramework/CH_GameplayMessageSubsystem.h"

#include "UW_QuickBar.generated.h"

class UButton;
/**
 * 快捷栏UI
 */
UCLASS()
class SIMPLESHOP_API UUW_QuickBar : public UUserWidget
{
	GENERATED_BODY()

	FCH_GameplayMessageListenerHandle OnSlotsChangeListenerHandle;
public:
	UUW_QuickBar(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly,Category=QuickBar,meta=(BindWidget))
	UListView* ListView_Slots;

	/**
	 * @brief 扩展按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = QuickBar, meta = (BindWidget))
	UButton* ButtonExpand;
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;

public:

	void OnNotificationSlotsChangeMessage(FGameplayTag Channel, const FQuickBarSlotsChangedMessage& Notification);
	
	UFUNCTION(BlueprintCallable,Category=QuickBar)
	void ExpandQuickBar();

	
};
