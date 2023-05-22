// Copyright CloudHu. All Rights Reserved.
#include "UI/Shop/ItemCompoundPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/ShopSubsystem.h"
#include "Data/ItemTable.h"
#include "Message/GlobalNativeTags.h"
#include "UI/Shop/UW_CompoundItem.h"

//绘制合成装备的最小单元
struct FDrawSlot
{
	//绘制画布里各种元素的坐标,主要是做线段的绘制
	struct FSlot
	{
		FSlot(UCanvasPanelSlot* InSlot)
			: Slot(InSlot)
		{
		}

		//设置槽位所在画布上的位置
		void SetPosition(const FVector2D& InPosition) const;

		//获取其尺寸
		FVector2D GetSize() const;
	private:
		UCanvasPanelSlot* Slot; //画布上的槽位
	};

	//绘制装备槽,点击时可以购买装备
	struct FTable : public FSlot
	{
		FTable(UCanvasPanelSlot* InSlot, const FItemTable* InTableSlot)
			: FSlot(InSlot)
			  , TableSlot(InTableSlot)
		{
		}

		const FItemTable* GetSlotTable() const;
	private:
		const FItemTable* TableSlot; //缓存装备数据
	};

	//类似乐高积木,这里通过数组来记录所有合成所需的积木素材,然后再搭建最终的作品
	TArray<FSlot> LREndPanel; //两端的拐弯 ⌈ 或 ⌉
	TArray<FSlot> VLinePanel; //|
	TArray<FSlot> HLinePanel; //_
	TArray<FSlot> TLinePanel; //T
	TArray<FSlot> LRVPanel; //_|_ 
	TArray<FTable> ItemPanel; //装备按钮
};

void FDrawSlot::FSlot::SetPosition(const FVector2D& InPosition) const
{
	if (Slot)
	{
		Slot->SetPosition(InPosition);
	}
}

FVector2D FDrawSlot::FSlot::GetSize() const
{
	return Slot != nullptr ? Slot->GetSize() : FVector2D::ZeroVector;
}

const FItemTable* FDrawSlot::FTable::GetSlotTable() const
{
	return TableSlot;
}

UItemCompoundPanel::UItemCompoundPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemCompoundPanel::NativeConstruct()
{
	Super::NativeConstruct();
	//监听钱包发出的货币变更消息
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	ListenerHandle = MessageSystem.RegisterListener(TAG_Item_Message_Compound, this, &ThisClass::OnNotificationCompoundMessage);
}

//PRAGMA_DISABLE_OPTIMIZATION
void UItemCompoundPanel::UpdateSlot(const FItemTable* InTable)
{
	//0.清除上一个物品的合成图
	EquipmentPanel->ClearChildren();

	if (InTable)
	{
		//1.根据数据表递归获取子类的深度,根据深度来盖楼
		int32 InLayer = GetLayerDepth(InTable);
		//2.创建物品槽来显示物品
		if (ItemSlotClass != nullptr)
		{
			if (UUW_CompoundItem* InMainSlotWidget = CreateWidget<UUW_CompoundItem>(GetWorld(), ItemSlotClass))
			{
				//3.将生成的物品槽添加到面板中,设置其位置,递归更新子类
				if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InMainSlotWidget))
				{
					InPanelSlot->SetSize(IconSize);
					InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));
					InPanelSlot->SetPosition(StartPostion);

					RecursiveUpdateSlot(InTable, StartPostion, --InLayer);
				}
				//4.根据数据更新物品信息
				InMainSlotWidget->UpdateSlot(InTable);
			}
		}
	}
}

//PRAGMA_ENABLE_OPTIMIZATION


void UItemCompoundPanel::OnNotificationCompoundMessage(FGameplayTag Channel, const FUserInterfaceMessage& Notification)
{
	if (Notification.Owner == GetOwningPlayerPawn())
	{
		if (Notification.ItemID == INDEX_NONE)
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SetVisibility(ESlateVisibility::Visible);

			UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());
			if (const FItemTable* InSlotTable = ShopSubsystem.GetSlotTableByID(Notification.ItemID))
			{
				UpdateSlot(InSlotTable);
			}
		}
	}
}

int32 UItemCompoundPanel::GetLayerDepth(const FItemTable* InTable, int32 InDepth /*= 1*/)
{
	InDepth++;
	//递归物品数据中的子类数组,获得楼层
	TArray<int32> Depths;
	TArray<int32> Children;
	if (const UObject* Src = GetDefault<UObject>(InTable->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Src))
		{
			Children = ItemDef->Execute_GetChildrenIds(Src);
		}
		else if (Src->Implements<UItemDefinitionInterface>())
		{
			Children = IItemDefinitionInterface::GetChildrenIds(Src);
		}
	}

	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	for (const auto& Tmp : Children)
	{
		if (const FItemTable* InSlotTable = ShopSubsystem.GetSlotTableByID(Tmp))
		{
			Depths.Add(GetLayerDepth(InSlotTable, InDepth));
		}
	}

	for (const auto& Tmp : Depths)
	{
		if (Tmp > InDepth)
		{
			InDepth = Tmp;
		}
	}

	return InDepth;
}

void UItemCompoundPanel::RecursiveUpdateSlot(const FItemTable* InTable, const FVector2D& InStartPosition, FSlotLayer InSlotLayer)
{
	//0.合成物品所需的数量,这里渲染整个合成图谱的思路就是先生成合成所需的素材,然后再来布局.这种思路就像是拼图一样,先有各种碎片,再根据碎片来拼凑完整的图像
	TArray<int32> Children;
	if (const UObject* Src = GetDefault<UObject>(InTable->ItemDefinition))
	{
		if (const IItemDefinitionInterface* ItemDef = Cast<IItemDefinitionInterface>(Src))
		{
			Children = ItemDef->Execute_GetChildrenIds(Src);
		}
		else if (Src->Implements<UItemDefinitionInterface>())
		{
			Children = IItemDefinitionInterface::GetChildrenIds(Src);
		}
	}

	const int32 NumberChildren = Children.Num();
	//1.画格子的Lambda函数,关于Lambda函数大家可以百度去了解一下,这里的复杂度已经很高了,所以不做科普了
	FDrawSlot DrawSlot;
	auto SpawnSlot = [&](TArray<FDrawSlot::FSlot>& PanelSlots, UTexture2D* InTexture2D, const int32 InLayer = 1)
	{
		if (UImage* InImage = NewObject<UImage>(this))
		{
			InImage->SetBrushFromTexture(InTexture2D);
			if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InImage))
			{
				InPanelSlot->SetSize(FVector2D(IconSize.X * InLayer, IconSize.Y));
				InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));

				PanelSlots.Add(FDrawSlot::FSlot(InPanelSlot));
			}
		}
	};
	//2.生成物品Lambda函数
	auto SpawnItem = [&](TArray<FDrawSlot::FTable>& PanelSlots, const FItemTable* InTableData)
	{
		if (ItemSlotClass != nullptr)
		{
			if (UUW_CompoundItem* InMainSlotWidget = CreateWidget<UUW_CompoundItem>(GetWorld(), ItemSlotClass))
			{
				if (UCanvasPanelSlot* InPanelSlot = EquipmentPanel->AddChildToCanvas(InMainSlotWidget))
				{
					InPanelSlot->SetSize(IconSize);
					InPanelSlot->SetAlignment(FVector2D(0.5f, 0.f));

					PanelSlots.Add(FDrawSlot::FTable(InPanelSlot, InTableData));
				}
				InMainSlotWidget->UpdateSlot(InTableData);
			}
		}
	};

	//3.生成所有所需的素材
	//////////////////////////////////////////////////////////////////////////
	if (NumberChildren == 1) //仅有一个子类,画一条垂直线即可
	{
		SpawnSlot(DrawSlot.VLinePanel, LineTexture.VerticalLine);
	}
	else if (NumberChildren >= 2) //两个以上子类,需要拆分
	{
		//生成 ⌉ 型 线条
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnRight);
		//生成 ⌈ 型 线条
		SpawnSlot(DrawSlot.LREndPanel, LineTexture.TurnLeft);
		//生成 _|_ 型线
		SpawnSlot(DrawSlot.LRVPanel, LineTexture.TurnLeftAndRightVertically);

		//生成 T 型线
		for (int32 i = 0; i < NumberChildren - 2; i++)
		{
			SpawnSlot(DrawSlot.TLinePanel, LineTexture.TLine);
		}
		//生成 - 水平线
		for (int32 i = 0; i < NumberChildren; i++)
		{
			SpawnSlot(DrawSlot.HLinePanel, LineTexture.HorizontalLine, InSlotLayer);
		}
	}
	UShopSubsystem& ShopSubsystem = UShopSubsystem::Get(GetWorld());

	//在格子中生成物品
	for (const auto& Tmp : Children)
	{
		if (const FItemTable* InSlotTable = ShopSubsystem.GetSlotTableByID(Tmp))
		{
			SpawnItem(DrawSlot.ItemPanel, InSlotTable);
		}
	}

	//收集所有的线条有效数量
	const int32 ValidNum = DrawSlot.HLinePanel.Num() +
		DrawSlot.LREndPanel.Num() +
		DrawSlot.LRVPanel.Num() +
		DrawSlot.TLinePanel.Num() +
		DrawSlot.VLinePanel.Num();

	//数量是1的情况,通过垂直线连接父子,类似 吕 字型布局
	if (NumberChildren == 1)
	{
		const FDrawSlot::FSlot InVLin = DrawSlot.VLinePanel.Pop(); //垂直线
		const FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop(); //子物品
		FVector2D NewPos = FVector2D(InStartPosition.X, InStartPosition.Y + InVLin.GetSize().Y); //计算垂直线位置:从起始位置往下移动垂直线Y轴长度
		InVLin.SetPosition(NewPos); //设置垂直线在父物品的下方
		NewPos = FVector2D(NewPos.X, NewPos.Y + InVLin.GetSize().Y); //计算子物品的位置
		InItem.SetPosition(NewPos); //将子物品设置在垂直线下方,最终形成 吕 字型布局,用垂直线连接父和子物品
	}
	else if (NumberChildren >= 2)
	{
		const FDrawSlot::FSlot InLrvLin = DrawSlot.LRVPanel.Pop(); //先获得一个生成 _|_ 型线来连接父物品
		const FVector2D NewPos = FVector2D(InStartPosition.X, InStartPosition.Y + InLrvLin.GetSize().Y); //从起始位置往下移动线段的Y轴长度
		InLrvLin.SetPosition(NewPos); //布局位置

		const float HWidth = DrawSlot.HLinePanel[0].GetSize().X; //获取水平线宽度
		FVector2D R = NewPos; //声明右侧的位置
		FVector2D L = NewPos; //声明左侧的位置
		//递归子项,计算拐点
		auto Add = [&](const int32 I, TArray<FDrawSlot::FSlot>& InPanels, FSlotLayer SlotLayer)
		{
			FVector2D InNewValidPosition; //声明新的有效位置

			const FDrawSlot::FSlot InNewSlot = InPanels.Pop(); //弹出基本元素,可以想象成拼图所用的碎片
			const FDrawSlot::FTable InItem = DrawSlot.ItemPanel.Pop(); //物品
			const float NextSpacing = HWidth / 2.f + InNewSlot.GetSize().X / 2.f; //计算下一个相邻物品的间隔
			if (I & 0x1) //奇数:代表左侧
			{
				L = FVector2D(L.X - NextSpacing, L.Y); //左侧位置用X轴减间隔,这个是笛卡尔坐标系基本常识
				InNewValidPosition = L; //得到新的有效坐标
			}
			else //偶数:代表右侧
			{
				R = FVector2D(R.X + NextSpacing, L.Y); //右侧位置用X轴加间隔
				InNewValidPosition = R;
			}
			InNewSlot.SetPosition(InNewValidPosition); //设置格子的位置

			const FVector2D InNewItemPosition = FVector2D(InNewValidPosition.X, InNewValidPosition.Y + InItem.GetSize().Y); //计算物品位置
			InItem.SetPosition(InNewItemPosition); //设置物品位置
			//递归更新物品
			RecursiveUpdateSlot(InItem.GetSlotTable(), InNewItemPosition, SlotLayer -= 2);
		};

		//绘制水平线段
		for (int32 i = 0; i < ValidNum; i++)
		{
			FVector2D InNewValidPosition = FVector2D::ZeroVector;

			if (DrawSlot.HLinePanel.Num())
			{
				FDrawSlot::FSlot InHor = DrawSlot.HLinePanel.Pop();
				const float NestSpacing = InHor.GetSize().X / 2.f + InLrvLin.GetSize().X / 2.f;
				if (i & 0x1)
				{
					L = FVector2D(L.X - NestSpacing, L.Y);
					InNewValidPosition = L;
				}
				else
				{
					R = FVector2D(R.X + NestSpacing, L.Y);
					InNewValidPosition = R;
				}

				InHor.SetPosition(InNewValidPosition);
			}

			//绘制 T 型线段
			if (DrawSlot.TLinePanel.Num())
			{
				Add(i, DrawSlot.TLinePanel, InSlotLayer);
			}
		}

		//绘制左右拐点线段
		for (int32 i = 0; i < 2; i++)
		{
			Add(i, DrawSlot.LREndPanel, InSlotLayer);
		}
	}
}

void UItemCompoundPanel::NativeDestruct()
{
	Super::NativeDestruct();

	if (ListenerHandle.IsValid())
	{
		ListenerHandle.Unregister();
	}
}
