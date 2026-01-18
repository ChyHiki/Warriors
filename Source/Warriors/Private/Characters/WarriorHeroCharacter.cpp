// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"

#include "WarriorDebugHelper.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;//与人物右上方
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation=true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull()) {
		
		if(UDataAsset_StartUpDataBase* LoadedData= CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
		}
	}


	/*if(WarriorAbilitySystemComponent && WarriorAttributeSet)
	{
		const FString ASCText = FString::Printf(TEXT
			("Owner Actor : %s, AvatarActor: %s"), 
			*WarriorAbilitySystemComponent->GetOwnerActor()->GetActorLabel(), 
			*WarriorAbilitySystemComponent->GetAvatarActor()->GetActorLabel());

		Debug::Print(TEXT("AbilitySystemComponent Valid. ") + ASCText, FColor::Green );
		Debug::Print(TEXT("AttributeSet Valid. "));
	}*/
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//确保 InputConfigDataAsset 已分配（非空）。若为空则触发断言并终止（在非 Shipping 配置中会崩溃），并打印指定信息
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
	//获取当前角色的控制器，并将其转换为 APlayerController 类型，然后调用 GetLocalPlayer() 方法获取关联的本地玩家对象
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	//从本地玩家对象中获取 UEnhancedInputLocalPlayerSubsystem 子系统的实例，该子系统用于管理增强输入功能
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	//确保子系统实例已成功获取（非空）。若为空则触发断言并终止
	check(Subsystem);
	//将默认的输入映射上下文添加到子系统中，优先级为 0
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	//将传入的 PlayerInputComponent 强制转换为 UWarriorInputComponent 类型，如果转换失败则触发断言
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);
	//绑定移动输入操作，当触发指定的输入标签时调用 Input_Move 方法
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);

}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	//if (MoveComp)
	//{
	//	// 再次确保启用“面朝向移动方向”
	//	MoveComp->bOrientRotationToMovement = true;
	//	// 确保旋转速率合适
	//	MoveComp->RotationRate = FRotator(0.f, 500.f, 0.f);
	//}

	//// 确保控制器旋转不会直接控制 Actor（避免冲突）
	//bUseControllerRotationYaw = false;

	//UE_LOG(LogTemp, Display, TEXT("BeginPlay: bOrient=%d bUseControllerRotationYaw=%d RotRateYaw=%f"),
	//	MoveComp ? MoveComp->bOrientRotationToMovement : 0,
	//	bUseControllerRotationYaw ? 1 : 0,
	//	MoveComp ? MoveComp->RotationRate.Yaw : 0.f);
}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// 获取输入的二维向量值，表示角色的移动方向和强度
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	// 获取当前角色的控制器
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
	if (MovementVector.Y != 0.f)
	{
		// 计算角色在前后方向上的移动方向向量
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		// 添加角色在前后方向上的移动输入
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		// 计算角色在左右方向上的移动方向向量
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		// 添加角色在左右方向上的移动输入
		AddMovementInput(RightDirection, MovementVector.X);
	}

	// 运行时更详细调试信息，方便定位为何不旋转
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	const bool bGrounded = MoveComp ? MoveComp->IsMovingOnGround() : false;
	const int32 MoveMode = MoveComp ? static_cast<int32>(MoveComp->MovementMode) : -1;
	const bool bOrient = MoveComp ? MoveComp->bOrientRotationToMovement : false;
	const float RotYaw = MoveComp ? MoveComp->RotationRate.Yaw : 0.f;

}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//将标签作为参数传递到WarriorAbilitySystemComponent，以便在按下时触发相应的能力
void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}
void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AWarriorHeroCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    UE_LOG(LogTemp, Display, TEXT("This instance class = %s"), *GetClass()->GetName());
    UE_LOG(LogTemp, Display, TEXT("C++ base class = %s"), *StaticClass()->GetName());

    // 打印实例上所有 CharacterMovementComponent（通常只有一个）
    TArray<UActorComponent*> Comps;
    GetComponents(Comps);
    for (UActorComponent* Comp : Comps)
    {
        if (UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(Comp))
        {
            UE_LOG(LogTemp, Display, TEXT("Instance MovementComp Name=%s bOrient=%d RotRate=(P=%f Y=%f R=%f)"),
                *CMC->GetName(),
                CMC->bOrientRotationToMovement ? 1 : 0,
                CMC->RotationRate.Pitch, CMC->RotationRate.Yaw, CMC->RotationRate.Roll);

			UE_LOG(LogTemp, Display, TEXT("RotationRate = P=%f Y=%f R=%f"),
				GetCharacterMovement()->RotationRate.Pitch,
				GetCharacterMovement()->RotationRate.Yaw,
				GetCharacterMovement()->RotationRate.Roll);
        }
    }

    // 打印该类默认对象（CDO）上的 MovementComponent 值
    if (AWarriorHeroCharacter* CDOObj = Cast<AWarriorHeroCharacter>(GetClass()->GetDefaultObject()))
    {
        if (UCharacterMovementComponent* CDO_MoveComp = CDOObj->GetCharacterMovement())
        {
            UE_LOG(LogTemp, Display, TEXT("CDO MovementComp Name=%s bOrient=%d RotRate=(P=%f Y=%f R=%f)"),
                *CDO_MoveComp->GetName(),
                CDO_MoveComp->bOrientRotationToMovement ? 1 : 0,
                CDO_MoveComp->RotationRate.Pitch, CDO_MoveComp->RotationRate.Yaw, CDO_MoveComp->RotationRate.Roll);

			UE_LOG(LogTemp, Display, TEXT("RotationRate = P=%f Y=%f R=%f"),
				GetCharacterMovement()->RotationRate.Pitch,
				GetCharacterMovement()->RotationRate.Yaw,
				GetCharacterMovement()->RotationRate.Roll);
        }
    }
}
