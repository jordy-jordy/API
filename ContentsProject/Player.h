#pragma once
#include <EngineCore/Actor.h>

enum class PlayerState
{
	Idle,
	Move,
	Idle_Anim
};

// ���� :
class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void RunSoundPlay();

	void LevelChangeStart();
	void LevelChangeEnd();

	void SetWallTileMap(class ATileMap* _TileMap)
	{
		WallTileMap = _TileMap;
	}

	void SetBomb(class ABomb* _Bomb)
	{
		Bomb = _Bomb;
	}

	FVector2D PosToTileIndex(FVector2D _Pos);


protected:

private:
	float Speed = 300.0f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRendererHead = nullptr;
	class USpriteRenderer* SpriteRendererBody = nullptr;

	float IdleAnimTime = 2.0f; // 2�� ���� �Է��� ������ ����Ǵ� Idle �ִϸ��̼�
	float IdleAnimTimer = 0.0f; // Idle �ִϸ��̼� Ÿ�̸�

	PlayerState CurPlayerState = PlayerState::Idle;
	void ChangeState(PlayerState CurPlayerState);

	void Idle_Anim(float _DeltaTime);

	void IdleStart();
	void Idle(float _DeltaTime);
	void MoveStart();
	void Move(float _DeltaTime);
	void PlaceBomb(float _DeltaTime);

	class ATileMap* WallTileMap = nullptr;

	class ABomb* Bomb = nullptr;

};

