#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "RHICommandList.h"
/*
class FBoidCompute : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FBoidCompute);
	SHADER_USE_PARAMETER_STRUCT(FBoidCompute, FGlobalShader);

public:
	FBoidCompute(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		
	}
	
	// Shader parameters
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER(uint32, NumElements)
		SHADER_PARAMETER_SRV(StructuredBuffer<float>, InputBuffer)
		SHADER_PARAMETER_UAV(RWStructuredBuffer<float>, OutputBuffer)
	END_SHADER_PARAMETER_STRUCT()
};*/
