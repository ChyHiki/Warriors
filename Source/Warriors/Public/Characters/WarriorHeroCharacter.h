// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class WARRIORS_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()
public:
	AWarriorHeroCharacter();

protected:
	//~ Begin APawn Interface.
	//重写PossessedBy函数
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	// 重写 SetupPlayerInputComponent 方法以设置角色的输入组件
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
	void PostInitializeComponents();
private:

#pragma region Components
	// 使用 UPROPERTY 宏声明一个可在编辑器中可见的组件属性
	// VisibleAnywhere 表示该属性在编辑器的所有视图中都可见，但不能直接编辑
	// BlueprintReadOnly 表示该属性可以在蓝图中以只读方式访问
	// Category = "Camera" 表示该属性在编辑器中归类到 "Camera" 分类下
	// meta = (AllowPrivateAccess = "true") 允许蓝图或其他类通过反射访问这个私有属性
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom; // 弹簧臂组件，通常用于控制摄像机与角色之间的距离和角度

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera; // 跟随摄像机组件，用于显示角色视角

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;
#pragma endregion

	
#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;// 输入配置数据资产，用于定义角色的输入映射和行为
	// 输入处理函数，处理角色的移动输入
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
#pragma endregion

public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; };

};
