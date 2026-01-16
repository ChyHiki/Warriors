// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

/**
 * 将本 DataAsset 中记录的能力赋予到目标的 AbilitySystemComponent 上。
 * 该函数会依次处理 ActivateOnGivenAbilities（赋予时可能自动激活的能力）
 * 和 ReactiveAbilities（被动/反应式能力），并将它们授权给 InWarriorASCToGive。
 *
 * @param InWarriorASCToGive: 目标的 WarriorAbilitySystemComponent，不能为空（使用 check 验证）。
 * @param ApplyLevel: 为每个被赋予的能力设置的等级（默认为 1）。
 */
void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// 确保目标 ASC 有效；在调试/开发构建中会触发断言以便尽早发现错误调用
	check(InASCToGive);	

	// 授予需要在赋予时激活的能力
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	// 授予被动或反应能力
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}

/**
 * 将给定的能力类数组转换为 FGameplayAbilitySpec 并调用目标 ASC 的 GiveAbility 以实际授权能力。
 *
 * 实现要点：
 * - 跳过空的输入数组或空的类引用。
 * - 使用 AbilitySpec.SourceObject 保存能力的来源（这里设置为 ASC 的 Avatar，便于追踪来源）。
 * - 将 AbilitySpec.Level 设置为传入的 ApplyLevel。
 *
 * @param InAbilitiesToGive: 需要授权的能力类列表（TSubclassOf<UWarriorGameplayAbility>）。
 * @param InWarriorASCToGive: 目标的 WarriorAbilitySystemComponent，用于调用 GiveAbility。
 * @param ApplyLevel: 为每个能力 Spec 指定的等级。
 */
void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	// 如果没有任何能力需要授权，直接返回
	if(InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for(const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		// 跳过无效的类引用（可能在编辑器中被删除或未设置）
		if(!Ability){continue;}
		
		// 构造 AbilitySpec：包含能力类、等级、来源等信息
		FGameplayAbilitySpec AbilitySpec(Ability);
		// 将来源对象设置为目标 ASC 的 Avatar Actor（通常是角色或 Pawn），便于运行时追踪/调试
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		// 设置能力等级（可以影响能力效果/数值）
		AbilitySpec.Level = ApplyLevel;

		// 将能力 Spec 授权给目标 AbilitySystemComponent（会在 ASC 内部创建实例/句柄）
		InASCToGive->GiveAbility(AbilitySpec);
	}

}
