// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//如果动态标签不包含输入标签，则跳过该能力
		if (!AbilitySpec.DynamicAbilityTags.HasTag(InInputTag)) continue;
		
		TryActivateAbility(AbilitySpec.Handle);//尝试激活该能力
	}

}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{

}
