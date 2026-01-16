// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/DataAsset_InputConfig.h"

/**​
* 函数实现：通过标签查找对应的 UInputAction
* @param InInputTag 输入动作的标签（如 "Input.Attack"）
* @return 对应的 UInputAction，如果未找到则返回 nullptr
*/
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	// 遍历所有本地输入动作配置
	for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// 检查标签是否匹配，并且 InputAction 有效
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return  InputActionConfig.InputAction;// 返回匹配的 UInputAction
		}
	}
	return nullptr;//未找到匹配项，返回空指针
}
