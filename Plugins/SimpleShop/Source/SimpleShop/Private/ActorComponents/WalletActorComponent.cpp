//  Copyright CloudHu. All Rights Reserved.
#include "ActorComponents/WalletActorComponent.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Message/GlobalNativeTags.h"
#include "Message/TransactionMessage.h"

// Sets default values for this component's properties
UWalletActorComponent::UWalletActorComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, Gold(100000)//测试给与最大值
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


//void UWalletActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	Super::EndPlay(EndPlayReason);
//
//}

bool UWalletActorComponent::Transaction(int32 InPrice, bool bInCome)
{
	int32 OldNum = Gold;
	if (bInCome)//收入
	{
		Gold += InPrice;		
	}
	else//支出
	{
		Gold -= InPrice;		
	}
	BroadcastGoldChangeMessage(OldNum, Gold);
	return true;
}

bool UWalletActorComponent::CanAfford(int32 InPrice)
{
	return Gold >= InPrice;
}

void UWalletActorComponent::BroadcastGoldChangeMessage(int32 OldNum, int32 NewNum)
{
	FWalletChangeMessage Message;
	Message.Character = GetPawnChecked<APawn>();
	Message.OldNum = OldNum;
	Message.NewNum = NewNum;

	UCH_GameplayMessageSubsystem& MessageSystem = UCH_GameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_Wallet_Message_GoldChanged, Message);
}

// Called when the game starts
void UWalletActorComponent::BeginPlay()
{
	Super::BeginPlay();
}