// Fill out your copyright notice in the Description page of Project Settings.


#include "HeathlComponent.h"


void UHeathlComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	
}



void UHeathlComponent::TakeDamage(FDamageData DamageData)
{
	//float takedDamageValue = DamageData.DamageValue;
	CurrentHealth -= DamageData.DamageValue;


	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if (OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(DamageData.DamageValue);
	}

}
float UHeathlComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHeathlComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}
void UHeathlComponent::AddHealth(float newHealth)
{
	CurrentHealth += MaxHealth;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}

