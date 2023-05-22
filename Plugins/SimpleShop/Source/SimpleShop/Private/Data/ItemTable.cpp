#include "Data/ItemTable.h"
#include "Message/GlobalNativeTags.h"

/**
 * @brief 在构造函数中初始化默认数值
 */
FItemTable::FItemTable()
	: ID(INDEX_NONE)
	  , Price(100)
	  , Stock(100)
{
}

/**
 * @brief 物品编号只要不是-1都是有效的
 * @return 是否有效
 */
bool FItemTable::IsValid() const
{
	return ID != INDEX_NONE;
}

/**
 * @brief 根据物品的标签判断
 * @return 是否是消耗品类型
 */
bool FItemTable::IsConsumables() const
{
	return ItemTypeHasTag(TAG_Item_Type_Consumables);
}

/**
 * @brief 判断物品是否有指定标签
 * @param Tag 指定标签
 * @return 是否有该标签
 */
bool FItemTable::ItemTypeHasTag(const FGameplayTag& Tag) const
{
	//这里的判断涉及到UE C++接口的机制,后面的章节会进行更加详细的解释
	//首先CDO表示物品类默认对象,在UE C++类中,只要父类层级继承自UObject,都会有一个类默认对象CDO
	//CDO是UE C++中的类反射机制,可以通过该对象来进行类型转换
	if (const UObject* ItemCDO = GetDefault<UObject>(ItemDefinition))
	{
		//映射转换成接口,然后通过接口的函数来判断是否拥有特定的标签
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(ItemCDO))
		{
			return ItemDef->Execute_GetTypeTags(ItemCDO).HasTagExact(Tag);
		}//在映射失败的情况下判断该对象是否实现了该接口,直接通过接口来调用判断函数
		else if (ItemCDO->Implements<UItemDefinitionInterface>())
		{
			return  IItemDefinitionInterface::GetTypeTags(ItemCDO).HasTagExact(Tag);
		}
	}
	//如果该对象没有实现接口,则直接返回false否定
	return false;
}