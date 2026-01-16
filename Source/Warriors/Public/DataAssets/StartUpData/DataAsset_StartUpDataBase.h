// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWarriorGameplayAbility;
class UWarriorAbilitySystemComponent;

/**
 * UDataAsset_StartUpDataBase
 * 启动数据的 DataAsset 基类，用于在角色/实体初始化时赋予能力（Abilities）。
 * 该 DataAsset 保持两类能力列表：被赋予后立即激活的能力（ActivateOnGivenAbilities）
 * 以及作为被动/反应式存在的能力（ReactiveAbilities）。
 *
 * 模式说明：
 * - 将能力类（TSubclassOf<UWarriorGameplayAbility>）保存到数组中，而不是能力实例。
 * - 在运行时通过 GiveToAbilitySystemComponent 将这些能力授权（Grant）到目标的
 *   UWarriorAbilitySystemComponent 上，通常在角色 Possessed / BeginPlay 等时机调用。
 */
UCLASS()
class WARRIORS_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	

public:
	// 将本 DataAsset 中的能力赋予给指定的 AbilitySystemComponent。
	// 参数：
	//  - InWarriorASCToGive: 目标的 AbilitySystemComponent，能力会被添加到该组件上。
	//  - ApplyLevel: 赋予时应用的能力等级（默认 1），通常用于设置能力强度/等级。
	// 注意：该函数负责调用内部的 GrantAbilities 来分发 ActivateOnGivenAbilities 与 ReactiveAbilities。
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);


protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	// 存储在能力被赋予时应立即激活的能力类。
	// 这些能力在被赋予（Give）时通常会自动触发（例如通过 AbilityActivationPolicy = OnGiven）。
	TArray< TSubclassOf <UWarriorGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	// 存储作为被动或反应性能力的类，这类能力通常不会在赋予时立即触发，
	// 而是作为可用能力存在于 AbilitySystem 中，根据条件被触发或由系统处理。
	TArray< TSubclassOf <UWarriorGameplayAbility>> ReactiveAbilities;

	// 内部通用函数：将指定的能力类数组授权到目标 AbilitySystemComponent。
	// 参数：
	//  - InAbilitiesToGive: 要授权的能力类数组（TSubclassOf<UWarriorGameplayAbility>）。
	//  - InWarriorASCToGive: 目标的 AbilitySystemComponent。
	//  - ApplyLevel: 赋予时使用的能力等级。
	// 该函数通常负责为每个能力创建 Spec 并调用目标 ASC 的相关授权接口（例如 GiveAbility）。
	void GrantAbilities(const TArray< TSubclassOf <UWarriorGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

};
