#include "DEBUG.h"


void DEBUG::print(FString text, float time, FColor color)
{
	if(GEngine){GEngine->AddOnScreenDebugMessage(-1, time, color, text);}
}


