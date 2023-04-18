// Copyright (C) CloudHu.2022.All Rights Reserved.
#include "UI/Inventory/UW_DragDropIcon.h"
#include "Components/Image.h"

UUW_DragDropIcon::UUW_DragDropIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_DragDropIcon::DrawIcon(const FSlateBrush& Brush) const
{
	if (ICON)
	{
		ICON->SetBrush(Brush);
	}
}
