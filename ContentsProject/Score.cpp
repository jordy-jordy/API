#include "PreCompile.h"
#include "Score.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineTimer.h>

#include "ContentsEnum.h"

AScore::AScore()
{

	for (size_t i = 0; i < 10; i++)
	{
		// ī�޶� �������� �̳༮�� �������� �ʴ´�.
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false);
		Renders.push_back(Sprite);
	}
}


AScore::~AScore()
{
}

void AScore::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;

	for (size_t i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}

void AScore::BeginPlay()
{
	Super::BeginPlay();


}

void AScore::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AScore::SetOrder(int _Order)
{
	for (size_t i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}

}

void AScore::SetDigitCount(size_t _Digits)
{
	if (_Digits > Renders.size())
	{
		MSGASSERT("������ �ڸ����� ���� ������ �ʰ��մϴ�.");
		return;
	}
	DigitCount = _Digits;
}

void AScore::SetValue(int _Score)
{
	std::string Number = std::to_string(_Score);
	
	// DigitCount�� Number���� ���� ���, �ڵ����� DigitCount�� Number�� �ڸ����� ����
	if (Number.size() > DigitCount)
	{
		DigitCount = Number.size();
	}

	// �ʿ��� �ڸ������� ���� ��� �տ� '0' �߰�
	if (Number.size() < DigitCount)
	{
		Number.insert(Number.begin(), DigitCount - Number.size(), '0');
	}
	else if (Number.size() > DigitCount)
	{
		MSGASSERT("�ڸ����� �Ѱ���ϴ�.");
		return;
	}

	FVector2D Pos = FVector2D::ZERO;
	float TotalWidth = Number.size() * TextScale.X;

	// ���� ��Ŀ� ���� ���� ��ġ ����
	if (CurrentAlignment == Alignment::Right)
	{
		Pos.X -= TotalWidth; // ������ ����
	}

	for (size_t i = 0; i < Number.size(); i++)
	{
		char Value = Number[i] - '0';
		Renders[i]->SetSprite(TextSpriteName, Value);
		Renders[i]->SetComponentScale(TextScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += TextScale.X;
		Renders[i]->SetActive(true);
	}

	for (size_t i = Number.size(); i < Renders.size(); i++)
	{
		Renders[i]->SetActive(false);
	}
}


