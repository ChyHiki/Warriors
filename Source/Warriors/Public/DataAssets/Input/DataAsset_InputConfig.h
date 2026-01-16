// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)// 可在蓝图中使用
struct FWarriorInputActionConfig
{
	GENERATED_BODY()// Unreal 反射系统生成的代码

public:
	// 输入动作的标签（如 "Input.Attack"）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	// 对应的 UInputAction（如攻击、跳跃等具体输入行为）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;


	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}

};
/**
 * 类：战士输入配置数据资产
 * 存储所有本地输入动作的配置，并提供查找功能
 */
UCLASS()
class WARRIORS_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 默认输入映射上下文（定义按键绑定）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	// 所有本地输入动作的数组（按标签分类）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> NativeInputActions;

	/**
	*函数：通过标签查找对应的 UInputAction
	*@param InInputTag 输入动作的标签（如 "Input.Attack"）
	*@return 对应的 UInputAction，如果未找到则返回 nullptr
	*/
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> AbilityInputActions;
};