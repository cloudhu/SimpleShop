// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Interface/ItemDefinitionInterface.h"
#include "ItemTable.generated.h"

//物品配置表
USTRUCT(BlueprintType)
struct FItemTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FItemTable();
	/**
	 * @brief 编号
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	int32 ID;

	/**
	 * @brief 价格
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	int32 Price;

	/**
	 * @brief 库存
	 */
	UPROPERTY(EditDefaultsOnly, Category="Slot")
	int32 Stock;

	/**
	 * @brief meta元数据MustImplement修饰任意实现ItemDefinitionInterface接口的对象都可以作为物品交易
	 */
	UPROPERTY(EditDefaultsOnly, Category = Slot, meta = (MustImplement = "ItemDefinitionInterface"))
	TSubclassOf<UObject> ItemDefinition;
public:
	/**
	 * @brief 是否是有效物品
	 * @return true有效,false无效
	 */
	bool IsValid() const;

	/**
	 * @brief 是否是消耗品
	 * @return true是,false否
	 */
	bool IsConsumables() const;

	/**
	 * @brief 该物品类型是否拥有指定的标签
	 * @param Tag 标签
	 * @return true是,false否
	 */
	bool ItemTypeHasTag(const FGameplayTag& Tag) const;
};
