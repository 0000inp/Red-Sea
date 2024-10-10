#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAIActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIActions : public UInterface
{
	GENERATED_BODY()
};

class SUMMERPROJECT_API IAIActions
{
	GENERATED_BODY()

public:
	virtual void Attack() = 0;
};
