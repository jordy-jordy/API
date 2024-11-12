#pragma once
#include <EngineCore/Actor.h>


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

	void SetPower(int _Power);

protected:
	void Tick(float _DeltaTime) override;
	void Bomb_Explode(float _DeltaTime);
	void Bomb_Destroy(float _DeltaTime);

private:
	class ATileMap* WallTileMap = nullptr;
	FIntPoint BombTileIndex;

	bool ExplodeTimer_On = false;
	float Explode_Start_Time = 2.0f; // ��ź ��ġ �� 2�� �ڿ� ��ź�� �����ϵ��� ����
	float Explodecheck_Start_Timer = 0.0f; // ��ź ���� Ÿ�̸�

	bool DestroyTimer_On = false;
	float Bomb_DestroyTime = 3.0f; // ��ź�� ���� �� 3.0�� �ڿ� ��ź�� ���������� ����
	float Bomb_DestroyTimer = 0.0f; // ��ź ���� Ÿ�̸�

	void ClearBombTile(); // ��ź ���� ���� Ÿ�ϸ� ������Ʈ

	int CurBombPower = 1; // ���� ��ź�� �Ŀ�

	//// ���� ������
	//class USpriteRenderer* Explode_Up = nullptr;
	//class USpriteRenderer* Explode_Left = nullptr;
	//class USpriteRenderer* Explode_Down = nullptr;
	//class USpriteRenderer* Explode_Right = nullptr;

	//class USpriteRenderer* Explode_Up_Mid = nullptr;
	//class USpriteRenderer* Explode_Left_Mid = nullptr;
	//class USpriteRenderer* Explode_Down_Mid = nullptr;
	//class USpriteRenderer* Explode_Right_Mid = nullptr;

	USpriteRenderer* Explode_Center;

	FIntPoint Explosion_Index_Up = { BombTileIndex.X, BombTileIndex.Y - 1 };
	FIntPoint Explosion_Index_Left = { BombTileIndex.X - 1, BombTileIndex.Y };
	FIntPoint Explosion_Index_Down = { BombTileIndex.X, BombTileIndex.Y + 1 };
	FIntPoint Explosion_Index_Right = { BombTileIndex.X + 1, BombTileIndex.Y };


};

