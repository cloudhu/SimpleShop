//  Copyright CloudHu. All Rights Reserved.


#include "IntroductionToAlgorithm.h"
DEFINE_LOG_CATEGORY(LogIntroductionToAlgorithm);
// Sets default values
AIntroductionToAlgorithm::AIntroductionToAlgorithm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AIntroductionToAlgorithm::BeginPlay()
{
	Super::BeginPlay();
	TArray<int> ArrayTest{2, 100, 4, 33, 0, 9, 1, 88, 12, 77, 3};
	//InsertionSort(ArrayTest);
	MergeSort(ArrayTest,0,10);
	for (auto& i : ArrayTest)
	{
		UE_LOG(LogIntroductionToAlgorithm, Log, TEXT("LogIntroductionToAlgorithm:%i"), i);
	}
}

void AIntroductionToAlgorithm::InsertionSort(TArray<int>& ArraySrc)
{
	for (int j = 1; j < ArraySrc.Num(); ++j)
	{
		const int Key = ArraySrc[j];
		int i = j - 1;
		while (i > -1 && ArraySrc[i] > Key)
		{
			ArraySrc[i + 1] = ArraySrc[i];
			i--;
		}
		ArraySrc[i + 1] = Key;
	}
}

void AIntroductionToAlgorithm::Merge(TArray<int>& ArraySrc, const int Start, const int Mid, const int End)
{
	const int Length1 = Mid - Start + 1;
	const int Length2 = End - Mid;
	TArray<int> TmpArr1;
	TArray<int> TmpArr2;
	TmpArr1.SetNum(Length1 + 1);
	TmpArr2.SetNum(Length2 + 1);
	for (int i = 0; i < Length1; ++i)
	{
		TmpArr1[i] = ArraySrc[Start + i];
	}

	for (int i = 0; i < Length2; ++i)
	{
		TmpArr2[i] = ArraySrc[Mid + i + 1];
	}

	TmpArr1[Length1] = 999;
	TmpArr2[Length2] = 999;
	int K1 = 0;
	int K2 = 0;
	for (int k = Start; k <= End; ++k)
	{
		if (TmpArr1[K1] <= TmpArr2[K2])
		{
			ArraySrc[k] = TmpArr1[K1];
			K1++;
		}
		else
		{
			ArraySrc[k] = TmpArr2[K2];
			K2++;
		}
	}
}

void AIntroductionToAlgorithm::MergeSort(TArray<int>& ArraySrc, const int Start, const int End)
{
	if (Start < End)
	{
		int Mid = (Start + End) / 2;
		MergeSort(ArraySrc, Start, Mid);
		MergeSort(ArraySrc, Mid + 1, End);
		Merge(ArraySrc, Start, Mid, End);
	}
}
