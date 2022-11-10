//  Copyright CloudHu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntroductionToAlgorithm.generated.h"
MYTESTPROJECT_API DECLARE_LOG_CATEGORY_EXTERN(LogIntroductionToAlgorithm, Log, All);
UCLASS()
class MYTESTPROJECT_API AIntroductionToAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIntroductionToAlgorithm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static void InsertionSort(TArray<int>& ArraySrc);

	static void Merge(TArray<int>& ArraySrc,const int Start,const int Mid,const int End);
	
	static void MergeSort(TArray<int>& ArraySrc,const int Start,const int End);
	
};
