#include "PreCompile.h"
#include "Result.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"
#include "PlayGameMode.h"


AResult::AResult()
{
	STAGECLEAR = CreateDefaultSubObject<USpriteRenderer>();
	STAGECLEAR->SetSprite("01_StageClear");
	STAGECLEAR->SetOrder(ERenderOrder::UI_OVERFADE);
	STAGECLEAR->SetSpriteScale(1.0f);
	STAGECLEAR->SetPivotType(PivotType::Top);
	STAGECLEAR->SetComponentLocation({ 303, 55 });
	STAGECLEAR->CreateAnimation("StageClear", "01_StageClear", 0, 5, 0.15f, true);
	STAGECLEAR->ChangeAnimation("StageClear");

	RESULTINFO = CreateDefaultSubObject<USpriteRenderer>();
	RESULTINFO->SetSprite("STAGECLEAR_INFO.png");
	RESULTINFO->SetOrder(ERenderOrder::UI_OVERFADE);
	RESULTINFO->SetSpriteScale(1.0f);
	RESULTINFO->SetPivotType(PivotType::Center);
	RESULTINFO->SetComponentLocation(RESULTINFO->GetComponentScale().Half());

	RESULTBBMS = CreateDefaultSubObject<USpriteRenderer>();
	RESULTBBMS->SetSprite("STAGECLEAR_BBMs.png");
	RESULTBBMS->SetOrder(ERenderOrder::UI_OVERFADE);
	RESULTBBMS->SetSpriteScale(1.0f);
	RESULTBBMS->SetPivotType(PivotType::Top);
	RESULTBBMS->SetComponentLocation(BBMs_CurPos);
	RESULTBBMS->SetActive(false);
};


AResult::~AResult()
{

};


void AResult::BeginPlay()
{
	Super::BeginPlay();
}


void AResult::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (PlayMode->GetShowedAllScore() == true)
	{
		RESULTBBMS->SetActive(true);

		// �̵� �ӵ� ����
		float MoveSpeed = 30.0f * _DeltaTime;
		float XMoveSpeed = 680.0f * _DeltaTime; // X�� �̵� �ӵ�

		// Y�� �ִϸ��̼� ó��
		if (IsMoved == false) // �Ʒ��� �̵� ��
		{
			BBMs_CurMoveSize += MoveSpeed;

			if (BBMs_CurMoveSize >= BBMs_TargetDownMoveSize)
			{
				BBMs_CurMoveSize = BBMs_TargetDownMoveSize;
				IsMoved = true; // ���� ��ȯ
			}
		}
		else // ���� �̵� ��
		{
			BBMs_CurMoveSize -= MoveSpeed;

			if (BBMs_CurMoveSize <= BBMs_TargetUpMoveSize)
			{
				BBMs_CurMoveSize = BBMs_TargetUpMoveSize;
				IsMoved = false; // ���� ��ȯ
			}
		}

		// X�� �ִϸ��̼� ó��
		if (BBMs_CurPos.X > BBMs_TargetXMoveSize)
		{
			BBMs_CurPos.X -= XMoveSpeed;

			if (BBMs_CurPos.X < BBMs_TargetXMoveSize)
			{
				BBMs_CurPos.X = BBMs_TargetXMoveSize; // ��ǥ ��ġ�� �����ϸ� ����
			}
		}

		// ���� ��ġ ������Ʈ
		RESULTBBMS->SetComponentLocation({ BBMs_CurPos.X, BBMs_CurPos.Y + BBMs_CurMoveSize });
	}
}


void AResult::SetMode(APlayGameMode* _data)
{
	PlayMode = _data;
}

