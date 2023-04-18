// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GlobalNativeTags.generated.h"


//所有物品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Basic);//基础
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Epic);//史诗
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Legendary);//传奇
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Mythic);//神话
//辅助品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_AuxiliaryProps);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_PhysicalAttack);//物理攻击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicAttack);//魔法攻击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_PhysicalPenetration);//物理穿透
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicPenetration);//魔法穿透
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Crit);//暴击
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Armor);//护甲
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_MagicResistance);//魔法抗性
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_AttackSpeed);//攻击速度
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Shoes);//鞋子

//可合成的物品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Compoundable);

//合成物品
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Message_Compound);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_CategoryOnClick_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Expanded_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Category_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Shop_Category_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Shop_CategoryOnClick_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Gravity_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_MaxGravity_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_All);

//物品数量
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Item_Count);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ConfirmedTransaction_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Transaction_Message);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Transaction_Message_Result);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Item_Type_Consumables);//消耗品

//金钱发生改变时发消息通知
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Wallet_Message_GoldChanged);

//物品发生改变时发消息通知
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