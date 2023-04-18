// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Definition/ItemCategory.h"
#include "UserInterfaceMessage.generated.h"

//UI消息
USTRUCT(BlueprintType)
struct FUserInterfaceMessage
{
	GENERATED_USTRUCT_BODY()

	//所有者
	UPROPERTY(BlueprintReadOnly, Category = UserInterfaceMessage)
	APawn* Owner = nullptr;

	/// <summary>
	/// 物品编号
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = UserInterfaceMessage)
	int32 ItemID = INDEX_NONE;
};

/// <summary>
/// 目录数量变化消息
/// </summary>
USTRUCT(BlueprintType)
struct FCategoryClickMessage
{
	GENERATED_USTRUCT_BODY()

	//所有者
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	const APawn* Character = nullptr;

	/**
	 * @brief 标签
	 */
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	FGameplayTag Tag;

	//新的数目
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	int32 NewNum = INDEX_NONE;
};

/// <summary>
/// 目录实例变化消息
/// </summary>
USTRUCT(BlueprintType)
struct FCategoryInstanceChangeMessage
{
	GENERATED_USTRUCT_BODY()

	//所有者
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	const APawn* Character = nullptr;


	/** 目录实例 */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	UItemCategory* ItemCategory;

	//变化
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	int32 Delta = INDEX_NONE;

	//新的数目
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	int32 NewNum = INDEX_NONE;
};