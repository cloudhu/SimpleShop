// Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/TransactionMessage.h"
#include "UW_ItemSlotBase.generated.h"

class UItemInstance;
class UTextBlock;
class UImage;
class UButton;
struct FItemTable;
class UUW_ItemTips;
struct FTransactionMessage;

/**
 * 物品UI基类
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_ItemSlotBase : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	//缓存数据
private:
	/**
	 * @brief 实例唯一编号，实例是全局唯一的，就像世界上没有两片相同的树叶一样，种子的DNA是一样的，但是长出来的树叶千变万化
	 */
	FGuid InstanceGuid;

	/**
	 * @brief 物品编号
	 */
	int32 ItemID;

	/**
	 * @brief 实例索引
	 */
	int32 InstanceIndex;

	/**
	 * @brief 价格
	 */
	int32 Price;

	/**
	 * @brief 数量
	 */
	int32 Amount;

	/**
	 * @brief 物品的拥有者
	 */
	UPROPERTY()
	APawn* ItemOwner;

	/**
	 * @brief 提示UI
	 */
	UPROPERTY()
	UUW_ItemTips* TipPtr;

	/**
	 * @brief 数据缓存
	 */
	const FItemTable* DataTable;

	/**
	 * @brief 交易消息监听句柄
	 */
	FGameplayMessageListenerHandle TransactionMessageListenerHandle;

public:
	/**
	 * @brief 物品图标
	 */
	UPROPERTY(BlueprintReadOnly, Category = Item, meta = (BindWidget))
	UImage* ItemIcon;

	/**
	 * @brief 物品按钮
	 */
	UPROPERTY(BlueprintReadOnly, Category = Item, meta = (BindWidget))
	UButton* ItemButton;

	/**
	 * @brief 数量文本
	 */
	UPROPERTY(BlueprintReadOnly, Category = CategoryButton, meta = (BindWidget))
	UTextBlock* CountText;

	/**
	 * @brief 提示类
	 */
	UPROPERTY(EditDefaultsOnly, Category = Item)
	TSubclassOf<UUW_ItemTips> TipClass;

public:
	//构造函数
	UUW_ItemSlotBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief UI类原生构造
	 */
	virtual void NativeConstruct() override;

	/**
	 * @brief UI类原生析构
	 */
	virtual void NativeDestruct() override;

	/**
	 * @brief 当物品被添加到List中时触发
	 * @param ListItemObject 列表物品对象
	 */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	/**
	 * @brief 判断是哪里的物品
	 * @return true:是快捷栏;false:背包
	 */
	virtual bool IsQuickBarItem() const { return false; }

public: //Getters
	/**
	 * @brief 获取提示UI
	 * @return 提示UI指针
	 */
	UWidget* GetTip();

	/**
	 * @brief 获取物品所属的玩家
	 * @return 玩家指针
	 */
	APawn* GetItemOwner() const { return ItemOwner; }

	/**
	 * @brief 获取物品编号
	 * @return 物品编号
	 */
	int32 GetItemID() const { return ItemID; }

	/**
	 * @brief 获取实例索引
	 * @return 索引
	 */
	int32 GetInstanceIndex() const { return InstanceIndex; }

	/**
	 * @brief 获取价格
	 * @return 价格
	 */
	int32 GetPrice() const { return Price; }

	/**
	 * @brief 获取数量
	 * @return 数量
	 */
	int32 GetAmount() const { return Amount; }

	/**
	 * @brief 获取数据表
	 * @return 数据表
	 */
	const FItemTable* GetDataTable() const { return DataTable; }

	/**
	 * @brief 获取全局唯一编号
	 * @return 全局唯一编号
	 */
	const FGuid& GetGuid() const { return InstanceGuid; }

public: //Setters
	/**
	 * @brief 设置物品编号
	 * @param InID 编号
	 */
	void SetItemID(int32 InID);

	/**
	 * @brief 设置物品价格
	 * @param InPrice 价格
	 */
	void SetItemPrice(int32 InPrice);

	/**
	 * @brief 设置图标显示
	 * @param Brush 刷子
	 */
	void SetIconBrush(const FSlateBrush& Brush) const;

	/**
	 * @brief 设置物品所有者
	 * @param InOwner 所属角色
	 */
	void SetItemOwner(APawn* InOwner);

	/**
	 * @brief 设置实例全局唯一编号
	 * @param InGuid 全局唯一编号
	 */
	void SetInstanceGuid(const FGuid& InGuid);

	/**
	 * @brief 设置物品数量
	 * @param InNum 数量
	 */
	void SetNumText(const int32 InNum);

	/**
	 * @brief 设置物品实例编号
	 * @param InInstanceID 实例编号
	 */
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
	void SetInstanceIndex(int32 InInstanceID);

	/**
	 * @brief 是否激活物品
	 * @param bEnabled 物品状态
	 */
	void SetItemIsEnabled(const bool bEnabled) const;
public:
	/**
	 * @brief 根据数据更新UI格子
	 * @param InTable 数据表
	 */
	virtual void UpdateSlot(const FItemTable* InTable);

	/**
	 * @brief 处理交易结果
	 * @param bSuccess 交易结果
	 * @param InAmount 交易数量
	 */
	virtual void TransactionResult(const bool bSuccess, const int32 InAmount);

	/**
	 * @brief 收到物品交易消息通知时
	 * @param Channel 频道
	 * @param Notification 消息 
	 */
	void OnNotificationTransactionResultMessage(FGameplayTag Channel, const FTransactionMessageResult& Notification);

	/**
	 * @brief 显示图标
	 * @param bVisible true 显示
	 */
	void ShowIcon(const bool bVisible) const;

	/**
	 * @brief 广播交易消息
	 */
	void BroadcastTransactionMessage() const;

	/**
	 * @brief 通过物品实例更新物品
	 * @param Instance 实例
	 */
	void UpdateItemByInstance(const UItemInstance* Instance);


	/**
	 * @brief 物品点击时触发
	 */
	UFUNCTION(BlueprintCallable, Category = ItemSlot)
	virtual void OnClickedWidget();
};
