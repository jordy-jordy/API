#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/EngineAPICore.h>

APlayer::APlayer()
{
	SetActorLoaction({ 200, 200 });
	SetActorScale({ 100, 100 });
}

APlayer::~APlayer()
{
}


void APlayer::BeginPlay()
{
}

void APlayer::Tick()
{
	AddActorLoaction(FVector2D::DOWN * 0.0001f);
}