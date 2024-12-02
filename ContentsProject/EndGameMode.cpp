#include "PreCompile.h"
#include "EndGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>

#include "Ending.h"
#include "Fade.h"
#include "TitleGameMode.h"

ATitleGameMode* AEndGameMode::TITLELEVEL = nullptr; // ����ƽ ��� �ʱ�ȭ

AEndGameMode::AEndGameMode()
{
};

AEndGameMode::~AEndGameMode()
{

};

void AEndGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ���� ��� �ε�
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}
		Dir.Append("Sounds//02_END");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineSound::Load(FilePath);
		}
	}

	END_Fade = GetWorld()->SpawnActor<AFade>();
	END_Fade->SetFadeSpeed(1.5f);
	END_Fade->FadeOut();

	EndingCut = GetWorld()->SpawnActor<AEnding>();
}

void AEndGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (ON_SOUND_ENDROLL == false)
	{
		SOUND_ENDROLL = UEngineSound::Play("023_StaffRoll(Normal).mp3");
		ON_SOUND_ENDROLL = true;
	}

	if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
	{
		END_Fade->FadeIn();

		// TITLE �����͸� ���� ResetState ȣ��
		if (TITLELEVEL != nullptr)
		{
			TITLELEVEL->ResetState();
		}

		TimeEventer.PushEvent(1.5f, std::bind(&AEndGameMode::MOVETO_TITLE, this), false, false);
	}


}

void AEndGameMode::MOVETO_TITLE()
{
	SOUND_ENDROLL.Stop();
	UEngineAPICore::GetCore()->OpenLevel("TITLE");
}

