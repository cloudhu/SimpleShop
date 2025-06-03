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
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	if (IsFocusable())
	{
		InputModeGameAndUI.SetWidgetToFocus(TakeWidget());
	}
	GetOwningPlayer()->SetInputMode(InputModeGameAndUI);
	SetVisibility(ESlateVisibility::Visible);
}
