// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"


class UBoxComponent;

/**
 * AWarriorWeaponBase
 * 武器基类（Actor），包含显示网格和简单的碰撞体。
 * 该类用于实现道具/武器的基础功能（视觉表示 + 碰撞检测）。
 */
UCLASS()
class WARRIORS_API AWarriorWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	// 构造函数：设置默认组件、初始属性等
	AWarriorWeaponBase();


protected:
	// 可见的静态网格组件，表示武器的外观（在编辑器和蓝图中只读）
	// VisibleAnywhere: 在任何视图都可见但不可编辑（实例/蓝图继承可见）
	// BlueprintReadOnly: 在蓝图中只读
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;

	// 用于近战/触发检测的盒形碰撞组件（例如命中检测）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

public:
	// 内联访问器：返回武器网格组件指针（不可修改）
	// FORCEINLINE 提示编译器尽量内联以降低调用开销
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	// 内联访问器：返回武器碰撞盒组件指针（不可修改）
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
