#include "Data/ItemTable.h"
#include "Message/GlobalNativeTags.h"

FItemTable::FItemTable()
	: ID(INDEX_NONE)
	  , Price(100)
	  , Stock(100)
{
}

bool FItemTable::IsValid() const
{
	return ID != INDEX_NONE;
}

bool FItemTable::IsConsumables() const
{
	return ItemTypeHasTag(TAG_Item_Type_Consumables);
}

bool FItemTable::ItemTypeHasTag(const FGameplayTag& Tag) const
{
	if (const UObject* ItemCDO = GetDefault<UObject>(ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(ItemCDO))
		{
			return ItemDef->Execute_GetTypeTags(ItemCDO).HasTagExact(Tag);
		}
		else if (ItemCDO->Implements<UItemDefinitionInterface>())
		{
			return  IItemDefinitionInterface::GetTypeTags(ItemCDO).HasTagExact(Tag);
		}
	}
	return false;
}
