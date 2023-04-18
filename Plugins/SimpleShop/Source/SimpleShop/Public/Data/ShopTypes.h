// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShopTypes.generated.h"

//商店配置表
USTRUCT(BlueprintType)
struct FShopTypes : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** 等级 */
	UPROPERTY(EditDefaultsOnly, Category = ShopConfig)
	int32 Level = 1;

	/** 最大数量 */
	UPROPERTY(EditDefaultsOnly, Category = ShopConfig)
	int32 MaxAmount = 8;

	/** 最大负重 */
	UPROPERTY(EditDefaultsOnly, Category = ShopConfig)
	float MaxGravity = 20.f;

	/** 升级消耗 */
	UPROPERTY(EditDefaultsOnly, Category = "ShopConfig")
	int32 UpgradeCost;
};

//背包配置表
USTRUCT(BlueprintType)
struct FInventoryConfiguration : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** 等级 */
	UPROPERTY(EditDefaultsOnly, Category = InventoryConfig)
	int32 Level = 1;

	/** 增加容量 */
	UPROPERTY(EditDefaultsOnly, Category = InventoryConfig)
	int32 AddedVolume = 4;

	/** 最大负重 */
	UPROPERTY(EditDefaultsOnly, Category = "InventoryConfig")
	float MaxGravity = 20.f;

	/** 升级消耗 */
	UPROPERTY(EditDefaultsOnly, Category = "InventoryConfig")
	int32 UpgradeCost = 800;
};

//快捷栏配置表
USTRUCT(BlueprintType)
struct FQuickBarConfiguration : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** 等级 */
	UPROPERTY(EditDefaultsOnly, Category = QuickBarConfig)
	int32 Level = 1;

	/** 最大数量 */
	UPROPERTY(EditDefaultsOnly, Category = QuickBarConfig)
	int32 MaxVolume = 3;

	/** 最大负重 */
	UPROPERTY(EditDefaultsOnly, Category = "QuickBarConfig")
	float MaxGravity = 20.f;

	/** 升级消耗 */
	UPROPERTY(EditDefaultsOnly, Category = "QuickBarConfig")
	int32 UpgradeCost = 1000;
};
