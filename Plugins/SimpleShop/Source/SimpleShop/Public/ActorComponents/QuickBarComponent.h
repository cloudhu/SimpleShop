// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CH_GameplayMessageSubsystem.h"
#include "Core/ActorComponentBase.h"
#include "QuickBarComponent.generated.h"

class UUW_UpgradeConfirm;
class UItemInstance;
//class UEquipmentInstance;
//class UEquipmentManagerComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class SIMPLESHOP_API UQuickBarComponent : public UActorComponentBase
{
	GENERATED_BODY()

public:
	UQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * @brief 升级确认窗口类
	 */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UUW_UpgradeConfirm> UpgradeConfirmClass;

	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void CycleActiveSlotBackward();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "QuickBar")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "QuickBar")
	TArray<UItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "QuickBar")
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "QuickBar")
	UItemInstance* GetActiveSlotItem() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "QuickBar")
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "QuickBar")
	void AddItemToSlot(int32 SlotIndex, UItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "QuickBar")
	UItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	/** 设置快捷栏的数量.快捷栏最多支持9个,对应数字1-9 */
	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void SetNumSlots(int32 InNumSlots);

	UFUNCTION(BlueprintPure, Category = "QuickBar")
	static UQuickBarComponent* FindQuickBarComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UQuickBarComponent>() : nullptr); }

	virtual void BeginPlay() override;

	void SwapItemSlot(int32 SlotIndexA, int32 SlotIndexB);
	
	void SendEquipItemMessage(const UItemInstance* SlotItem,const bool bEquip) const;

	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void UpdateSlots();

	/// <summary>
	/// 格子发生变化的时候发送消息
	/// </summary>
	/// <param name="SlotItem">物品实例</param>
	void OnSlotChanged(UItemInstance* SlotItem) const;

	/// <summary>
	/// 获取对应索引的物品数量
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	int32 GetItemAmountByIndex(int32 ItemIndex) const;

	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void OnExpandQuickBar();

	UFUNCTION(BlueprintCallable, Category = "QuickBar")
	void ConfirmUpgradeQuickBar();

	bool CanExpand() const { return Level + 1 < MaxLevel; }
private:
	void UnEquipItemInSlot();

	void EquipItemInSlot();

protected:
	/**
	 * @brief 当前格子数目
	 */
	UPROPERTY()
	int32 NumSlots = 6;

	/**
	 * @brief 最高等级
	 */
	UPROPERTY()
	int32 MaxLevel = 6;

	/**
	 * @brief 升级消耗
	 */
	UPROPERTY(Replicated)
	int32 UpgradeCost = 800;

	/**
	 * @brief 等级
	 */
	UPROPERTY(Replicated)
	int32 Level = 0;

	/**
	 * @brief 最大负重
	 */
	UPROPERTY(Replicated)
	float MaxGravity = 10.f;

	UFUNCTION()
	void OnRep_Slots() const;

	UFUNCTION()
	void OnRep_ActiveSlotIndex() const;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Slots)
	TArray<TObjectPtr<UItemInstance>> Slots;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	UUW_UpgradeConfirm* ConfirmWindow;
};


USTRUCT(BlueprintType)
struct FQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	APawn* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UItemInstance>> Slots;
};


USTRUCT(BlueprintType)
struct FSlotChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	APawn* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	UItemInstance* SlotInstance;
};

USTRUCT(BlueprintType)
struct FQuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	AActor* Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 ActiveIndex = 0;
};

USTRUCT(BlueprintType)
struct FEquipChangedMessage
{
	GENERATED_BODY()

	/// <summary>
	/// 装备的所有者
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	AActor* Owner = nullptr;

	/// <summary>
	/// 物品定义
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TSubclassOf<UObject> ItemDef;

	/// <summary>
	/// 是否装备,true 代表装备,false:卸装
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	bool bEquip = true;

	/**
	 * @brief 物品的唯一编号,用于对物品的属性进行监听
	 */
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	FGuid Guid = FGuid();
};
