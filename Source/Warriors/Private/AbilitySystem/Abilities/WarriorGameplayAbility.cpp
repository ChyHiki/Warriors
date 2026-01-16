// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

// 当能力被赋予给某个主体（AbilitySystemComponent）时调用
// 常用于在赋予时立即根据策略激活能力或做初始化
void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// 如果激活策略是 OnGiven（赋予时自动激活）
	if(AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		// 确保 ActorInfo 有效且该能力当前尚未处于激活状态
		if(ActorInfo && !Spec.IsActive())
		{
			// 使用 AbilitySystemComponent 尝试按 Spec.Handle 激活该能力
			// 注意：TryActivateAbility 可能会检查冷却、消耗、条件等
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

// 当能力结束时调用（无论是正常结束还是被取消）
// 可以在此处做清理工作，例如移除临时效果/销毁资源/撤销赋予时的状态
void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 如果激活策略是 OnGiven（赋予时自动激活），能力结束时需要清理能力 Spec
	// 这样可以在能力自动激活后确保其不会残留在 AbilitySystem 中
	if (AbilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven) 
	{
		if (ActorInfo)
		{
			// ClearAbility 会从 AbilitySystemComponent 中移除对应的能力 Spec（通过 Handle）
			// 这通常用于自动管理临时赋予的能力
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UWarriorGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UWarriorAbilitySystemComponent* UWarriorGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{	
	return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
	