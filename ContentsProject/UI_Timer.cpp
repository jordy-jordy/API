#include "PreCompile.h"
#include "UI_Timer.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineTimer.h>

#include "ContentsEnum.h"


AUI_Timer::AUI_Timer()
{
	TOPBAR = CreateDefaultSubObject<USpriteRenderer>();
	TOPBAR->SetSprite("ScoreBar_Adventure.png");
	TOPBAR->SetComponentScale({ 608, 44 });
	TOPBAR->SetComponentLocation({ 304, 0 });
	TOPBAR->SetPivotType(PivotType::Top);
	TOPBAR->SetOrder(ERenderOrder::UI);

	TIME_MINUTE[1] = CreateDefaultSubObject<USpriteRenderer>();
	TIME_MINUTE[1]->SetSprite("TimeCount.png", 3);
	TIME_MINUTE[1]->SetComponentScale({12, 12});
	TIME_MINUTE[1]->SetComponentLocation({ 280, 18 });
	TIME_MINUTE[1]->SetPivotType(PivotType::Top);
	TIME_MINUTE[1]->SetOrder(50000);

	TIME_SECOND[0] = CreateDefaultSubObject<USpriteRenderer>();
	TIME_SECOND[0]->SetSprite("TimeCount.png", 1);
	TIME_SECOND[0]->SetComponentScale({ 12, 12 });
	TIME_SECOND[0]->SetComponentLocation({ 312, 18 });
	TIME_SECOND[0]->SetPivotType(PivotType::Top);
	TIME_SECOND[0]->SetOrder(50000);

	TIME_SECOND[9] = CreateDefaultSubObject<USpriteRenderer>();
	TIME_SECOND[9]->SetSprite("TimeCount.png", 1);
	TIME_SECOND[9]->SetComponentScale({ 12, 12 });
	TIME_SECOND[9]->SetComponentLocation({ 328, 18 });
	TIME_SECOND[9]->SetPivotType(PivotType::Top);
	TIME_SECOND[9]->SetOrder(50000);

};

AUI_Timer::~AUI_Timer()
{

};

void AUI_Timer::BeginPlay()
{
	Super::BeginPlay();

	StartTimer(120.0f);
}

void AUI_Timer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (bTimerRunning == true)
	{
		Timer.TimeCheck(); // DeltaTime ����
		RemainingTime -= Timer.GetDeltaTime(); // ���� �ð� ����

		if (RemainingTime <= 0.0)
		{
			RemainingTime = 0.0;
			bTimerRunning = false; // Ÿ�̸� ����
			// �߰� ���� (��: �̺�Ʈ ȣ��)
		}

		UpdateUITimer(); // UI ������Ʈ
	}
}

void AUI_Timer::StartTimer(float StartTimeInSeconds)
{
	Timer.TimeStart(); // Ÿ�̸� �ʱ�ȭ
	RemainingTime = StartTimeInSeconds;
	bTimerRunning = true;
}

void AUI_Timer::UpdateUITimer()
{
	int Minutes = static_cast<int>(RemainingTime) / 60;
	int Seconds = static_cast<int>(RemainingTime) % 60;

	// TIME_MINUTE[0], TIME_MINUTE[1], TIME_SECOND[0], TIME_SECOND[1]�� ������Ʈ
	// ����: �а� �ʸ� Sprite�� ǥ��
	//TIME_MINUTE[0]->SetSprite(std::to_string(Minutes / 10) + ".png");
	//TIME_MINUTE[1]->SetSprite(std::to_string(Minutes % 10) + ".png");
	//TIME_SECOND[0]->SetSprite(std::to_string(Seconds / 10) + ".png");
	//TIME_SECOND[1]->SetSprite(std::to_string(Seconds % 10) + ".png");
}