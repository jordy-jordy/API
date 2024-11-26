#include "PreCompile.h"
#include "TitleGameMode.h"

#include <string>

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>

#include "TitleLogo.h"
#include "CoinInsert.h"
#include "ChooseStage.h"
#include "Score.h"
#include "ContentsEnum.h"
#include "ChooseStage.h"
#include "Transit_Ani.h"
#include "Fade.h"


ATitleGameMode::ATitleGameMode()
{
}

ATitleGameMode::~ATitleGameMode()
{
}


void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// �׿����� �ΰ� ��� ����
	TITLE = GetWorld()->SpawnActor<UTitleLogo>();

	// ���� ���� ��� ����
	COININSERT = GetWorld()->SpawnActor<UCoinInsert>();
	COININSERT->SetActive(false);
	COININSERT->SetScore_Coin(COINs);

	// �������� ���� ��� ����
	CHOOSE = GetWorld()->SpawnActor<UChooseStage>();
	CHOOSE->SetActive(false);

	// ��ȯ �ִϸ��̼� ��� ����
	TRANSIT = GetWorld()->SpawnActor<ATransit_Ani>();
	TRANSIT->SetActive(false);

	// ���ھ�(����) ����
	COINs = GetWorld()->SpawnActor<AScore>();
	COINs->SetTextSpriteName("Title_countdown_24x24.png");
	COINs->SetActorLocation({ 600, 440 });
	COINs->SetTextScale({ 16, 16 });
	COINs->SetAlignment(AScore::Alignment::Right);
	COINs->SetOrder(ERenderOrder::TEXT_UI);
	COINs->SetDigitCount(DIGITCOUNT_Coins);
	COINs->SetValue(COIN_NUMBER);
	COINs->SetActive(false);

	// ���ھ�(���� �ð�) ���� < �������� ���� ��� >
	TIMEs_StageChoose = GetWorld()->SpawnActor<AScore>();
	TIMEs_StageChoose->SetTextSpriteName("BlueCount.png");
	TIMEs_StageChoose->SetActorLocation({ 502, 104 });
	TIMEs_StageChoose->SetTextScale({ 32, 32 });
	TIMEs_StageChoose->SetAlignment(AScore::Alignment::Left);
	TIMEs_StageChoose->SetOrder(ERenderOrder::TEXT_UI);
	TIMEs_StageChoose->SetDigitCount(DIGITCOUNT_StageChooseTime);
	TIMEs_StageChoose->SetActive(true);

	// ���̵� ����
	Actor_Fade = GetWorld()->SpawnActor<AFade>();
	Actor_Fade->SetFadeSpeed(1.5f);
	Actor_Fade->SetActive(false);

	// ���� �̹��� (����4, ũ����) ����
	LEVEL4 = CreateDefaultSubObject<USpriteRenderer>();
	LEVEL4->SetSprite("Level-4_112x16.png");
	LEVEL4->SetOrder(ERenderOrder::BACKGROUND1);
	LEVEL4->SetSpriteScale(1.0f);
	LEVEL4->SetPivotType(PivotType::Top);
	LEVEL4->SetComponentLocation({ 312, 432 });
	LEVEL4->SetActive(false);

	CREDIT = CreateDefaultSubObject<USpriteRenderer>();
	CREDIT->SetSprite("Credits_112x16.png");
	CREDIT->SetOrder(ERenderOrder::BACKGROUND1);
	CREDIT->SetSpriteScale(1.0f);
	CREDIT->SetPivotType(PivotType::Top);
	CREDIT->SetComponentLocation({ 488, 432 });
	CREDIT->SetActive(false);

}

void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (InitCurState() == SCENES::NONE)
	{
		return;
	}

	if (InitCurState() == SCENES::ANI_TRANSIT)
	{
		if (ISFIRSTFADE_ANI_TRANSIT == false)
		{
			Actor_Fade->SetActive(false);
			ISFIRSTFADE_ANI_TRANSIT = true;
		}

		if (TRANSIT != nullptr)
		{
			TRANSIT->SetActive(true);

			if (TRANSIT->MAINRENDERER->IsCurAnimationEnd() == true)
			{
				Actor_Fade->SetActive(true);
				if (ISFADING_ANI_TRANSIT == false)
				{
					Actor_Fade->FadeIn();
					ISFADING_ANI_TRANSIT = true;
				}

				TimeEventer.PushEvent(2.0f, std::bind(&ATitleGameMode::OpenPlayLevel, this), false, false);
				TimeEventer.PushEvent(2.0f, std::bind(&ATransit_Ani::DestroyChoose, TRANSIT), false, false);
			}

			if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
			{
				Actor_Fade->SetActive(true);
				if (ISFADING_ANI_TRANSIT == false)
				{
					Actor_Fade->FadeIn();
					ISFADING_ANI_TRANSIT = true;
				}

				TimeEventer.PushEvent(2.0f, std::bind(&ATitleGameMode::OpenPlayLevel, this), false, false);
				TimeEventer.PushEvent(2.0f, std::bind(&ATransit_Ani::DestroyChoose, TRANSIT), false, false);
			}
		}

		if (TRANSIT->IsDestroy() == true)
		{
			TRANSIT = nullptr;
		}

	}

	if (InitCurState() == SCENES::CHOOSE_STAGE)
	{
		TIMEs_StageChoose->SetActive(true);
		StageChooseTime_NUMBER -= _DeltaTime;
		TIMEs_StageChoose->SetValue(static_cast<int>(StageChooseTime_NUMBER));

		if (StageChooseTime_NUMBER < 0)
		{
			StageChooseTime_NUMBER = 0.0f;
		}

		if (ISFIRSTFADE_CHOOSE_STAGE == false)
		{
			// ���̵� �ƿ� ȿ�� ����
			Actor_Fade->SetActive(true);
			Actor_Fade->FadeOut();
			ISFIRSTFADE_CHOOSE_STAGE = true;
		}

		if (CHOOSE != nullptr)
		{
			// �������� ���� ȭ�� Ȱ��ȭ
			CHOOSE->SetActive(true);

			if (CHOOSE->GetIsStageONE() == true && UEngineInput::GetInst().IsDown(VK_SPACE))
			{
				// 1.5�� �ڿ� ���̵� �� ����
				TimeEventer.PushEvent(1.5f, std::bind(&AFade::FadeIn, Actor_Fade), false, false);

				// 3�� �ڿ� �������� ���� ����
				TimeEventer.PushEvent(3.0f, std::bind(&UChooseStage::DestroyChoose, CHOOSE), false, false);

				return;
			}
		}

		// CHOOSE �� ���� �Ǿ����� �����͵� null �� �ʱ�ȭ
		if (CHOOSE->IsDestroy() == true)
		{
			TIMEs_StageChoose->SetActive(false);
			CHOOSE = nullptr;
			ISPASS_CHOOSE_STAGE = true;
		}
	}

	if (InitCurState() == SCENES::COIN_INSERT)
	{
		if (COININSERT != nullptr)
		{
			COININSERT->SetActive(true); // ���� �μ�Ʈ ��� Ȱ��ȭ
			
			if (true == UEngineInput::GetInst().IsDown('F'))
			{
				COIN_NUMBER += 1;
				COINs->SetValue(COIN_NUMBER);
			}

			// ������ �ְ� �������� ���� ȭ������ ��ȯ
			if (COIN_NUMBER > 0 && true == UEngineInput::GetInst().IsDown(VK_SPACE))
			{
				COININSERT->Destroy();
				COININSERT = nullptr;
				return;
			}
		}

		if (COININSERT == nullptr)
		{
			ISPASS_COIN_INSERT = true;
		}
	}

	if (InitCurState() == SCENES::ANI_OP)
	{
		if (COINs != nullptr)
		{
			COINs->SetActive(true); // ���ھ� (����) Ȱ��ȭ
		}

		if (TITLE != nullptr)
		{
			// ������ �ִϸ��̼��� ������ ��
			if (TITLE->MAINRENDERER->IsCurAnimationEnd() == true)
			{
				CHANGEDELAY += _DeltaTime;
				if (CHANGEDELAY >= 3.5f)
				{
					TITLE->Destroy(); // Ÿ��Ʋ�ΰ� ���� ����
					TITLE = nullptr;
					CHANGEDELAY = 0.0f;
					return;
				}
			}

			// �ִϸ��̼� ���� Ű�� ������ �� (�μ�Ʈ ���� ������� ��ȯ)
			if (true == UEngineInput::GetInst().IsDown(VK_SPACE) || true == UEngineInput::GetInst().IsDown('F'))
			{
				TITLE->Destroy(); // Ÿ��Ʋ�ΰ� ���� ����
				TITLE = nullptr;
				return;
			}
		}

		if (TITLE == nullptr)
		{
			ISPASS_ANI_OP = true;
		}
	}

	if (InitCurState() == SCENES::TITLELOGO)
	{
		// Ÿ��Ʋ �ΰ� ������ ������
		if (TITLE->MAINRENDERER->IsCurAnimationEnd() == true)
		{
			CHANGEDELAY += _DeltaTime;
			if (CHANGEDELAY >= 3.0f)
			{
				ISPASS_TITLELOGO = true;
				TITLE->MAINRENDERER->ChangeAnimation("OP_Animation");
				TITLE->BASE00->SetActive(true);
				TITLE->BASE01->SetActive(true);
				LEVEL4->SetActive(true);
				CREDIT->SetActive(true);
				return;
			}
		}

		// ������ �ִϸ��̼����� �Ѿ ��
		if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
		{
			ISPASS_TITLELOGO = true;
			TITLE->MAINRENDERER->ChangeAnimation("OP_Animation");
			TITLE->BASE00->SetActive(true);
			TITLE->BASE01->SetActive(true);
			LEVEL4->SetActive(true);
			CREDIT->SetActive(true);
			return;
		}

		// �ٷ� �μ�Ʈ ���� ������� �Ѿ ��
		if (true == UEngineInput::GetInst().IsDown('F'))
		{
			ISPASS_TITLELOGO = true;
			ISPASS_ANI_OP = true;
			COINs->SetActive(true); // ���ھ� (����) Ȱ��ȭ
			TITLE->Destroy(); // Ÿ��Ʋ�ΰ� ���� ����
			TITLE = nullptr;
			LEVEL4->SetActive(true);
			CREDIT->SetActive(true);
			return;
		}
	}

}

void ATitleGameMode::OpenPlayLevel()
{
	UEngineAPICore::GetCore()->OpenLevel("STAGE01");
}
