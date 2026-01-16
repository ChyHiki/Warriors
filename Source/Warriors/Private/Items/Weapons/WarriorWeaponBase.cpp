// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

// 设置默认值
AWarriorWeaponBase::AWarriorWeaponBase()
{
	// 不需要每帧调用 Tick，关闭以提高性能
	PrimaryActorTick.bCanEverTick = false;

	// 创建用于显示武器外观的静态网格组件，并设置为根组件
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// 创建用于命中检测的盒形碰撞体
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	// 将碰撞体附着到根组件（武器网格），使其随网格移动/旋转
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	// 设置盒子半尺寸（未缩放的半边长），这里使用 20 单位的立方体半尺寸
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	// 默认关闭碰撞，常见做法是在攻击时开启用于检测命中，结束后再关闭
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



