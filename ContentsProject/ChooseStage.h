#pragma once
#include <EngineCore/Actor.h>

// ���� :
class UChooseStage : public AActor
{
public:
	// constrcuter destructer
	UChooseStage();
	~UChooseStage();

	// delete Function
	UChooseStage(const UChooseStage& _Other) = delete;
	UChooseStage(UChooseStage&& _Other) noexcept = delete;
	UChooseStage& operator=(const UChooseStage& _Other) = delete;
	UChooseStage& operator=(UChooseStage&& _Other) noexcept = delete;

	class USpriteRenderer* Background = nullptr;
	class USpriteRenderer* Basic_UI00 = nullptr;
	class USpriteRenderer* Basic_UI01 = nullptr;

	class USpriteRenderer* CHOOSE_00 = nullptr;
	class USpriteRenderer* CHOOSE_01 = nullptr;

	class USpriteRenderer* BLACKMAN = nullptr;
	class USpriteRenderer* BOMB_MAN = nullptr;

	class USpriteRenderer* CIRCLE = nullptr;

	bool GetIsStageONE()
	{
		return IsStageONE;
	}

	bool GetIsStageTWO()
	{
		return IsStageTWO;
	}

	void DestroyChoose()
	{
		Destroy();
	}


protected:
	void BeginPlay();
	void Tick(float _DeltaTime);


private:
	FVector2D BG_SIZE = { 608, 448 };

	bool IsDownMoving = false; // �Ʒ� ������ ���� �÷���
	bool IsUpMoving = false; // �� ������ ���� �÷���
	float MoveSpeed = 250.0f; // �̵� �ӵ�
	float StageOnePos = 193.0f; // �����̵� ��� ��ġ
	float StageTwoPos = 354.0f; // ��Ƽ ��� ��ġ

	bool IsStageONE = false;
	bool IsStageTWO = false;

};
