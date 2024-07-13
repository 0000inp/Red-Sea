#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SubmarineControl.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USubmarineControl : public UInterface
{
	GENERATED_BODY()
};

/**
* 
*/
class SUMMERPROJECT_API ISubmarineControl
{
	GENERATED_BODY()

public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
	virtual void ControlRotation(FVector2d LookVector) = 0;
	virtual void ControlMovement(FVector2d MoveVector) = 0;

	virtual void BeingUse() = 0;
	virtual void StopBeingUse() = 0;
};
