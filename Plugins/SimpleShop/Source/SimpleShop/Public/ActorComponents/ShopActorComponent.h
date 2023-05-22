// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ActorComponentBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/TransactionMessage.h"
#include "ShopActorComponent.generated.h"

class UUW_ShopLayout;
class UUW_ConfirmWindow;
//struct FItemTable;
UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class SIMPLESHOP_API UShopActorComponent : public UActorComponentBase
{
	GENERATED_BODY()

		/// <summary>
		/// 商店布局类
		/// </summary>
		UPROPERTY(EditDefaultsOnly, Category = Shop)
		TSubclassOf<UUW_ShopLayout> ShopLayoutClass;
public:
	// Sets default values for this component's properties
	UShopActorComponent(const FObjectInitializer& ObjectInitializer);

	//交易消息通知
	void OnNotificationTransactionMessage(FGameplayTag Channel, const FTransactionMessage& Notification);

	void ConfirmAction(const int32 Count) const;

	void CancelAction() const;

	UFUNCTION(BlueprintPure, Category = "Shop")
		static UShopActorComponent* FindShopActorComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UShopActorComponent>() : nullptr); }

public:
	//显示商店UI
	UFUNCTION(BlueprintCallable, Category = "Shop")
		void ShowShop(APawn* InOwner, bool bVisible = true);

	/// <summary>
	/// 变更商店所有者
	/// </summary>
	/// <param name="InOwner">店主</param>
	UFUNCTION(BlueprintCallable, Category = "Shop")
		void ChangeShopOwner(APawn* InOwner);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void ShowInventory(bool bVisible = true);
private:
	//商店UI指针缓存
	UPROPERTY()
		UUW_ShopLayout* ShopLayoutPtr;

	UPROPERTY()
		UUW_ConfirmWindow* ConfirmWindow;

	//交易监听句柄
	FGameplayMessageListenerHandle TransactionListenerHandle;

	FTransactionMessage CacheNotification;
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
