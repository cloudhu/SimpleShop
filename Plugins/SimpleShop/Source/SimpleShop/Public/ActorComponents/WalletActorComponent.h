//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/ActorComponentBase.h"
#include "WalletActorComponent.generated.h"

//角色的钱包
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIMPLESHOP_API UWalletActorComponent : public UActorComponentBase
{
	GENERATED_BODY()

private:
	//货币
	UPROPERTY()
		int32 Gold;
public:
	// Sets default values for this component's properties
	UWalletActorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Shop")
		static UWalletActorComponent* FindWalletActorComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UWalletActorComponent>() : nullptr); }


	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//交易的数目
	bool Transaction(int32 InPrice, bool bInCome = true);

	//是否可以支付
	bool CanAfford(int32 InPrice);

	/// <summary>
	/// 广播货币变化
	/// </summary>
	/// <param name="OldNum">之前的数量</param>
	/// <param name="NewNum">新的数量</param>
	void BroadcastGoldChangeMessage(int32 OldNum,int32 NewNum);

public:
	/// <summary>
	/// 获取当前货币数量
	/// </summary>
	/// <returns>货币数量</returns>
	UFUNCTION(BlueprintCallable,BlueprintPure,Category=Wallet)
	int32 GetCurrentGoldNum() { return Gold; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
