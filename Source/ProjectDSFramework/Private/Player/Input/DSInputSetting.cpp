// SDH, All rights reserved. (2021 ~ )


#include "Player/Input/DSInputSetting.h"
#include "InputAction.h"

UInputAction* UDSInputSetting::GetInputAction(FName InInputActionName) const
{
	UInputAction* const* TargetInputAction = InputActionTable.Find(InInputActionName);
	if (TargetInputAction != nullptr)
	{
		return *TargetInputAction;
	}

	return nullptr;
}
