// CloudHu:604746493@qq.com All Rights Reserved


#include "UI/Core/UW_PanelBase.h"

UUW_PanelBase::UUW_PanelBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UUW_PanelBase::OnPanelClose()
{
	GetOwningPlayer()->SetShowMouseCursor(false);
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	SetVisibility(ESlateVisibility::Hidden);
}

void UUW_PanelBase::OnPanelOpen()
{
	GetOwningPlayer()->SetShowMouseCursor(true);
	FInputModeUIOnly ModeUIOnly;
	ModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	if (IsFocusable())
	{
		ModeUIOnly.SetWidgetToFocus(TakeWidget());
	}
	GetOwningPlayer()->SetInputMode(ModeUIOnly);
	SetVisibility(ESlateVisibility::Visible);
}
