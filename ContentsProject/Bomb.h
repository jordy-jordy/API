#pragma once
#include <EngineCore/Actor.h>


enum class BombState
{
	Idle_Bomb = 0,
	Start_Exploding = 1,
	Running_Exploding = 2,
	End_Exploding = 3,
	Over = 4

};

class ABomb : public AActor
{
public:
	// constrcuter destructer
	ABomb();
	~ABomb();

	// delete Function
	ABomb(const ABomb& _Other) = delete;
	ABomb(ABomb&& _Other) noexcept = delete;
	ABomb& operator=(const ABomb& _Other) = delete;
	ABomb& operator=(ABomb&& _Other) noexcept = delete;

	class USpriteRenderer* SpriteRenderer;

	void SetWallTileMap(class ATileMap* _TileMap, FIntPoint _Index);

	void StartExplodeTimer();
	void StartDestroyTimer();


protected:
	void Tick(float _DeltaTime) override;
	void Bomb_Explode(float _DeltaTime);
	void Bomb_Destroy(float _DeltaTime);

private:
	class ATileMap* WallTileMap = nullptr;
	FIntPoint BombTileIndex;

	bool ExplodeTimer_On = false;
	float Explode_Start_Time = 2.0f; // ��ź ��ġ �� 4�� �ڿ� ��ź�� �����ϵ��� ����
	float Explodecheck_Start_Timer = 0.0f; // ��ź ���� Ÿ�̸�

	bool DestroyTimer_On = false;
	float Bomb_DestroyTime = 3.0f; // ��ź�� ���� �� 2�� �ڿ� ��ź�� ���������� ����
	float Bomb_DestroyTimer = 0.0f; // ��ź ���� Ÿ�̸�

	void ClearBombTile(); // ��ź ���� ���� Ÿ�ϸ� ������Ʈ

};

