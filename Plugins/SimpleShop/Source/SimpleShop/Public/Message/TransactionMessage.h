// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TransactionMessage.generated.h"

//交易消息
USTRUCT(BlueprintType)
struct FTransactionMessage
{
	GENERATED_USTRUCT_BODY()

	//买家
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Buyer = nullptr;

	//卖家
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Seller = nullptr;

	//物品编号
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 ItemID = INDEX_NONE;

	//单价
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 Price = INDEX_NONE;

	//物品实例编号
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 InstanceID = INDEX_NONE;

	//物品索引
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	bool bIsQuickBarItem = false;

	//最大数量
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 MaxCount = 100;

	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	FText DisplayName;

	/** 是否是合成物品 */
	UPROPERTY(BlueprintReadOnly, Category = "Transaction")
	bool bIsCompoundItem = false;
};

//交易结果反馈
USTRUCT(BlueprintType)
struct FTransactionMessageResult
{
	GENERATED_USTRUCT_BODY()

	/**
	 * @brief 买家
	 */
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Buyer = nullptr;

	/**
	 * @brief 卖家
	 */
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Seller = nullptr;

	/**
	 * @brief 物品编号
	 */
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 ItemID = INDEX_NONE;

	/**
	 * @brief 是否购买成功
	 */
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	bool bSuccess = false;

	/**
	 * @brief 最终交易数量
	 */
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 Amount = 1;
};

//钱包货币数量变化消息
USTRUCT(BlueprintType)
struct FWalletChangeMessage
{
	GENERATED_USTRUCT_BODY()

	//钱包的所有者
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	const APawn* Character = nullptr;

	//之前的数目
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	int32 OldNum = INDEX_NONE;

	//新的数目
	UPROPERTY(BlueprintReadOnly, Category = WalletChangeMessage)
	int32 NewNum = INDEX_NONE;
};

//交易消息
USTRUCT(BlueprintType)
struct FConfirmedTransactionMessage
{
	GENERATED_USTRUCT_BODY()

	//买家
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Buyer = nullptr;

	//卖家
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	const APawn* Seller = nullptr;

	//物品编号
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 ItemID = INDEX_NONE;

	//单价
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 Price = INDEX_NONE;

	//物品实例编号
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 InstanceIndex = INDEX_NONE;

	//物品索引
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	bool bIsQuickBarItem = false;

	//数量
	UPROPERTY(BlueprintReadOnly, Category = Transaction)
	int32 Count = 1;

	/** 是否是合成物品 */
	UPROPERTY(BlueprintReadOnly, Category = "Transaction")
	bool bIsCompoundItem = false;
};

///词缀数量变化消息
USTRUCT(BlueprintType)
struct FAffixChangeMessage
{
	GENERATED_USTRUCT_BODY()

	//钱包的所有者
	UPROPERTY(BlueprintReadOnly, Category = AffixChangeMessage)
	const APawn* Character = nullptr;

	//之前的数目
	UPROPERTY(BlueprintReadOnly, Category = AffixChangeMessage)
	int32 Delta = INDEX_NONE;

	//新的数目
	UPROPERTY(BlueprintReadOnly, Category = AffixChangeMessage)
	int32 NewNum = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = AffixChangeMessage)
	FGuid Guid = FGuid();
};
