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

	FVector2D InvertLOC(FVector2D _Dir);

	bool CanMove(FVector2D _NextPOS_Win, FVector2D _NextPOS_Local);

	bool ThereIsTILE = false;
	bool ThereIsOutOfMap = false;

	class USpriteRenderer* SpriteRendererHead = nullptr;
	class USpriteRenderer* SpriteRendererBody = nullptr;

	void SetPlayMode(class APlayGameMode* _data);


protected:

private:
	float Speed = 300.0f;
	int MySpriteIndex = 0;
	int BOMBPOWER = 3;

	float IdleAnimTime = 2.0f; // 2�� ���� �Է��� ������ ����Ǵ� Idle �ִϸ��̼�
	float IdleAnimTimer = 0.0f; // Idle �ִϸ��̼� Ÿ�̸�

	PlayerState CurPlayerState = PlayerState::Idle;
	void ChangeState(PlayerState CurPlayerState);

	void Idle_Anim(float _DeltaTime);

	void IdleStart();
	void Idle(float _DeltaTime);
	void MoveStart();
	void Move(float _DeltaTime);
	void PlaceBomb();

	class ATileMap* WallTileMap = nullptr;

	class ABomb* Bomb = nullptr;

	int TILE_INDEX_MIN_X = 0;
	int TILE_INDEX_MAX_X = 13;
	int TILE_INDEX_MIN_Y = 0;
	int TILE_INDEX_MAX_Y = 11;

	FVector2D CUSTOM_VECTOR_ZERO  = { 0, 0 };
	FVector2D CUSTOM_VECTOR_UP    = { 0, -1 };
	FVector2D CUSTOM_VECTOR_DOWN  = { 0, 1 };
	FVector2D CUSTOM_VECTOR_LEFT  = { -1, 0 };
	FVector2D CUSTOM_VECTOR_RIGHT = { 1, 0 };

	FVector2D SIZE_LEFTTOP  = { -15, -15 };
	FVector2D SIZE_LEFTBOT  = { -15,  15 };
	FVector2D SIZE_RIGHTTOP = {  15, -15 };
	FVector2D SIZE_RIGHTBOT = {  15,  15 };

	bool IsCleared = false;

	class APlayGameMode* PlayMode = nullptr;
};

