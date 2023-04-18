//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "UW_CategoryBase.generated.h"

struct FCategoryClickMessage;
struct FCategoryInstanceChangeMessage;
class UTextBlock;
class UButton;
/**
 * 目录基类
 */
UCLASS(Blueprintable, BlueprintType)
class SIMPLESHOP_API UUW_CategoryBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UUW_CategoryBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = CategoryBase, meta = (BindWidget))
	UButton* ItemButton;

	//显示名称
	UPROPERTY(BlueprintReadOnly, Category = CategoryBase, meta = (BindWidget))
	UTextBlock* DisplayName;

	/** 数量 */
	UPROPERTY(BlueprintReadOnly, Category = CategoryBase, meta = (BindWidget))
	UTextBlock* NumText;

public:
	UFUNCTION(BlueprintCallable,Category=CategoryBase)
	virtual void OnWidgetClicked();
public:
	/** 绑定词缀标签. */
	virtual void BindCategory(const FGameplayTag InTag);

	void SetNumText(const int32 InNum);

	void SetDisplayNameText(const FText& InText) const;

	virtual void OnNumberChangeMessage(FGameplayTag Channel, const FCategoryInstanceChangeMessage& Notification);
protected:
	virtual void NativeDestruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void ResumeCategory(FGameplayTag Channel, const FCategoryClickMessage& Notification);
public:
	/** 标签 */
	UPROPERTY(BlueprintReadOnly, Category = "CategoryBase")
	FGameplayTag Tag;

	//监听句柄
	FCH_GameplayMessageListenerHandle ListenerHandle;

	FCH_GameplayMessageListenerHandle OnClickedListenerHandle;

	UPROPERTY()
	int32 CacheNum;
};
