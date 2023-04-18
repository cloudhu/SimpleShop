// Copyright (C) CloudHu.2022.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_DragDropIcon.generated.h"

class UImage;
/**
 * 拖拽的图标
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable)
class SIMPLESHOP_API UUW_DragDropIcon : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage *ICON;

public:
	UUW_DragDropIcon(const FObjectInitializer& ObjectInitializer);

	void DrawIcon(const FSlateBrush& Brush) const;
};
