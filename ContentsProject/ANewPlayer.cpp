#include "PreCompile.h"
#include "ANewPlayer.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

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

	{
		CollisionComponent = CreateDefaultSubObject<U2DCollision>();
		CollisionComponent->SetComponentLocation({ 200, 0 });
		CollisionComponent->SetComponentScale({ 50, 50 });
		CollisionComponent->SetCollisionGroup(ECollisionGroup::PlayerBody);
	}



	DebugOn();
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

	GetWorld()->SetCameraToMainPawn(false);

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

	FSM.CreateState(NewPlayerState::Attack, std::bind(&ANewPlayer::Attack, this, std::placeholders::_1),
		[this]()
		{
			SpriteRenderer->ChangeAnimation("Run_Right");
		}
	);

	FSM.ChangeState(NewPlayerState::Idle);

}

void ANewPlayer::PlayerCameraCheck()
{
	FVector2D WindowSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	GetWorld()->SetCameraPos(GetActorLocation() - WindowSize.Half());

	DebugOn();
}

void ANewPlayer::PlayerGroundCheck(FVector2D _MovePos)
{
	IsMove = false;
	IsGround = false;

	if (nullptr != ColImage)
	{
		// �ȼ��浹���� ���� �߿��Ѱ� ���ʿ� ������ �ʴ°��̴�.
		FVector2D NextPos = GetActorLocation() + _MovePos;

		UColor Color = ColImage->GetColor(NextPos, UColor::BLACK);
		if (Color == UColor::WHITE)
		{
			IsMove = true;
		}
		else if (Color == UColor::BLACK)
		{
			IsGround = true;
			// ���� ������ ���������� �÷��ִ� ��ɵ� �Բ� ����ų� �ؾ��Ѵ�.
		}
	}
}

void ANewPlayer::Gravity(float _DeltaTime)
{
	if (false == IsGround)
	{
		GravityForce += FVector2D::DOWN * _DeltaTime * 500.0f;
	}
	else {
		GravityForce = FVector2D::ZERO;
	}

	// ��� 
	AddActorLocation(GravityForce * _DeltaTime);
}

void ANewPlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);


	// ���迡�� �׻� 0, 0�� �����Ѵ�.

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineDebug::SwitchIsDebug();
	}

	FSM.Update(_DeltaTime);
}

void ANewPlayer::Idle(float _DeltaTime)
{
	// ������ ���
	PlayerCameraCheck();
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	if (true == UEngineInput::GetInst().IsPress('F'))
	{
		FSM.ChangeState(NewPlayerState::Attack);
		return;
	}

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

void ANewPlayer::Attack(float _DeltaTime)
{
	//if (4 == SpriteRenderer->GetAniamtionFrame())
	//{
		// �����浹, �浹��, �浹��

	//if (true == CollisionComponent->IsCollision(ECollisionGroup::MonsterBody))
	//{
	//	// �浹 �߾�.
	//}

		//U2DCollision* Result = CollisionComponent->CollisionOnce(ECollisionGroup::MonsterBody);

		//if (nullptr != Result)
		//{
		//	// ���� �浹�� �ְ� �ִ�.
		//}
	//}

	// 
}


void ANewPlayer::Move(float _DeltaTime)
{
	PlayerCameraCheck();
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

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
		// AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	}

	if (true == UEngineInput::GetInst().IsPress('F'))
	{
		FSM.ChangeState(NewPlayerState::Attack);
		return;
	}

	Vector.Normalize();

	// �� üũ�ϴ� �Լ�.
	PlayerGroundCheck(Vector * _DeltaTime * Speed);

	if (true == IsMove)
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
	}


	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		FSM.ChangeState(NewPlayerState::Idle);
		return;
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