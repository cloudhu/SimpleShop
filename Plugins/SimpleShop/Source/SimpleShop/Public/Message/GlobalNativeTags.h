// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GlobalNativeTags.generated.h"

//声明插件日志的类别
DECLARE_LOG_CATEGORY_EXTERN(LogSimpleShop,Log,All);

//////////物品目录的标签声明///////////////
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_All);//所有类型
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Basic);//基础
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Epic);//史诗
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Legendary);//传奇
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Mythic);//神话

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Consumables);//消耗品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_AuxiliaryProps);//辅助品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_PhysicalAttack);//物理攻击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicAttack);//魔法攻击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_PhysicalPenetration);//物理穿透
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicPenetration);//魔法穿透
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Crit);//暴击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Armor);//护甲
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicResistance);//魔法抗性
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_AttackSpeed);//攻击速度
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Shoes);//鞋类

//可合成的物品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Compoundable);

//////////////消息标签///////////////
//合成物品消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Message_Compound);
//背包目录被点击的消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_CategoryOnClick_Message);
//背包扩展消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Expanded_Message);
//背包目录消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Category_Message);
//商店目录消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Shop_Category_Message);
//商店目录被点击的消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Shop_CategoryOnClick_Message);
//背包重量消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Gravity_Message);
//最大背包承重消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_MaxGravity_Message);

//背包物品数量
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Item_Count);
//确认交易消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ConfirmedTransaction_Message);
//交易消息
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Transaction_Message);
//交易消息结果
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Transaction_Message_Result);

//金钱发生改变时发消息通知
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Wallet_Message_GoldChanged);

//背包物品数量发生改变时发消息通知
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Message_StackChanged);

//快捷栏消息标签
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_QuickBar_Message_SlotsChanged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_QuickBar_Message_OneSlotChanged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_QuickBar_Message_ActiveIndexChanged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_QuickBar_Message_EquipChanged);

USTRUCT(BlueprintType)
struct FGlobalNativeTags
{
	GENERATED_USTRUCT_BODY()

		FGlobalNativeTags();
};