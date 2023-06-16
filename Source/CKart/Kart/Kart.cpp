//©2023 Univio Entertainment. All rights reserved.


#include "./Kart.h"

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
	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;
	float rotation_angle = this->RotationPerSecond * DeltaTime * SteeringThrottle;
	FQuat delta_rotation(GetActorUpVector(), FMath::DegreesToRadians(rotation_angle));
	//Adding rotation 
	AddActorWorldRotation(delta_rotation, true);

	//updating location
	ChangeVelocity(DeltaTime);
	//Drifting the car
	if (Velocity.Size() > Drift)
		Velocity = (delta_rotation.operator-(FQuat(GetActorUpVector() * 0.9, 0.1f))).RotateVector(Velocity);
	else
		Velocity = delta_rotation.RotateVector(Velocity);

}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKart::MoveRight);
}

void AKart::MoveForward(float value)
{
	this->Throttle = value;
}
void AKart::MoveRight(float value)
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