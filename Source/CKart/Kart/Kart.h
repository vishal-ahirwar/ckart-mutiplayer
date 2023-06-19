//©2023 Univio Entertainment. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

UCLASS()
class CKART_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	FString get_role_as_string(ENetRole role)const;
	FVector Velocity{};
	UFUNCTION(Server, reliable,WithValidation)
	void Server_MoveForward(float value);
	UFUNCTION(Server,reliable,WithValidation)
	void Server_MoveRight(float value);

	void MoveForward(float value);

	void MoveRight(float value);

	UPROPERTY(EditAnyWhere)
		float Speed{ 20 };

	UPROPERTY(EditAnyWhere)
		float DragCoffient{ 16.f };

	UPROPERTY(EditAnyWhere)
		float RollingCoffient{ 0.15f };
	UPROPERTY(EditAnyWhere)
		float MaxDrivingForce{ 100000 };

	float Throttle;
	float SteeringThrottle;

	UPROPERTY(EditAnyWhere)
		float MinRotation{ 10 };

	UPROPERTY(EditAnyWhere)
		float Mass{ 4500 };
	UPROPERTY(EditAnyWhere)
		float Drift{ 4500 };
	void ChangeVelocity(float delta_time);
	FVector GetAirResistance()const;
	FVector GetRollingResistance()const;
	void ApplyRotation(float);
};