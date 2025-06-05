//  Copyright CloudHu. All Rights Reserved.
#include "Data/InventoryList.h"
#include "ActorComponents/InventoryManagerActorComponent.h"
#include "Definition/ItemInstance.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Interface/ItemDefinitionInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Message/GlobalNativeTags.h"

//PRAGMA_DISABLE_OPTIMIZATION
TArray<UItemInstance*> FInventoryList::GetAllValidItems() const
{
	TArray<UItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FInventoryEntry& Entry : Entries)
	{
		Results.Add(Entry.Instance);
	}
	return Results;
}

void FInventoryList::UpdateTaggedItems(const FGameplayTag& InTag) const
{
	// Debug::Print(InTag.ToString());
	for (const FInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance->HasTagExact(InTag)) //广播符合条件的物品
		{
			BroadcastChangeMessage(Entry, Entry.StackCount, Entry.StackCount);
		}
		else
		{
			BroadcastChangeMessage(Entry);
		}
	}
}

//PRAGMA_ENABLE_OPTIMIZATION

void FInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

//PRAGMA_DISABLE_OPTIMIZATION
UItemInstance* FInventoryList::AddEntry(const TSubclassOf<UObject> ItemClass, const int32 InItemID, const int32 StackCount)
{
	check(ItemClass != nullptr);
	check(OwnerComponent);

	const AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	//获取一个空格
	FInventoryEntry& NewEntry = GetEmptyEntry(ItemClass, StackCount);
	const int32 OldCount = NewEntry.StackCount;
	NewEntry.Instance->SetItemDef(ItemClass);
	//设置物品编号
	NewEntry.Instance->SetItemID(InItemID);
	NewEntry.StackCount = OldCount + StackCount;

	// 新增 将物品计数添加为GameplayTag，以便可以从UItemInstance中检索
	NewEntry.Instance->AddStatTagStack(TAG_Inventory_Item_Count, StackCount);

	MarkItemDirty(NewEntry);
	//广播消息
	BroadcastChangeMessage(NewEntry, OldCount, NewEntry.StackCount);
	return NewEntry.Instance;
}

//PRAGMA_ENABLE_OPTIMIZATION

void FInventoryList::RemoveEntry(const TSubclassOf<UObject>& ItemClass, const int32 InCount)
{
	int32 Total = InCount;
	//从末尾开始移除
	for (int32 i = Entries.Num() - 1; i > -1; i--)
	{
		if (FInventoryEntry& Entry = Entries[i]; Entry.Instance->GetItemDef() == ItemClass)
		{
			if (Entry.StackCount <= Total) //没有那么多,就分多次移除
			{
				Total -= Entry.StackCount; //减去已经移除的
				ResetEntry(Entry);
				if (Total <= 0)
				{
					MarkArrayDirty();
					break;
				}
			}
			else //比总数多,那么就一次性移除
			{
				Entry.Instance->RemoveStatTagStack(TAG_Inventory_Item_Count, InCount);
				const int32 OldCount = Entry.StackCount;
				Entry.StackCount = OldCount - Total;
				MarkItemDirty(Entry);
				BroadcastChangeMessage(Entry, OldCount, Entry.StackCount);
				break;
			}
		}
	}
}

//PRAGMA_DISABLE_OPTIMIZATION
void FInventoryList::RemoveEntryByIndex(const int32 InInstanceIndex, const int32 InCount)
{
	Debug::Print(FString::Printf(TEXT("--FInventoryList::RemoveEntryByIndex(IndexA：%d, InCount:%d);"), InInstanceIndex, InCount));
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		if (FInventoryEntry& Entry = *EntryIt; Entry.Instance->GetIndex() == InInstanceIndex)
		{
			if (InCount == 0 || Entry.StackCount == InCount) //数量相同的情况移除实例
			{
				ResetEntry(Entry);
			}
			else if (Entry.StackCount > InCount) //库存充足的情况还有剩余
			{
				Entry.Instance->RemoveStatTagStack(TAG_Inventory_Item_Count, InCount);
				const int32 OldCount = Entry.StackCount;
				Entry.StackCount = OldCount - InCount;
				MarkItemDirty(Entry);
				BroadcastChangeMessage(Entry, OldCount, Entry.StackCount);
			}
			else //库存不足的情况还需要移除其他同类型的物品
			{
				const TSubclassOf<UObject> ItemClass = Entry.Instance->GetItemDef();
				ResetEntry(Entry);
				RemoveEntry(ItemClass, InCount - Entry.StackCount);
			}
			break;
		}
	}
}

UItemInstance* FInventoryList::GetItemInstanceByIndex(const int32 InInstanceIndex) //const
{
	UItemInstance* ItemInstance = NewObject<UItemInstance>(OwnerComponent->GetOwner());
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		if (const FInventoryEntry& Entry = *EntryIt; Entry.Instance->GetIndex() == InInstanceIndex)
		{
			ItemInstance->SetItem(Entry.Instance);
			break;
		}
	}
	return ItemInstance;
}

//PRAGMA_ENABLE_OPTIMIZATION

void FInventoryList::SwapEntry(const int32 IndexA, const int32 IndexB, const FGuid GuidA, const FGuid GuidB)
{
	//交换实例索引 ,索引决定实例在UI背包格子中的位置
	// Debug::Print(FString::Printf(TEXT("--FInventoryList::SwapEntry(IndexA：%d, IndexB:%d);"), IndexA, IndexB));
	MoveInstanceToIndex(GuidA, IndexB);
	MoveInstanceToIndex(GuidB, IndexA);
}

void FInventoryList::MoveInstanceToIndex(const FGuid Guid, const int32 Index)
{
	const FInventoryEntry& Entry = FindEntryByGuid(Guid);
	Entry.Instance->SetIndex(Index);
	//广播更新消息
	BroadcastChangeMessage(Entry, Entry.StackCount, Entry.StackCount);
}

bool FInventoryList::CanAddItem(const TSubclassOf<UObject> ItemClass, const int32 StackCount)
{
	check(ItemClass != nullptr);
	check(OwnerComponent);

	const AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	//找到一个空格子的索引
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		const FInventoryEntry& EntryTmp = *EntryIt;
		if (EntryTmp.StackCount == 0)
		{
			return true;
		}

		if (EntryTmp.Instance->GetItemDef() == ItemClass)
		{
			if (const UObject* Obj = GetDefault<UObject>(ItemClass))
			{
				if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
				{
					if (EntryTmp.StackCount + StackCount <= ItemDef->Execute_GetOverlyingAmount(Obj))
					{
						return true;
					}
				}
				else if (Obj->Implements<UItemDefinitionInterface>())
				{
					if (EntryTmp.StackCount + StackCount <= IItemDefinitionInterface::GetOverlyingAmount(Obj))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

int32 FInventoryList::GetItemCapacity(const TSubclassOf<UObject> ItemClass)
{
	int32 Result = 0;

	if (const UObject* Src = GetDefault<UObject>(ItemClass))
	{
		int32 OverlyingNum;
		float ItemGravity;
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Src))
		{
			OverlyingNum = ItemDef->Execute_GetOverlyingAmount(Src); //物品可叠加的数量
			ItemGravity = ItemDef->Execute_GetGravity(Src); //物品重量
		}
		else if (Src->Implements<UItemDefinitionInterface>())
		{
			OverlyingNum = IItemDefinitionInterface::GetOverlyingAmount(Src); //物品可叠加的数量
			ItemGravity = IItemDefinitionInterface::GetGravity(Src); //物品重量
		}
		else
		{
			return Result;
		}

		if (const UInventoryManagerActorComponent* Inventory = Cast<UInventoryManagerActorComponent>(OwnerComponent))
		{
			const float RemainingGravity = Inventory->GetRemainingGravity(); //背包剩余负重
			float TmpGravity = 0; //剩余重量

			for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
			{
				if (TmpGravity >= RemainingGravity) //超重
				{
					break;
				}
				int32 TmpCount = UKismetMathLibrary::FFloor((RemainingGravity - TmpGravity) / ItemGravity);
				if (TmpCount == 0) //剩余负重不足时退出
				{
					break;
				}
				if (const FInventoryEntry& EntryTmp = *EntryIt; EntryTmp.Instance->GetItemDef() == ItemClass && EntryTmp.StackCount < OverlyingNum)
				{
					TmpCount = UKismetMathLibrary::Min(TmpCount, EntryTmp.StackCount);
					Result += TmpCount; //结果增加
					TmpGravity += TmpCount * ItemGravity; //临时重量增加
				}
			}
			for (int i = 0; i < Inventory->GetMaxVolume() - Entries.Num(); ++i)
			{
				if (TmpGravity >= RemainingGravity) //超重
				{
					break;
				}
				int32 TmpCount = UKismetMathLibrary::FFloor((RemainingGravity - TmpGravity) / ItemGravity);
				//临时容量取较小的值
				TmpCount = UKismetMathLibrary::Min(TmpCount, OverlyingNum);
				Result += TmpCount; //结果增加
				TmpGravity += TmpCount * ItemGravity; //临时重量增加
			}
		}
	}

	return Result;
}

FInventoryEntry& FInventoryList::GetEmptyEntry(const TSubclassOf<UObject>& ItemClass, const int32 StackCount)
{
	if (Entries.Num() > 0)
	{
		int32 OverlyingAmount = 1;
		if (const UObject* Obj = GetDefault<UObject>(ItemClass))
		{
			if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				OverlyingAmount = ItemDef->Execute_GetOverlyingAmount(Obj);
			}
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				OverlyingAmount = IItemDefinitionInterface::GetOverlyingAmount(Obj);
			}
		}
		//找到一个通物品类型且容得下的格子
		for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
		{
			if (FInventoryEntry& EntryTmp = *EntryIt; EntryTmp.Instance->GetItemDef() == ItemClass)
			{
				if (EntryTmp.StackCount + StackCount <= OverlyingAmount)
				{
					return EntryTmp;
				}
			}
		}
		//找一个空格子
		for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
		{
			if (FInventoryEntry& EntryTmp = *EntryIt; EntryTmp.StackCount == 0)
			{
				return EntryTmp;
			}
		}
		//找不到时新增一个
		return GetNewEntry();
	}
	return GetNewEntry();
}

FInventoryEntry& FInventoryList::GetNewEntry()
{
	UItemInstance* ItemInstance = NewObject<UItemInstance>(OwnerComponent->GetOwner());
	ItemInstance->SetIndex(Entries.AddDefaulted(1));
	ItemInstance->SetGuid(FGuid::NewGuid());
	Entries.Last().Instance = ItemInstance;
	// Debug::Print(FString::Printf(TEXT("--UFInventoryList::ExpandVolume::GetNewEntry：StackCount:%d;"), Entries.Last().StackCount));
	return Entries.Last();
}

FInventoryEntry& FInventoryList::FindEntryByGuid(const FGuid InGuid)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		if (FInventoryEntry& Entry = *EntryIt; Entry.Instance->GetGuid() == InGuid)
		{
			return Entry;
		}
	}
	Debug::Print(InGuid.ToString() + TEXT("--FInventoryList::FindEntryByGuid:找不到GUID ！！！"), FColor::Red);
	return Entries.Last();
}

int32 FInventoryList::GetInventoryMaxVolume() const
{
	if (const UInventoryManagerActorComponent* Inventory = Cast<UInventoryManagerActorComponent>(OwnerComponent))
	{
		return Inventory->GetMaxVolume();
	}
	return 1;
}

void FInventoryList::ExpandVolume(const int32 InVolume)
{
	const int32 ExpandAmount = InVolume - GetValidDataAmount();
	// Debug::Print(FString::Printf(TEXT("--UFInventoryList::ExpandVolume::ExpandAmount：ExpandAmount:%d;"), ExpandAmount));
	for (int i = 0; i < ExpandAmount; ++i)
	{
		GetNewEntry();
	}
}

void FInventoryList::ResetEntry(FInventoryEntry& Entry) const
{
	Entry.Instance->SetItemID(INDEX_NONE);
	Entry.Instance->RemoveStatTagStack(TAG_Inventory_Item_Count);
	Entry.StackCount = 0;
	BroadcastChangeMessage(Entry);
}

void FInventoryList::BroadcastChangeMessage(const FInventoryEntry& Entry, const int32 OldCount, const int32 NewCount) const
{
	FInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent->GetOwner<APawn>();
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;
	// Debug::Print(Message.Instance->GetItemName().ToString() + FString::Printf(
	// 	TEXT("--BroadcastChangeMessage(ItemInstance--GetIndex：%d, NewCount:%d);"), Message.Instance->GetIndex(), Message.NewCount));
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_Inventory_Message_StackChanged, Message);
}

FString FInventoryEntry::GetDebugString() const
{
	TSubclassOf<UObject> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}
