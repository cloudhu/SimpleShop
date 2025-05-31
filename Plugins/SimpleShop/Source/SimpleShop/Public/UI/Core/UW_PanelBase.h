// CloudHu:604746493@qq.com All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PanelBase.generated.h"

/**
 * @brief Base class for UI panels providing common functionality for opening and closing panels.
 * This class is designed to be inherited by specific panel implementations such as inventory or shop panels.
 * It provides methods to handle visibility, mouse cursor behavior, and input modes when panels are opened or closed.
 */
UCLASS()
class SIMPLESHOP_API UUW_PanelBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_PanelBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category=UI)
	virtual void OnPanelClose();

	UFUNCTION(BlueprintCallable, Category=UI)
	virtual void OnPanelOpen();
};
