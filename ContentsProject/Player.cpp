#include "PreCompile.h"
#include "Player.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EnginePlatform/EngineInput.h>

#include "Bomb.h"
#include "ATileMap.h"

void APlayer::RunSoundPlay()
{
}

APlayer::APlayer()
{
	{
		SpriteRendererHead = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRendererHead->SetSprite("MainCharater_White.png");
		SpriteRendererHead->SetComponentLocation({ 0, 0 });
		SpriteRendererHead->SetComponentScale({ 64, 64 });
		SpriteRendererHead->SetPivot({0.0f, -28.0f});
		SpriteRendererHead->SetPivot({ 0.0f, -28.0f });


		SpriteRendererHead->CreateAnimation("Idle_Up_Head", "MainCharater_White.png", 17, 17, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Up_Head", "MainCharater_White.png", 18, 22 , 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Down_Head", "MainCharater_White.png", 0, 0, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Down_Head", "MainCharater_White.png", 1, 6, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Left_Head", "MainCharater_White.png", 8, 8, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Left_Head", "MainCharater_White.png", 9, 14, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Right_Head", "MainCharater_White.png", 24, 24, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Right_Head", "MainCharater_White.png", 25, 30, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Anim_Head", "MainCharater_White.png", { 580, 581 }, { 1.5f, 0.2f }, true);

	}

	{
		SpriteRendererBody = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRendererBody->SetSprite("MainCharater_White.png");
		SpriteRendererBody->SetComponentLocation({ 0, static_cast<int>(64 * 0.5f) });
		SpriteRendererBody->SetComponentScale({ 64, 64 });
		SpriteRendererBody->SetPivot({ 0.0f, -28.0f });

		SpriteRendererBody->CreateAnimation("Idle_Up_Body", "MainCharater_White.png", 48, 48, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Up_Body", "MainCharater_White.png", 50, 54 , 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Down_Body", "MainCharater_White.png", 32, 32, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Down_Body", "MainCharater_White.png", 33, 38, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Left_Body", "MainCharater_White.png", 40, 40, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Left_Body", "MainCharater_White.png", 41, 46, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Right_Body", "MainCharater_White.png", 56, 56, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Right_Body", "MainCharater_White.png", 57, 62, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Anim_Body", "MainCharater_White.png", { 612, 613 }, { 1.5f, 0.2f }, true);
	}

	SpriteRendererBody->ChangeAnimation("Idle_Down_Body");
	SpriteRendererHead->ChangeAnimation("Idle_Down_Head");

	SpriteRendererBody->SetOrder(ERenderOrder::PLAYER);
	SpriteRendererHead->SetOrder(ERenderOrder::PLAYER);
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	//FVector2D LocalLocation = GetActorLocation() - WallTileMap->GetActorLocation();
	//UEngineDebug::CoreOutPutString("GetActorLocation() : " + GetActorLocation().ToString());
	//UEngineDebug::CoreOutPutString("LocalLocation : " + LocalLocation.ToString());

	switch (CurPlayerState)
	{
	case PlayerState::Idle:
		Idle(_DeltaTime);
		break;
	case PlayerState::Move:
		Move(_DeltaTime);
		break;
	case PlayerState::Idle_Anim:
		Idle_Anim(_DeltaTime);
		break;
	default:
		break;
	}

	if (true == UEngineInput::GetInst().IsDown('F'))
	{
		IdleAnimTimer = 0.0f;

		FVector2D LocalLocation = GetActorLocation() - WallTileMap->GetActorLocation();
		FIntPoint CurTileIndex = WallTileMap->LocationToIndex(LocalLocation);

		if (false == WallTileMap->IsBomb(CurTileIndex))
		{
			PlaceBomb();
		}
	}

	SpriteRendererHead->SetOrder(GetActorLocation().Y - WallTileMap->GetActorLocation().Y);
	SpriteRendererBody->SetOrder(GetActorLocation().Y - WallTileMap->GetActorLocation().Y);

	if (nullptr == WallTileMap)
	{
		MSGASSERT("Ÿ�� ���� ���õ��� �ʾҽ��ϴ�.")
	}
}


FVector2D APlayer::PosToTileIndex(FVector2D _Pos)
{
	FVector2D Location = _Pos;
	FIntPoint Index = WallTileMap->LocationToIndex(Location);
	FVector2D InvertResult = {Index.X, Index.Y};
	return InvertResult;
}

FVector2D APlayer::InvertLOC(FVector2D _Dir)
{
	if (_Dir == FVector2D::ZERO)
	{
		return CUSTOM_VECTOR_ZERO;
	}

	if (_Dir == FVector2D::LEFT)
	{
		return CUSTOM_VECTOR_LEFT;
	}

	if (_Dir == FVector2D::RIGHT)
	{
		return CUSTOM_VECTOR_RIGHT;
	}

	if (_Dir == FVector2D::UP)
	{
		return CUSTOM_VECTOR_UP;
	}

	if (_Dir == FVector2D::DOWN)
	{
		return CUSTOM_VECTOR_DOWN;
	}

}


// ��ź ��ġ
void APlayer::PlaceBomb()
{
	FVector2D TileSize = WallTileMap->GetTileSize(); // 32
	FVector2D TileHalfSize = WallTileMap->GetTileHalfSize(); // 16

	FVector2D PlayerLocation = GetActorLocation(); // 112, 80 (+ 16�� ������ ����) // �������� LEFT TOP�� �������� å��
	FIntPoint PLAYERINDEX = WallTileMap->LocationToIndex(PlayerLocation);
	FVector2D TileMapLocation = WallTileMap->GetActorLocation(); // 96, 64 // �������� LEFT TOP�� �������� å��
	FVector2D LocalLocation = PlayerLocation - TileMapLocation; // Ÿ�ϸ��� LEFT TOP�� �������� å���ǵ��� ���

	FIntPoint BOMB_SET_Index = WallTileMap->LocationToIndex(LocalLocation); // Ÿ�ϸ� ���� �ε���
	FVector2D BOMB_SET_LOCATION = WallTileMap->IndexToTileLocation(PLAYERINDEX) + FVector2D(TileHalfSize.X, TileHalfSize.Y);

	// ��ź ��ġ ���� ���� ��Ȯ��
	if (WallTileMap->IsBomb(BOMB_SET_Index))
	{
		return; // �̹� ��ź�� ��ġ�� ���
	}

	ABomb* Bomb = GetWorld()->SpawnActor<ABomb>();

	// Ÿ�ϸʿ� ��ź ���� (����)
	WallTileMap->SetBomb(BOMB_SET_Index, Bomb);
	// ��ġ�� ��ź ��ġ
	Bomb->SetWallTileMap(WallTileMap, BOMB_SET_Index); // Ÿ�ϸ� ���� ����
	Bomb->SetActorLocation(BOMB_SET_LOCATION);

	Bomb->SetPower(BOMBPOWER);

	ChangeState(PlayerState::Idle);

}

void APlayer::ChangeState(PlayerState _CurPlayerState)
{
	switch (_CurPlayerState)
	{
	case PlayerState::Idle:
		IdleStart();
		break;
	case PlayerState::Move:
		MoveStart();
		break;
	default:
		break;
	}
	CurPlayerState = _CurPlayerState;
}

void APlayer::IdleStart()
{
}

void APlayer::MoveStart()
{
}

void APlayer::Idle_Anim(float _DeltaTime)
{
	SpriteRendererBody->ChangeAnimation("Idle_Anim_Body");
	SpriteRendererHead->ChangeAnimation("Idle_Anim_Head");
	
	if (true == UEngineInput::GetInst().IsPress('A') ||
		true == UEngineInput::GetInst().IsPress('D') ||
		true == UEngineInput::GetInst().IsPress('W') ||
		true == UEngineInput::GetInst().IsPress('S'))
	{
		IdleAnimTimer = 0.0f;
		ChangeState(PlayerState::Move);
		return;
	}
}


void APlayer::Idle(float _DeltaTime)
{
	SpriteRendererBody->ChangeAnimation("Idle_Down_Body");
	SpriteRendererHead->ChangeAnimation("Idle_Down_Head");

	IdleAnimTimer += _DeltaTime;
	if (IdleAnimTimer >= IdleAnimTime)
	{
		ChangeState(PlayerState::Idle_Anim);
		return;
	}

	if (true == UEngineInput::GetInst().IsPress('A') ||
		true == UEngineInput::GetInst().IsPress('D') ||
		true == UEngineInput::GetInst().IsPress('W') ||
		true == UEngineInput::GetInst().IsPress('S'))
	{
		IdleAnimTimer = 0.0f;
		ChangeState(PlayerState::Move);
		return;
	}
}

void APlayer::Move(float _DeltaTime)
{
	FVector2D Vector = FVector2D::ZERO;
	int DIR = 0;

	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Vector = FVector2D::RIGHT;
		SpriteRendererBody->ChangeAnimation("Run_Right_Body");
		SpriteRendererHead->ChangeAnimation("Run_Right_Head");
		DIR = 1;
	}
	else if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Vector = FVector2D::LEFT;
		SpriteRendererBody->ChangeAnimation("Run_Left_Body");
		SpriteRendererHead->ChangeAnimation("Run_Left_Head");
		DIR = 2;
	}
	else if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Vector = FVector2D::DOWN;
		SpriteRendererBody->ChangeAnimation("Run_Down_Body");
		SpriteRendererHead->ChangeAnimation("Run_Down_Head");
		DIR = 3;
	}
	else if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Vector = FVector2D::UP;
		SpriteRendererBody->ChangeAnimation("Run_Up_Body");
		SpriteRendererHead->ChangeAnimation("Run_Up_Head");
		DIR = 4;
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		ChangeState(PlayerState::Idle);
		Vector = FVector2D::ZERO;
		return;
	}

	FVector2D PlusPos = Vector;
	switch (DIR)
	{
	case 1: PlusPos *= 16.0f; break; // RIGHT
	case 2: PlusPos *= 16.0f; break; // LEFT
	case 3: PlusPos *= 16.0f; break; // DOWN
	case 4: PlusPos *= 16.0f; break; // UP
	default: break;
	}

	// Ÿ�ϸ� ũ��
	const int POS_X_MIN = 96;
	const int POS_X_MAX = 512;
	const int POS_Y_MIN = 64;
	const int POS_Y_MAX = 416;
	
	FVector2D PLAYER_POS_NEXT = GetActorLocation() + (Vector * _DeltaTime * Speed) + PlusPos; // ������ LEFT TOP ����
	//UEngineDebug::CoreOutPutString("PLAYER_POS_NEXT : " + PLAYER_POS_NEXT.ToString());

	FVector2D PLAYER_LOCAL_LOC = GetActorLocation() - WallTileMap->GetActorLocation(); // Ÿ�ϸ� LEFT TOP ����
	FVector2D PLAYER_LOCAL_LOC_NEXT = PLAYER_LOCAL_LOC + (Vector * _DeltaTime * Speed) + PlusPos;

	FIntPoint PLAYER_LOCAL_IDX_NEXT = WallTileMap->LocationToIndex(PLAYER_LOCAL_LOC_NEXT);

	Tile* TileData = WallTileMap->GetTileRef(PLAYER_LOCAL_LOC_NEXT);
	bool BombCheck = WallTileMap->IsBomb(PLAYER_LOCAL_IDX_NEXT);

	if (PLAYER_POS_NEXT.X < POS_X_MIN || PLAYER_POS_NEXT.X > POS_X_MAX || PLAYER_POS_NEXT.Y < POS_Y_MIN || PLAYER_POS_NEXT.Y > POS_Y_MAX)
	{
		return;
	}

	if (TileData != nullptr)
	{
		if (TileData->SpriteIndex != 2 &&
			TileData->SpriteIndex != 1 /*&&
			true != BombCheck*/)
		{
			AddActorLocation(Vector * _DeltaTime * Speed);
		}
	}
}

void APlayer::LevelChangeStart()
{
	Super::LevelChangeStart();
}

void APlayer::LevelChangeEnd()
{
	Super::LevelChangeEnd();
}

