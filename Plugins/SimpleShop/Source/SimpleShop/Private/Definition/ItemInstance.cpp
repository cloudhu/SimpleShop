//  Copyright CloudHu. All Rights Reserved.
#include "Definition/ItemInstance.h"
#include "Net/UnrealNetwork.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
}

//数据同步
void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemID);
	DOREPLIFETIME(ThisClass, Index);
	DOREPLIFETIME(ThisClass, Guid);
}

void UItemInstance::AddStatTagStack(const FGameplayTag Tag, const int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UItemInstance::RemoveStatTagStack(const FGameplayTag Tag, const int32 StackCount)
{
	int32 Count = StackCount;
	if (Count == 0)
	{
		Count = StatTags.GetStackCount(Tag);
	}
	StatTags.RemoveStack(Tag, Count);
}

int32 UItemInstance::GetStatTagStackCount(const FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UItemInstance::HasTagExact(const FGameplayTag Tag) const
{
	return Tags.HasTagExact(Tag);
}

bool UItemInstance::HasStatTagExact(const FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

FText UItemInstance::GetItemName() const
{
	if (ItemDef != nullptr)
	{
		if (const UObject* Obj = GetDefault<UObject>(ItemDef))
		{
			if (const IItemDefinitionInterface* IItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				return IItemDef->Execute_GetDisplayName(Obj);
			}
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				return IItemDefinitionInterface::GetDisplayName(Obj);
			}
		}
	}

	return FText::GetEmpty();
}

FSlateBrush UItemInstance::GetIconBrush() const
{
	if (ItemDef != nullptr)
	{
		if (const UObject* Obj = GetDefault<UObject>(ItemDef))
		{
			if (const IItemDefinitionInterface* IItemDef = Cast<IItemDefinitionInterface>(Obj))
			{
				return IItemDef->Execute_GetIconBrush(Obj);
			}
			else if (Obj->Implements<UItemDefinitionInterface>())
			{
				return IItemDefinitionInterface::GetIconBrush(Obj);
			}
		}
	}

	return FSlateBrush();
}

void UItemInstance::SetItemID(const int32 InID)
{
	ItemID = InID;
}

void UItemInstance::SetIndex(const int32 InIndex)
{
	Index = InIndex;
}

void UItemInstance::SetGuid(const FGuid InGuid)
{
	Guid = InGuid;
}

void UItemInstance::SetItemDef(const TSubclassOf<UObject> InDef)
{
	ItemDef = InDef;
	if (ItemDef != nullptr)
	{
		if (const UObject* ItemCDO = GetDefault<UObject>(ItemDef))
		{
			if (const IItemDefinitionInterface* ItemDefinition = Cast<IItemDefinitionInterface>(ItemCDO))
			{
				Tags = ItemDefinition->Execute_GetTypeTags(ItemCDO);
			}
			else if (ItemCDO->Implements<UItemDefinitionInterface>())
			{
				Tags = IItemDefinitionInterface::GetTypeTags(ItemCDO);
			}
		}
	}
	else
	{
		Tags = FGameplayTagContainer::EmptyContainer;
	}
}
