//©2023 Univio Entertainment. All rights reserved.


#include "./Kart.h"
#include "DrawDebugHelpers.h"

// Sets default values
AKart::AKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Force=GetActorForwardVector()*MaxDrivingForce*Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);
	//updating location
	ChangeVelocity(DeltaTime);
	//Drifting the car
	DrawDebugString(GetWorld(), FVector(0, 0, 100),this->get_role_as_string(GetLocalRole()), this, FColor::White,DeltaTime);
}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::MoveRight);
}

void AKart::Server_MoveForward_Implementation(float value)//Server Side replications
{
	this->Throttle = value;
}
bool AKart::Server_MoveForward_Validate(float value) { return true; };//Server Side replications
bool AKart::Server_MoveRight_Validate(float) { return true; };//Server Side replications

void AKart::Server_MoveRight_Implementation(float value)//Server Side replications
{
	this->SteeringThrottle = value;
}
void AKart::ChangeVelocity(float delta_time)
{
	FVector translation = this->Velocity * delta_time * 100.f;
	FHitResult hit;
	AddActorWorldOffset(translation, true, &hit);
	if (hit.IsValidBlockingHit())Velocity = FVector::ZeroVector;
}
;
FVector AKart::GetAirResistance()const
{

	return -Velocity.GetSafeNormal()*Velocity.SizeSquared() * DragCoffient;
};
FVector AKart::GetRollingResistance()const
{
	float normal_force = -GetWorld()->GetGravityZ() * Mass;
	return -Velocity.GetSafeNormal() * RollingCoffient * normal_force;
};

void AKart::ApplyRotation(float DeltaTime)
{
	float rotation_angle = (FVector::DotProduct(GetActorForwardVector(),Velocity)* DeltaTime)/(this->MinRotation  * SteeringThrottle);
	FQuat delta_rotation(GetActorUpVector(), (rotation_angle));
	//Adding rotation 
	AddActorWorldRotation(delta_rotation, true);
	if (Velocity.Size() > Drift)
		Velocity = (delta_rotation.operator-(FQuat(GetActorUpVector() * 0.9, 0.1f))).RotateVector(Velocity);


}

FString AKart::get_role_as_string(ENetRole role)const
{
	switch (role)
	{
	case ROLE_None:
		return "None";

	case ROLE_SimulatedProxy:
		return "SimulatedProxy";

	case ROLE_AutonomousProxy:
		return "AutonomousProxy";

	case ROLE_Authority:
		return "Authority";

	default:
		return "Error";

	}
}

void AKart::MoveForward(float value)//Client Side replications
{
	Throttle = value;
	Server_MoveForward(Throttle);
}

void AKart::MoveRight(float value)//Client Side replications
{
	SteeringThrottle = value;
	Server_MoveRight(SteeringThrottle);
}
