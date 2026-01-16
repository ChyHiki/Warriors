// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorGameplayAbility.generated.h"


class UPawnCombatComponent;
class UWarriorAbilitySystemComponent;


UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	// 仅在显式触发（例如按键或逻辑触发）时激活
	OnTriggered,

	// 在能力被赋予（给定）给主体时自动激活
	OnGiven,
};

/**
 * UWarriorGameplayAbility
 * 基于 UE GameplayAbility 的游戏能力基类。
 * 提供按策略（OnTriggered / OnGiven）在被赋予时自动激活或在结束时清理的逻辑。
 */
UCLASS()
class WARRIORS_API UWarriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.

	// 当能力被赋予给某个主体（AbilitySystemComponent）时调用。
	// 常用于在赋予时根据 AbilityActivationPolicy 决定是否立即激活能力。
	// 参数：
	//  - ActorInfo: 所属主体的能力信息（包含 AbilitySystemComponent、Owner、Avatar 等）。
	//  - Spec: 与该能力实例相关的描述符（包括 Handle、等级、标签等）。
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// 当能力结束（正常结束或被取消）时调用。
	// 在策略为 OnGiven 时，会在结束时清理该能力的 Spec（从 AbilitySystemComponent 中移除）。
	// 参数：
	//  - Handle: 能力实例的句柄，用于在 AbilitySystem 中定位该能力 Spec。
	//  - ActorInfo: 能力所属的 Actor 信息（可能为空，需检查）。
	//  - ActivationInfo: 激活相关信息（例如是否由客户端请求等）。
	//  - bReplicateEndAbility: 是否需要将结束事件复制到客户端。
	//  - bWasCancelled: 能力是否被取消而非正常完成。
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
	// 能力的激活策略：
	//  - OnTriggered: 仅在外部触发时激活（默认）
	//  - OnGiven: 在能力被赋予时自动激活，并在结束时移除能力 Spec
	EWarriorAbilityActivationPolicy AbilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;

};
