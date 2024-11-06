#include "PreCompile.h"
#include "ANewPlayer.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>

ANewPlayer::ANewPlayer()
{
	SetActorLocation({ 100, 100 });

	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetSprite("Player_Right.png");
		SpriteRenderer->SetComponentScale({ 300, 300 });
		SpriteRenderer->CreateAnimation("Run_Right", "Player_Right.png", 2, 4, 0.1f);
		SpriteRenderer->CreateAnimation("Idle_Right", "Player_Right.png", 0, 0, 0.1f);
	}
}

ANewPlayer::~ANewPlayer()
{
}

void ANewPlayer::BeginPlay()
{
	Super::BeginPlay();
	// ���� ī�޶� �Ǻ��� ��������� �Ѵ�.
	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	GetWorld()->SetCameraPivot(Size.Half() * -1.0f);
	// ������ ��� ������ ���̴�.
	// �Ｎ�Լ������
	// [] ���� ĸó
	// [=] ���ϸ� ���縦 �մϴ�.
	// [&] ���ϸ� ������ �մϴ�.
	// [this] ������ ��� ���� ����

	//  UFSMStateManager NewFSM = FSM;

	// int Value2;
	// int Value = Value2; // ����� ��
	// Value = 30;

	// ����������

	// = �ܺο��� ������� ���� �⺻������ �����ؼ� ���ο��� ����ض�
	// & �ܺο��� ������� ���� �⺻������ ���������� ���ο��� ����ض�
	// this, Value => ��� �ܺο� �����ϴ� ����� ���� �����ؼ� ���ο��� ����ض�.

	// int Value;

	//FSM.CreateState(NewPlayerState::Idle,
	//	[this](float _DeltaTime)
	//	{
	//		// int Test = this->Test;
	//		// �ܺο� �����ϴ� ���� ������ ���� �����ؼ� ���ο��� 
	//		
	//		// �Ϲ����� ���������� ���⼭ ����Ҽ� ����.
	//		// �ɹ������� this�� ������� ���ȴٴ� ���� ������ �ȵȴ�.

	//		if (true == UEngineInput::GetInst().IsPress('A') ||
	//			true == UEngineInput::GetInst().IsPress('D') ||
	//			true == UEngineInput::GetInst().IsPress('W') ||
	//			true == UEngineInput::GetInst().IsPress('S'))
	//		{
	//			// ���¸� �ٲٴ� ������ �� �ٷ� �����ϴ°� ����.
	//			// ���������� ���¸� �ٲ�ٸ� �����ض�.
	//			this->FSM.ChangeState(NewPlayerState::Move);
	//			return;
	//		}

	//	},
	//	[this]()
	//	{
	//		SpriteRenderer->ChangeAnimation("Idle_Right");
	//	}
	//	);

	FSM.CreateState(NewPlayerState::Idle, std::bind(&ANewPlayer::Idle, this, std::placeholders::_1),
		[this]()
		{
			SpriteRenderer->ChangeAnimation("Idle_Right");
		}
	);

	FSM.CreateState(NewPlayerState::Move, std::bind(&ANewPlayer::Move, this, std::placeholders::_1),
		[this]()
		{
			SpriteRenderer->ChangeAnimation("Run_Right");
		}
	);

	FSM.ChangeState(NewPlayerState::Idle);

}

void ANewPlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	FSM.Update(_DeltaTime);
}

void ANewPlayer::Idle(float _DeltaTime)
{
	if (true == UEngineInput::GetInst().IsPress('A') ||
		true == UEngineInput::GetInst().IsPress('D') ||
		true == UEngineInput::GetInst().IsPress('W') ||
		true == UEngineInput::GetInst().IsPress('S'))
	{
		// ���¸� �ٲٴ� ������ �� �ٷ� �����ϴ°� ����.
		// ���������� ���¸� �ٲ�ٸ� �����ض�.
		FSM.ChangeState(NewPlayerState::Move);
		return;
	}
}


void ANewPlayer::Move(float _DeltaTime)
{
	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Vector += FVector2D::RIGHT;
	}
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Vector += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Vector += FVector2D::DOWN;
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Vector += FVector2D::UP;
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		FSM.ChangeState(NewPlayerState::Idle);
		return;
	}

	if (nullptr != ColImage)
	{

		// �ȼ��浹���� ���� �߿��Ѱ� ���ʿ� ������ �ʴ°��̴�.
		FVector2D NextPos = GetActorLocation() + Vector * _DeltaTime * Speed;

		UColor Color = ColImage->GetColor(NextPos, UColor::BLACK);
		if (Color == UColor::WHITE)
		{
			AddActorLocation(Vector * _DeltaTime * Speed);
		}
	}
}

void ANewPlayer::SetColImage(std::string_view _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void ANewPlayer::LevelChangeStart()
{
	int a = 0;
}