// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"

/**
 * UPawnExtensionComponentBase
 * 用作 ActorComponent 的基类，提供方便的获取所属 Pawn / Controller 的辅助方法。
 *
 * 设计要点：
 * - 组件的所有者应为一个 `APawn`（或其子类），因此提供了模版化的 `GetOwningPawn<T>()` 以便在派生类中直接获取具体类型。
 * - 使用 `CastChecked` 做类型转换，若转换失败会在运行时断言（适合在开发阶段尽早发现问题）。
 * - 提供模版化的 `GetOwningController<T>()` 以便直接获取特定类型的控制器（例如 `APlayerController` 或自定义 Controller）。
 *
 * 注意：
 * - `GetOwningController` 假定 Pawn 已存在且被 Possess（即 `GetOwningPawn<APawn>()` 非空且有 Controller），调用方应确保在合适时机调用（例如 BeginPlay 后或 Possessed 后）。
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORS_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()


protected:
	// 获取拥有该组件的 Pawn，并将其转换为模板类型 T（T 必须继承自 APawn）。
	// 若转换失败，`CastChecked` 会触发断言（Debug/Development 下崩溃），用于尽早发现类型错误。
	template<class T>
	T* GetOwningPawn() const
	{
		// 静态断言：在编译期确保 T 可转换为 APawn，避免误用模板参数
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Parameter to GetPawn must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	// 非模板版，直接以 APawn 类型返回拥有者。
	// 等同于：`GetOwningPawn<APawn>()`
	APawn *GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	// 获取拥有 Pawn 的 Controller，并将其转换为模板类型 T（T 必须继承自 AController）。
	// 通过先获取 Pawn，再调用 Pawn 的 `GetController<T>()`。
	// 注意：如果 Pawn 为空或未被 Possess，则此调用可能访问空指针或返回空。
	template<class T>
	T* GetOwningController() const
	{
		// 静态断言：在编译期确保 T 可转换为 AController
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Parameter to GetController must be derived from AController");
		// 通过 APawn 的模板方法获取 Controller（会返回 nullptr 或指定类型的控制器）
		return GetOwningPawn<APawn>()->GetController<T>();
	}


};
