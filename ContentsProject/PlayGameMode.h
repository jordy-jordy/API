#pragma once
#include <EngineCore/GameMode.h>

// ���� :
class APlayGameMode : public AGameMode
{
public:
	// constrcuter destructer
	APlayGameMode();
	~APlayGameMode();

	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;

	void BeginPlay();

	FVector2D WallTileMapLocation = { 96, 64 };

protected:
	void Tick(float _DeltaTime) override;
	void PlayerInit();
	void PlayTileMapInit();

private:
	class ATileMap* WallTileMap = nullptr;

};

