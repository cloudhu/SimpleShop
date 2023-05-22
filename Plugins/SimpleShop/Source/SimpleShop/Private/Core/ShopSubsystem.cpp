// Copyright CloudHu. All Rights Reserved.
#include "Core/ShopSubsystem.h"
#include "ActorComponents/QuickBarComponent.h"
#include "Definition/ItemInstance.h"
#include "Engine/Engine.h"
#include "Message/GlobalNativeTags.h"

UShopSubsystem::UShopSubsystem()
{
	//读取配置表 0.商品 DataTable'/SimpleShop/Data/DT_ShopItems.DT_ShopItems'
	static ConstructorHelpers::FObjectFinder<UDataTable> Slot_Table(TEXT("/SimpleShop/Data/DT_ShopItems"));
	SlotTablePtr = Slot_Table.Object;

	//1.背包格子 DataTable'/SimpleShop/Data/DT_InventoryConfigs.DT_InventoryConfigs'
	static ConstructorHelpers::FObjectFinder<UDataTable> Inventory_Table(TEXT("/SimpleShop/Data/DT_InventoryConfigs"));
	InventoryTablePtr = Inventory_Table.Object;

	//2.快捷栏 DataTable'/SimpleShop/Data/DT_QuickBarConfigs.DT_QuickBarConfigs'
	static ConstructorHelpers::FObjectFinder<UDataTable> QuickBar_Table(TEXT("/SimpleShop/Data/DT_QuickBarConfigs"));
	QuickBarTablePtr = QuickBar_Table.Object;
}

UShopSubsystem& UShopSubsystem::Get(const UObject* WorldContextObject)
{
	//标准的游戏实例子系统的单例获取方法范例
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);
	UShopSubsystem* Router = UGameInstance::GetSubsystem<UShopSubsystem>(World->GetGameInstance());
	check(Router);
	return *Router;
}

//Debug的时候用该命令来防止IDE做代码优化,从而无法查看调试信息
//PRAGMA_DISABLE_OPTIMIZATION
void UShopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//缓存物品数据
	GetSlotTablesTemplate();
	//日志记录
	UE_LOG(LogSimpleShop,Log,TEXT("UShopSubsystem已经初始化"));
	// 监听交易消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	TransactionListenerHandle = MessageSystem.RegisterListener(TAG_ConfirmedTransaction_Message, this, &ThisClass::OnNotificationTransactionMessage);
}
//PRAGMA_ENABLE_OPTIMIZATION

const TArray<FItemTable*>* UShopSubsystem::GetSlotTablesTemplate()
{
	//首先判断有没有缓存数据,如果有则直接返回缓存的数据,避免重复读取
	if (!CacheSlotTables.Num())
	{
		if (SlotTablePtr)//没有缓存数据则从数据表指针中读取到缓存数据中
		{
			SlotTablePtr->GetAllRows(TEXT("Slot Tables"), CacheSlotTables);
		}
	}

	return &CacheSlotTables;
}

const TArray<FInventoryConfiguration*>* UShopSubsystem::GetInventoryConfigurations()
{
	if (!InventoryTables.Num())
	{
		if (InventoryTablePtr)
		{
			InventoryTablePtr->GetAllRows(TEXT("Inventory Tables"), InventoryTables);
		}
	}

	return &InventoryTables;
}

const TArray<FQuickBarConfiguration*>* UShopSubsystem::GetQuickBarConfigurations()
{
	if (!QuickBarConfigurations.Num())
	{
		if (QuickBarTablePtr)
		{
			QuickBarTablePtr->GetAllRows(TEXT("QuickBar Tables"), QuickBarConfigurations);
		}
	}

	return &QuickBarConfigurations;
}

const FItemTable* UShopSubsystem::GetSlotTableByID(const int32 InID)
{
	//遍历物品数据,找到对应编号的物品数据
	for (auto& Tmp : *GetSlotTablesTemplate())
	{
		if (Tmp->ID == InID)
		{
			return Tmp;
		}
	}

	return nullptr;
}

const FInventoryConfiguration* UShopSubsystem::GetInventoryConfigurationByLevel(const int32 InLevel)
{
	for (const FInventoryConfiguration* Tmp : *GetInventoryConfigurations())
	{
		if (Tmp->Level == InLevel)
		{
			return Tmp;
		}
	}

	return nullptr;
}

const FQuickBarConfiguration* UShopSubsystem::GetQuickBarConfigurationByLevel(const int32 InLevel)
{
	for (const FQuickBarConfiguration* Tmp : *GetQuickBarConfigurations())
	{
		if (Tmp->Level == InLevel)
		{
			return Tmp;
		}
	}

	return nullptr;
}

void UShopSubsystem::Deinitialize()
{
	Super::Deinitialize();
	//取消监听
	if (TransactionListenerHandle.IsValid())
	{
		TransactionListenerHandle.Unregister();
	}
}

void UShopSubsystem::OnNotificationTransactionMessage(FGameplayTag Channel, const FConfirmedTransactionMessage& Notification)
{
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	FTransactionMessageResult ResultMessage;
	ResultMessage.Buyer = Notification.Buyer;
	ResultMessage.Seller = Notification.Seller;
	ResultMessage.ItemID = Notification.ItemID;
	ResultMessage.Amount = Notification.Count;

	if (UWalletActorComponent* BuyerWallet = FindWalletActorComponent(Notification.Buyer)) //找到买家
	{
		int32 Cost = Notification.Price; //Todo:这些在服务器上进行
		if (Notification.bIsCompoundItem)
		{
			Cost = CalculateCompoundItemCost(GetSlotTableByID(Notification.ItemID), Notification.Buyer, true);
		}

		if (BuyerWallet->CanAfford(Cost)) //从买家钱包进行支出
		{
			BuyerWallet->Transaction(Notification.Price, false);
			//交易消息:成功
			ResultMessage.bSuccess = true;

			//买家支付成功之后就应该得到物品
			if (UInventoryManagerActorComponent* InventoryManager = FindInventoryManagerActorComponent(Notification.Buyer))
			{
				InventoryManager->AddItemDefinition(GetSlotTableByID(Notification.ItemID)->ItemDefinition, Notification.ItemID, Notification.Count);
			}
		}
		else
		{
			//交易消息:失败
			ResultMessage.bSuccess = false;
		}
	}

	if (Notification.bIsQuickBarItem)
	{
		if (UQuickBarComponent* QuickBar = UQuickBarComponent::FindQuickBarComponent(Notification.Seller))
		{
			QuickBar->RemoveItemFromSlot(Notification.InstanceIndex);
		}
	}
	else
	{
		//从卖家的背包中移除物品
		if (UInventoryManagerActorComponent* InventoryManager = FindInventoryManagerActorComponent(Notification.Seller))
		{
			InventoryManager->RemoveItemByIndex(Notification.InstanceIndex, Notification.Count);
		}
	}

	if (UWalletActorComponent* SellerWallet = FindWalletActorComponent(Notification.Seller)) //找到卖家
	{
		SellerWallet->Transaction(Notification.Price); //给卖家钱包进行收入
		//交易消息:成功
		ResultMessage.bSuccess = true;
	}
	MessageSystem.BroadcastMessage(TAG_Transaction_Message_Result, ResultMessage);
}

int32 UShopSubsystem::CalculateCompoundItemCost(const FItemTable* InTable, const APawn* InPawn, bool bIsServer /*= false*/)
{
	int32 Cost = 99999;
	if (!InTable)
	{
		return Cost;
	}

	//原本消耗的数量
	Cost = InTable->Price;

	if (InTable->IsConsumables())
	{
		return Cost; //如果是消耗品,直接返回原价
	}

	//I	 递归获取合成所需的基础物品编号
	TArray<int32> ItemIds;
	RecursivelyGetItemID(InTable, ItemIds);

	//II  获取已经购买了的合成所需的物品数据
	if (UInventoryManagerActorComponent* InventoryComponent = FindInventoryManagerActorComponent(InPawn))
	{
		TArray<UItemInstance*> InventoryItems = InventoryComponent->GetAllValidItems(); //背包中的物品
		TArray<int32> RemoveItems; //需要移除的物品
		for (const int32 ItemID : ItemIds)
		{
			for (const UItemInstance* Instance : InventoryItems)
			{
				if (!RemoveItems.Contains(Instance->GetIndex()))
				{
					if (ItemID == Instance->GetItemID())
					{
						if (const FItemTable* Table = GetSlotTableByID(Instance->GetItemID()))
						{
							Cost -= Table->Price; //III 减去已经购买了的
							RemoveItems.Add(Instance->GetIndex());
							break;
						}
					}
				}
			}
		}

		//服务器上执行
#if WITH_SERVER_CODE
		//I  移除物品子项
		if (bIsServer) //加这个判断的原因是在编辑器运行时,编辑器既是客户端,又是权威
		{
			for (const int32 SlotIndex : RemoveItems)
			{
				InventoryComponent->RemoveItemByIndex(SlotIndex);
			}
		}
#endif
	}
	return Cost;
}

void UShopSubsystem::RecursivelyGetItemID(const FItemTable* InTable, TArray<int32>& ItemIds)
{
	if (const UObject* ItemCDO = GetDefault<UObject>(InTable->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(ItemCDO))
		{
			if (ItemDef->Execute_GetChildrenIds(ItemCDO).Num() > 0)
			{
				for (auto& Tmp : ItemDef->Execute_GetChildrenIds(ItemCDO))
				{
					if (const FItemTable* ChildTable = GetSlotTableByID(Tmp))
					{
						ItemIds.Add(ChildTable->ID);
						RecursivelyGetItemID(ChildTable, ItemIds);
					}
				}
			}
		}
		else if (ItemCDO->Implements<UItemDefinitionInterface>())
		{
			if (IItemDefinitionInterface::GetChildrenIds(ItemCDO).Num() > 0)
			{
				for (const auto& Tmp : IItemDefinitionInterface::GetChildrenIds(ItemCDO))
				{
					if (const FItemTable* ChildTable = GetSlotTableByID(Tmp))
					{
						ItemIds.Add(ChildTable->ID);
						RecursivelyGetItemID(ChildTable, ItemIds);
					}
				}
			}
		}
	}
}