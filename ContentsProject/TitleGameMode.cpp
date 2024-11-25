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

	Actor_Fade = GetWorld()->SpawnActor<AFade>();
	Actor_Fade->SetFadeSpeed(1.5f);
	Actor_Fade->SetActive(false);
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
		return;
	}

	if (InitCurState() == SCENES::CHOOSE_STAGE)
	{
		if (Actor_Fade->IsFadeOut == false && Actor_Fade->IsFadeIn == false)
		{
			Actor_Fade->SetActive(true);
			Actor_Fade->FadeOut();
		}

		CHOOSE->SetActive(true);
		TIMEs_StageChoose->SetActive(true);
		TIMEs_StageChoose->SetValue(static_cast<int>(StageChooseTime_NUMBER));

		StageChooseTime_NUMBER -= _DeltaTime;

		if (CHOOSE->GetIsStageONE() == true && UEngineInput::GetInst().IsDown(VK_SPACE))
		{
			TimeEventer.PushEvent(1.5f, std::bind(&AFade::FadeIn, Actor_Fade), false, false);
			TimeEventer.PushEvent(3.0f, std::bind(&ATitleGameMode::OpenPlayLevel, this), false, false);
			//ISPASS_CHOOSE_STAGE = true;
			//CHOOSE->Destroy();
			//CHOOSE = nullptr;
			return;
		}
	}

	if (InitCurState() == SCENES::COIN_INSERT)
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
			ISPASS_COIN_INSERT = true;
			COININSERT->Destroy();
			COININSERT = nullptr;
			return;
		}
	}

	if (InitCurState() == SCENES::ANI_OP)
	{
		COINs->SetActive(true); // ���ھ� (����) Ȱ��ȭ

		// ������ �ִϸ��̼��� ������ ��
		if (TITLE->MAINRENDERER->IsCurAnimationEnd() == true)
		{
			CHANGEDELAY += _DeltaTime;
			if (CHANGEDELAY >= 4.0f)
			{
				ISPASS_ANI_OP = true;
				TITLE->Destroy(); // Ÿ��Ʋ�ΰ� ���� ����
				TITLE = nullptr;
				CHANGEDELAY = 0.0f;
				return;
			}
		}

		// �ִϸ��̼� ���� Ű�� ������ �� (�μ�Ʈ ���� ������� ��ȯ)
		if (true == UEngineInput::GetInst().IsDown(VK_SPACE) || true == UEngineInput::GetInst().IsDown('F'))
		{
			ISPASS_ANI_OP = true;
			TITLE->Destroy(); // Ÿ��Ʋ�ΰ� ���� ����
			TITLE = nullptr;
			return;
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
				TITLE->LEVEL4->SetActive(true);
				TITLE->CREDIT->SetActive(true);
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
			TITLE->LEVEL4->SetActive(true);
			TITLE->CREDIT->SetActive(true);
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
			return;
		}
	}

}

void ATitleGameMode::OpenPlayLevel()
{
	UEngineAPICore::GetCore()->OpenLevel("STAGE01");
}
