#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>

#include "PlayGameMode.h"


enum class SCENES
{
	TITLELOGO = 0,
	ANI_OP = 1,
	COIN_INSERT = 2,
	CHOOSE_STAGE = 3,
	ANI_TRANSIT = 4,
	NONE = 5
};

// ���� :
class ATitleGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATitleGameMode();
	~ATitleGameMode();

	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

	int GetCOIN_NUMBER()
	{
		return COIN_NUMBER;
	}

	SCENES InitCurState()
	{
		if (ISPASS_TITLELOGO    == false &&
			ISPASS_ANI_OP       == false &&
			ISPASS_COIN_INSERT  == false &&
			ISPASS_CHOOSE_STAGE == false &&
			ISPASS_ANI_TRANSIT  == false)
		{
			return SCENES::TITLELOGO;
		}
		if (ISPASS_TITLELOGO    == true &&
			ISPASS_ANI_OP       == false &&
			ISPASS_COIN_INSERT  == false &&
			ISPASS_CHOOSE_STAGE == false &&
			ISPASS_ANI_TRANSIT  == false)
		{
			return SCENES::ANI_OP;
		}
		if (ISPASS_TITLELOGO    == true &&
			ISPASS_ANI_OP       == true &&
			ISPASS_COIN_INSERT  == false &&
			ISPASS_CHOOSE_STAGE == false &&
			ISPASS_ANI_TRANSIT  == false)
		{
			return SCENES::COIN_INSERT;
		}
		if (ISPASS_TITLELOGO    == true &&
			ISPASS_ANI_OP       == true &&
			ISPASS_COIN_INSERT  == true &&
			ISPASS_CHOOSE_STAGE == false &&
			ISPASS_ANI_TRANSIT  == false)
		{
			return SCENES::CHOOSE_STAGE;
		}
		if (ISPASS_TITLELOGO    == true &&
			ISPASS_ANI_OP       == true &&
			ISPASS_COIN_INSERT  == true &&
			ISPASS_CHOOSE_STAGE == true &&
			ISPASS_ANI_TRANSIT  == false)
		{
			return SCENES::ANI_TRANSIT;
		}

		return SCENES::NONE;
	}

	void OpenPlayLevel();

	void StartTIme()
	{
		IsStopTime = false;
	}

	void StopTIme()
	{
		IsStopTime = true;
	}

	void CHOOSETO_TRANSITANI();
	void TRANSITANITO_PLAY();

	class UTitleLogo* TITLE = nullptr;


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* LEVEL4 = nullptr;
	class USpriteRenderer* CREDIT = nullptr;

	class UCoinInsert* COININSERT = nullptr;
	class UChooseStage* CHOOSE = nullptr;
	class ATransit_Ani* TRANSIT = nullptr;

	class AScore* COINs = nullptr;
	class AScore* TIMEs_StageChoose = nullptr;
	class AFade* Actor_Fade = nullptr;

	int COIN_NUMBER = 0;
	float StageChooseTime_NUMBER = 21.0f;
	float TIME_DELAY = 0.0f;

	int DIGITCOUNT_Coins = 2;
	int DIGITCOUNT_StageChooseTime = 2;

	bool ISPASS_TITLELOGO = false;
	bool ISPASS_ANI_OP = false;
	bool ISPASS_COIN_INSERT = false;
	bool ISPASS_CHOOSE_STAGE = false;
	bool ISPASS_ANI_TRANSIT = false;

	bool ISFIRSTFADE_TITLELOGO = false;
	bool ISFIRSTFADE_ANI_OP = false;
	bool ISFIRSTFADE_COIN_INSERT = false;
	bool ISFIRSTFADE_CHOOSE_STAGE = false;
	bool ISFIRSTFADE_ANI_TRANSIT = false;

	bool ISFADING_ANI_TRANSIT = false;

	float CHANGEDELAY = 0.0f;

	bool IsStopTime = false;

	bool IsChooseStage = false;

	float SoundVolume = 1.0f;

	class USoundPlayer SOUND_NGLOGO;
	bool ON_SOUND_NGLOGO = false;

	class USoundPlayer SOUND_OPANIM;
	bool ON_SOUND_OPANIM = false;

	class USoundPlayer SOUND_BBMLOGO;
	bool ON_SOUND_BBMLOGO = false;

	class USoundPlayer SOUND_COININSERT;
	bool ON_SOUND_COININSERT = false;

	class USoundPlayer SOUND_CHOOSESTAGE;
	bool ON_SOUND_CHOOSESTAGE = false;

	class USoundPlayer SOUND_TRANSITANIM;
	bool ON_SOUND_TRANSITANIM = false;

	float Transit_Time = 0.0f;

};

