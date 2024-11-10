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
	// UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	{
		SpriteRendererHead = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRendererHead->SetSprite("MainCharater_White.png");
		SpriteRendererHead->SetComponentLocation({ 0, 0 });
		SpriteRendererHead->SetComponentScale({ 64, 64 });
		SpriteRendererHead->SetPivotType(PivotType::Bot);

		SpriteRendererHead->CreateAnimation("Idle_Up_Head", "MainCharater_White.png", 17, 17, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Up_Head", "MainCharater_White.png", 18, 22, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Down_Head", "MainCharater_White.png", 0, 0, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Down_Head", "MainCharater_White.png", 1, 6, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Left_Head", "MainCharater_White.png", 9, 9, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Left_Head", "MainCharater_White.png", 10, 14, 0.1f);

		SpriteRendererHead->CreateAnimation("Idle_Right_Head", "MainCharater_White.png", 24, 24, 0.1f);
		SpriteRendererHead->CreateAnimation("Run_Right_Head", "MainCharater_White.png", 25, 30, 0.1f);

		//std::string Name = SpriteRenderer->GetCurSpriteName();
	}

	{
		SpriteRendererBody = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRendererBody->SetSprite("MainCharater_White.png");
		SpriteRendererBody->SetComponentLocation({ 0, static_cast<int>(64 * 0.5f) });
		SpriteRendererBody->SetComponentScale({ 64, 64 });
		SpriteRendererBody->SetPivotType(PivotType::Bot);

		SpriteRendererBody->CreateAnimation("Idle_Up_Body", "MainCharater_White.png", 48, 48, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Up_Body", "MainCharater_White.png", 49, 54, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Down_Body", "MainCharater_White.png", 32, 32, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Down_Body", "MainCharater_White.png", 33, 38, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Left_Body", "MainCharater_White.png", 40, 40, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Left_Body", "MainCharater_White.png", 41, 46, 0.1f);

		SpriteRendererBody->CreateAnimation("Idle_Right_Body", "MainCharater_White.png", 56, 56, 0.1f);
		SpriteRendererBody->CreateAnimation("Run_Right_Body", "MainCharater_White.png", 57, 62, 0.1f);
	}

	SpriteRendererHead->ChangeAnimation("Idle_Down_Head");
	SpriteRendererBody->ChangeAnimation("Idle_Down_Body");

	SpriteRendererHead->SetOrder(ERenderOrder::PLAYER);
	SpriteRendererBody->SetOrder(ERenderOrder::PLAYER);

}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(PlayerState::Idle);
}

FVector2D APlayer::PosToTileIndex(FVector2D _Pos)
{
	FVector2D Location = _Pos;
	FIntPoint Index = WallTileMap->LocationToIndex(Location);
	FVector2D InvertResult = {Index.X, Index.Y};
	return InvertResult;
}

FVector2D APlayer::IndexToTilePos(FVector2D _Index)
{
	FIntPoint Index = { static_cast<int>(_Index.X), static_cast<int>(_Index.Y) };
	FVector2D Location = WallTileMap->IndexToTileLocation(Index);
	return Location;
}

// ��ź ��ġ
void APlayer::PlaceBomb(float _DeltaTime)
{
	FVector2D Location = GetActorLocation();
	FVector2D index = PosToTileIndex(Location);
	FVector2D Pos = IndexToTilePos(index);
	FVector2D HalfTiles = WallTileMap->GetTileHalfSize();

	FIntPoint TileIndex =  WallTileMap->LocationToIndex(Location);

	// ��ź ��ġ ���� ���� ��Ȯ��
	if (WallTileMap->IsBomb(TileIndex))
	{
		return; // �̹� ��ź�� ��ġ�� ���
	}

	ABomb* Bomb = GetWorld()->SpawnActor<ABomb>();

	// Ÿ�ϸʿ� ��ź ���� (����)
	WallTileMap->SetBomb(TileIndex, Bomb);
	// ��ġ�� ��ź ��ġ
	Bomb->SetActorLocation(Pos + HalfTiles);

	Bomb->SetWallTileMap(WallTileMap, TileIndex); // Ÿ�ϸ� ���� ����
	Bomb->StartExplodeTimer(); // ���� Ÿ�̸� ����

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
	SpriteRendererHead->ChangeAnimation("Idle_Down_Head");
	SpriteRendererBody->ChangeAnimation("Idle_Down_Body");
}

void APlayer::MoveStart()
{
}

void APlayer::Idle(float _DeltaTime)
{
	SpriteRendererHead->ChangeAnimation("Idle_Down_Head");
	SpriteRendererBody->ChangeAnimation("Idle_Down_Body");

	if (true == UEngineInput::GetInst().IsPress('A') ||
		true == UEngineInput::GetInst().IsPress('D') ||
		true == UEngineInput::GetInst().IsPress('W') ||
		true == UEngineInput::GetInst().IsPress('S'))
	{
		ChangeState(PlayerState::Move);
		return;
	}

}

void APlayer::Move(float _DeltaTime)
{
	FVector2D Vector = FVector2D::ZERO;
	int Temp = 0;

	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Vector = FVector2D::RIGHT;
		SpriteRendererHead->ChangeAnimation("Run_Right_Head");
		SpriteRendererBody->ChangeAnimation("Run_Right_Body");
		Temp = 1;
	}
	else if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Vector = FVector2D::LEFT;
		SpriteRendererHead->ChangeAnimation("Run_Left_Head");
		SpriteRendererBody->ChangeAnimation("Run_Left_Body");
		Temp = 2;
	}
	else if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Vector = FVector2D::DOWN;
		SpriteRendererHead->ChangeAnimation("Run_Down_Head");
		SpriteRendererBody->ChangeAnimation("Run_Down_Body");
		Temp = 3;
	}
	else if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Vector = FVector2D::UP;
		SpriteRendererHead->ChangeAnimation("Run_Up_Head");
		SpriteRendererBody->ChangeAnimation("Run_Up_Body");
		Temp = 4;
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
	switch (Temp)
	{
	case 1: PlusPos *= 16.0f; break;
	case 2: PlusPos *= 16.0f; break;
	case 3: PlusPos *= 5.0f; break;
	case 4: PlusPos *= 19.0f; break;
	default: break;
	}

	FVector2D TileSize = WallTileMap->GetTileSize(); // 32
	FVector2D LocalLocation = GetActorLocation() - WallTileMap->GetActorLocation(); // Ÿ�ϸ� �������� ����
	FVector2D NextLocalLocation = LocalLocation + PlusPos + (Vector * _DeltaTime* Speed); // �÷��̾� �Ǻ��� �������� ũ��

	FVector2D CurTileIndex = PosToTileIndex(LocalLocation); 

	FVector2D NextTileIndex = PosToTileIndex(NextLocalLocation);

	// Ÿ�� ����� ������ Ÿ�� �ε��� ����
	FVector2D LocationAtIndex = LocalLocation / TileSize; // �÷��̾� ��ġ�� Ÿ�ϸ� �ε����� ���� ����
	FVector2D NextLocationAtIndex = NextLocalLocation / TileSize; // �÷��̾ �̵��ϴ� ������ Ÿ�ϸ� �ε���
	
	//UEngineDebug::CoreOutPutString("NextLocalLocation : " + NextLocalLocation.ToString());
	//UEngineDebug::CoreOutPutString("CurTileIndex : " + CurTileIndex.ToString());
	//UEngineDebug::CoreOutPutString("NextTileIndex : " + NextTileIndex.ToString());
	//UEngineDebug::CoreOutPutString("LocationAtIndex : " + LocationAtIndex.ToString());
	//UEngineDebug::CoreOutPutString("NextLocationAtIndex : " + NextLocationAtIndex.ToString());

	Tile* TileData = WallTileMap->GetTileRef(NextLocalLocation);

	if (NextLocationAtIndex.X < 0 || NextLocationAtIndex.Y < 0 || NextLocationAtIndex.X > 13 || NextLocationAtIndex.Y > 11)
	{
		return;
	}
	if (TileData->SpriteIndex != 2 && TileData->SpriteIndex != 1)
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
	}
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
	default:
		break;
	}
	
	if (true == UEngineInput::GetInst().IsDown('F'))
	{
		FVector2D LocalLocation = GetActorLocation() - WallTileMap->GetActorLocation();
		FIntPoint CurTileIndex = WallTileMap->LocationToIndex(LocalLocation);

		if (false == WallTileMap->IsBomb(CurTileIndex))
		{
			PlaceBomb(_DeltaTime);
		}
	}

	SpriteRendererHead->SetOrder(GetActorLocation().Y - WallTileMap->GetActorLocation().Y);
	SpriteRendererBody->SetOrder(GetActorLocation().Y - WallTileMap->GetActorLocation().Y);

	if (nullptr == WallTileMap)
	{
		MSGASSERT("Ÿ�� ���� ���õ��� �ʾҽ��ϴ�.")
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

