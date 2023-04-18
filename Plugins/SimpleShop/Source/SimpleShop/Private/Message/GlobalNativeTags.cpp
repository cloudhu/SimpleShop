// Copyright Epic Games, Inc. All Rights Reserved.
#include "Message/GlobalNativeTags.h"

//所有物品
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Basic, "Item.Type.Basic");//基础
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Epic, "Item.Type.Epic");//史诗
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Legendary, "Item.Type.Legendary");//传奇
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Mythic, "Item.Type.Mythic");//神话
//辅助品
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_AuxiliaryProps, "Item.Type.AuxiliaryProps");
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_PhysicalAttack, "Item.Type.PhysicalAttack");//物理攻击
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_MagicAttack, "Item.Type.MagicAttack");//魔法攻击
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_PhysicalPenetration, "Item.Type.PhysicalPenetration");//物理穿透
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_MagicPenetration, "Item.Type.MagicPenetration");//魔法穿透
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Crit, "Item.Type.Crit");//暴击
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Armor, "Item.Type.Armor");//护甲
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_MagicResistance, "Item.Type.MagicResistance");//魔法抗性
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_AttackSpeed, "Item.Type.AttackSpeed");//攻击速度
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Shoes, "Item.Type.Shoes");//鞋子

//可合成的物品
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Compoundable, "Item.Type.Compoundable");

//合成物品
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Message_Compound, "Item.Message.Compound");

//物品发生改变时发消息通知
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Message_StackChanged, "Inventory.Message.StackChanged");
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Expanded_Message, "Inventory.Message.Expanded");
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Gravity_Message, "Inventory.Message.Gravity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_MaxGravity_Message, "Inventory.Message.MaxGravity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Category_Message, "Inventory.Message.Category");
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_CategoryOnClick_Message, "Inventory.Message.CategoryClick");
UE_DEFINE_GAMEPLAY_TAG(TAG_Shop_Category_Message, "Shop.Message.Category");
UE_DEFINE_GAMEPLAY_TAG(TAG_Shop_CategoryOnClick_Message, "Shop.Message.CategoryClick");
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_All, "Item.Type.All");
//物品数量
UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Item_Count, "Inventory.Item.Count");

UE_DEFINE_GAMEPLAY_TAG(TAG_ConfirmedTransaction_Message, "Transaction.ConfirmedTransaction");
UE_DEFINE_GAMEPLAY_TAG(TAG_Transaction_Message, "Transaction.Message");
UE_DEFINE_GAMEPLAY_TAG(TAG_Transaction_Message_Result, "Transaction.MessageResult");

UE_DEFINE_GAMEPLAY_TAG(TAG_Item_Type_Consumables, "Item.Type.Consumables");//消耗品

//金钱发生改变时发消息通知
UE_DEFINE_GAMEPLAY_TAG(TAG_Wallet_Message_GoldChanged, "Wallet.Message.GoldChanged");



UE_DEFINE_GAMEPLAY_TAG(TAG_QuickBar_Message_SlotsChanged, "QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG(TAG_QuickBar_Message_OneSlotChanged, "QuickBar.Message.OneSlotChanged");
UE_DEFINE_GAMEPLAY_TAG(TAG_QuickBar_Message_ActiveIndexChanged, "QuickBar.Message.ActiveIndexChanged");
UE_DEFINE_GAMEPLAY_TAG(TAG_QuickBar_Message_EquipChanged, "QuickBar.Message.EquipChanged");

FGlobalNativeTags::FGlobalNativeTags()
{

}
