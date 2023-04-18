// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "Definition/ItemDefinition.h"
#include "Interface/ItemDefinitionInterface.h"
#include "ItemTable.generated.h"

//物品配置表
USTRUCT(BlueprintType)
struct FItemTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FItemTable();
	//编号
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	int32 ID;

	//价格
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	int32 Price;

	/**
	 * @brief 库存
	 */
	UPROPERTY(EditDefaultsOnly, Category="Slot")
	int32 Stock;

	//MustImplement任意实现ItemDefinitionInterface接口的对象都可以作为物品交易
	UPROPERTY(EditDefaultsOnly, Category = Slot, meta = (MustImplement = "ItemDefinitionInterface"))
	TSubclassOf<UObject> ItemDefinition;
public:
	bool IsValid() const;

	bool IsConsumables() const;

	bool ItemTypeHasTag(const FGameplayTag& Tag) const;
};
