#include "PreCompile.h"
#include "Bomb.h"

#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

#include "ATileMap.h"
#include "Tile_Destroy.h"
#include "Mushroom.h"


ABomb::ABomb()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//SpriteRenderer->SetSprite("01_Bomb_00_Idle");
	SpriteRenderer->SetComponentScale({ 32, 32 });

	SpriteRenderer->CreateAnimation("Bomb_Idle", "01_Bomb_00_Idle", 0, 3, IdleFrameSpeed, true);
	SpriteRenderer->ChangeAnimation("Bomb_Idle");

	SpriteRenderer->CreateAnimation("Bomb_Gone", "01_Bomb_00_Idle", 4, 4, IdleFrameSpeed, false);

	SpriteRenderer->SetOrder(ERenderOrder::BOMB);
	TimeEventer.PushEvent(Bomb_Explode_Time, std::bind(&ABomb::Bomb_ExPlode, this), false, false);
}

ABomb::~ABomb()
{
	ClearBombTile(); // �Ҹ��ڿ��� Ÿ�ϸ� ������Ʈ
}

void ABomb::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Bomb_Destroy();
}

void ABomb::SetWallTileMap(ATileMap* _TileMap, FIntPoint _Index)
{
	WallTileMap = _TileMap;
	BombTileIndex = _Index;
}

void ABomb::Bomb_Destroy()
{
	if (nullptr != Explode_Center)
	{
		if (true == Explode_Center->IsCurAnimationEnd())
		{
			// ��ź ����
			Destroy();
		}
	}
}

void ABomb::ClearBombTile()
{
	if (WallTileMap != nullptr)
	{
		WallTileMap->SetBomb(BombTileIndex, nullptr); // ��ź �����͸� nullptr�� ����
	}
}


void ABomb::Bomb_ExPlode()
{
	int _Power = CurBombPower;

	SpriteRenderer->ChangeAnimation("Bomb_Gone");
	Explode_Center = CreateDefaultSubObject<USpriteRenderer>();
	//Explode_Center->SetSprite("ExplodeCenter.png");
	Explode_Center->CreateAnimation("Bomb_Center", "ExplodeCenter.png", 0, 19, ExplodeFrameSpeed, false);
	Explode_Center->ChangeAnimation("Bomb_Center");
	Explode_Center->SetComponentScale({ 32, 32 });
	Explode_Center->SetComponentLocation({ 0, 0 });

	FVector2D TileSize = WallTileMap->GetTileHalfSize();

	FVector2D Pos = GetActorLocation() - WallTileMap->GetActorLocation(); // X, Y�� TILE HALF SIZE (16)�� ���� ��
	// �̴�� ����ϸ� float�� int�� ��ȯ�Ǵ� ������ �� �ְ��� �Ͼ

	FVector2D Pos_Minus_TileHalfSize = Pos - WallTileMap->GetTileHalfSize(); // X, Y�� TILE HALF SIZE (16)�� �� ��
	// ��Ȯ�� ���� ��� ���� ���



	// ��� ������ ��ġ�� ���Ϳ� �ִ´�?
	std::vector <FVector2D> AllMonstersPOS; // ��ġ�� ���� ����
	
	std::list <AMonster*> AllMonsters; // ���� �����Ǿ� �ִ� ��� ���͸� ������
	AllMonsters = GetWorld()->GetActorsFromClass<AMonster>();

	// iterator�� ��ȸ ����
	std::list<AMonster*>::iterator StartIter = AllMonsters.begin();
	std::list<AMonster*>::iterator EndIter = AllMonsters.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		AMonster* CurMonster = *StartIter;

		if (nullptr == CurMonster)
		{
			continue;
		}

		// ���� ������ ��ġ�� AllMonstersPOS �� ����
		AllMonstersPOS.push_back(CurMonster->GetActorLocation());
	}
		int a = 0;





	// ���� Ȯ�� ó��
	int LeftCount = 0;
	for (int i = 1; i <= _Power - 1; i++)
	{
		Tile* TileDataLeft = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ -32, 0 } *static_cast<float>(i));

		if (TileDataLeft == nullptr || TileDataLeft->SpriteIndex == 2)
		{
			break; // ���� Ȯ�� �ߴ�
		}

		USpriteRenderer* Explode_LeftMid = CreateDefaultSubObject<USpriteRenderer>();
		//Explode_LeftMid->SetSprite("ExplodeLeftMid.png");
		Explode_LeftMid->CreateAnimation("Bomb_LeftMid", "ExplodeLeftMid.png", 0, 19, ExplodeFrameSpeed, false);
		Explode_LeftMid->ChangeAnimation("Bomb_LeftMid");
		Explode_LeftMid->SetComponentScale({ 32, 32 });
		Explode_LeftMid->SetOrder((Pos + FVector2D{ -32, 0 } *static_cast<float>(i)).Y);
		Explode_LeftMid->SetComponentLocation(FVector2D{ -32, 0 } *static_cast<float>(i));

		if (TileDataLeft->SpriteIndex == 1)
		{
			FVector2D TilePos_Location = Explode_LeftMid->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;
			WallTileMap->RemoveTile(TargetPos);

			FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
			// ���� ���͸� ���� ���� �ִϸ��̼� ���
			ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
			WallTileMap->SetDestroyObject(TargetIndex, Object);
			// ��ġ�� ���� �ִϸ��̼� ��ġ
			Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
			Object->SetActorLocation(GetActorLocation()+ TilePos_Location);
			break;
		}

		LeftCount = i;
	}
	Tile* TileDataLeft = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ -32, 0 } *(static_cast<float>(LeftCount + 1)));
	if (TileDataLeft != nullptr)
	{
		if (TileDataLeft->SpriteIndex != 2 && LeftCount + 1 > 0)
		{
			USpriteRenderer* Explode_Left = CreateDefaultSubObject<USpriteRenderer>();
			//Explode_Left->SetSprite("ExplodeLeft.png");
			Explode_Left->CreateAnimation("Bomb_Left", "ExplodeLeft.png", 0, 19, ExplodeFrameSpeed, false);
			Explode_Left->ChangeAnimation("Bomb_Left");
			Explode_Left->SetComponentScale({ 32, 32 });
			Explode_Left->SetOrder((Pos + FVector2D{ -32, 0 }).Y);
			Explode_Left->SetComponentLocation(FVector2D{ -32, 0 } *(static_cast<float>(LeftCount + 1)));
			
			FVector2D TilePos_Location = Explode_Left->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;

			if (TileDataLeft->SpriteIndex == 1)
			{
				FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
				// ���� ���͸� ���� ���� �ִϸ��̼� ���
				ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
				WallTileMap->SetDestroyObject(TargetIndex, Object);
				// ��ġ�� ���� �ִϸ��̼� ��ġ
				Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
				Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			}

			WallTileMap->RemoveTile(TargetPos);
		}
	}

	// �� Ȯ�� ó��
	int UpCount = 0;
	for (int i = 1; i <= _Power - 1; i++)
	{
		Tile* TileDataUp = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 0, -32 } *static_cast<float>(i));

		if (TileDataUp == nullptr || TileDataUp->SpriteIndex == 2)
		{
			break; // �� Ȯ�� �ߴ�
		}

		USpriteRenderer* Explode_UpMid = CreateDefaultSubObject<USpriteRenderer>();
		//Explode_UpMid->SetSprite("ExplodeUpMid.png");
		Explode_UpMid->CreateAnimation("Bomb_UpMid", "ExplodeUpMid.png", 0, 19, ExplodeFrameSpeed, false);
		Explode_UpMid->ChangeAnimation("Bomb_UpMid");
		Explode_UpMid->SetComponentScale({ 32, 32 });
		Explode_UpMid->SetOrder((Pos + FVector2D{ 0, -32 } *static_cast<float>(i)).Y);
		Explode_UpMid->SetComponentLocation(FVector2D{ 0, -32 } *static_cast<float>(i));

		if (TileDataUp->SpriteIndex == 1)
		{
			FVector2D TilePos_Location = Explode_UpMid->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;
			WallTileMap->RemoveTile(TargetPos);

			FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
			// ���� ���͸� ���� ���� �ִϸ��̼� ���
			ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
			WallTileMap->SetDestroyObject(TargetIndex, Object);
			// ��ġ�� ���� �ִϸ��̼� ��ġ
			Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
			Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			break;
		}
		UpCount = i;
	}
	Tile* TileDataUp = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 0, -32 } *(static_cast<float>(UpCount + 1)));
	if (TileDataUp != nullptr)
	{
		if (TileDataUp->SpriteIndex != 2 && UpCount + 1 > 0)
		{
			USpriteRenderer* Explode_Up = CreateDefaultSubObject<USpriteRenderer>();
			//Explode_Up->SetSprite("ExplodeUp.png");
			Explode_Up->CreateAnimation("Bomb_Up", "ExplodeUp.png", 0, 19, ExplodeFrameSpeed, false);
			Explode_Up->ChangeAnimation("Bomb_Up");
			Explode_Up->SetComponentScale({ 32, 32 });
			Explode_Up->SetOrder((Pos + FVector2D{ 0, -32 }).Y);
			Explode_Up->SetComponentLocation(FVector2D{ 0, -32 } *(static_cast<float>(UpCount + 1)));

			FVector2D TilePos_Location = Explode_Up->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;

			if (TileDataUp->SpriteIndex == 1)
			{
				FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
				// ���� ���͸� ���� ���� �ִϸ��̼� ���
				ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
				WallTileMap->SetDestroyObject(TargetIndex, Object);
				// ��ġ�� ���� �ִϸ��̼� ��ġ
				Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
				Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			}
			
			WallTileMap->RemoveTile(TargetPos);
		}
	}

	// ������ Ȯ�� ó��
	int RightCount = 0;
	for (int i = 1; i <= _Power - 1; i++)
	{
		Tile* TileDataRight = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 32, 0 } *static_cast<float>(i));

		if (TileDataRight == nullptr || TileDataRight->SpriteIndex == 2)
		{
			break; // ������ Ȯ�� �ߴ�
		}

		USpriteRenderer* Explode_RightMid = CreateDefaultSubObject<USpriteRenderer>();
		//Explode_RightMid->SetSprite("ExplodeRightMid.png");
		Explode_RightMid->CreateAnimation("Bomb_RightMid", "ExplodeRightMid.png", 0, 19, ExplodeFrameSpeed, false);
		Explode_RightMid->ChangeAnimation("Bomb_RightMid");
		Explode_RightMid->SetComponentScale({ 32, 32 });
		Explode_RightMid->SetOrder((Pos + FVector2D{ 32, 0 } *static_cast<float>(i)).Y);
		Explode_RightMid->SetComponentLocation(FVector2D{ 32, 0 } *static_cast<float>(i));

		if (TileDataRight->SpriteIndex == 1)
		{
			FVector2D TilePos_Location = Explode_RightMid->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;
			WallTileMap->RemoveTile(TargetPos);

			FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
			// ���� ���͸� ���� ���� �ִϸ��̼� ���
			ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
			WallTileMap->SetDestroyObject(TargetIndex, Object);
			// ��ġ�� ���� �ִϸ��̼� ��ġ
			Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
			Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			break;
		}
		RightCount = i;
	}
	Tile* TileDataRight = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 32, 0 } *(static_cast<float>(RightCount + 1)));
	if (TileDataRight != nullptr)
	{
		if (TileDataRight->SpriteIndex != 2 && RightCount + 1 > 0)
		{
			USpriteRenderer* Explode_Right = CreateDefaultSubObject<USpriteRenderer>();
			//Explode_Right->SetSprite("ExplodeRight.png");
			Explode_Right->CreateAnimation("Bomb_Right", "ExplodeRight.png", 0, 19, ExplodeFrameSpeed, false);
			Explode_Right->ChangeAnimation("Bomb_Right");
			Explode_Right->SetComponentScale({ 32, 32 });
			Explode_Right->SetOrder((Pos + FVector2D{ 32, 0 }).Y);
			Explode_Right->SetComponentLocation(FVector2D{ 32, 0 } *(static_cast<float>(RightCount + 1)));

			FVector2D TilePos_Location = Explode_Right->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;

			if (TileDataRight->SpriteIndex == 1)
			{
				FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
				// ���� ���͸� ���� ���� �ִϸ��̼� ���
				ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
				WallTileMap->SetDestroyObject(TargetIndex, Object);
				// ��ġ�� ���� �ִϸ��̼� ��ġ
				Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
				Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			}
			
			WallTileMap->RemoveTile(TargetPos);
		}
	}

	// �Ʒ� Ȯ�� ó��
	int DownCount = 0;
	for (int i = 1; i <= _Power - 1; i++)
	{
		Tile* TileDataDown = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 0, 32 } *static_cast<float>(i));

		if (TileDataDown == nullptr || TileDataDown->SpriteIndex == 2)
		{
			break; // ������ Ȯ�� �ߴ�
		}

		USpriteRenderer* Explode_DownMid = CreateDefaultSubObject<USpriteRenderer>();
		//Explode_DownMid->SetSprite("ExplodeDownMid.png");
		Explode_DownMid->CreateAnimation("Bomb_DownMid", "ExplodeDownMid.png", 0, 19, ExplodeFrameSpeed, false);
		Explode_DownMid->ChangeAnimation("Bomb_DownMid");
		Explode_DownMid->SetComponentScale({ 32, 32 });
		Explode_DownMid->SetOrder((Pos + FVector2D{ 0, 32 } *static_cast<float>(i)).Y);
		Explode_DownMid->SetComponentLocation(FVector2D{ 0, 32 } *static_cast<float>(i));

		if (TileDataDown->SpriteIndex == 1)
		{
			FVector2D TilePos_Location = Explode_DownMid->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;
			WallTileMap->RemoveTile(TargetPos);

			FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
			// ���� ���͸� ���� ���� �ִϸ��̼� ���
			ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
			WallTileMap->SetDestroyObject(TargetIndex, Object);
			// ��ġ�� ���� �ִϸ��̼� ��ġ
			Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
			Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			break;
		}
		DownCount = i;
	}
	Tile* TileDataDown = WallTileMap->GetTileRef(Pos_Minus_TileHalfSize + FVector2D{ 0, 32 } *(static_cast<float>(DownCount + 1)));
	if (TileDataDown != nullptr)
	{
		if (TileDataDown->SpriteIndex != 2 && DownCount + 1 > 0)
		{
			USpriteRenderer* Explode_Down = CreateDefaultSubObject<USpriteRenderer>();
			//Explode_Down->SetSprite("ExplodeDown.png");
			Explode_Down->CreateAnimation("Bomb_Down", "ExplodeDown.png", 0, 19, ExplodeFrameSpeed, false);
			Explode_Down->ChangeAnimation("Bomb_Down");
			Explode_Down->SetComponentScale({ 32, 32 });
			Explode_Down->SetOrder((Pos + FVector2D{ 0, 32 }).Y);
			Explode_Down->SetComponentLocation(FVector2D{ 0, 32 } *(static_cast<float>(DownCount + 1)));

			FVector2D TilePos_Location = Explode_Down->GetComponentLocation();
			FVector2D BombPos_Location = GetActorLocation() - WallTileMap->GetActorLocation();
			FVector2D TargetPos = BombPos_Location + TilePos_Location;

			if (TileDataDown->SpriteIndex == 1)
			{
				FIntPoint TargetIndex = WallTileMap->LocationToIndex(TargetPos);
				// ���� ���͸� ���� ���� �ִϸ��̼� ���
				ATile_Destroy* Object = GetWorld()->SpawnActor<ATile_Destroy>();
				WallTileMap->SetDestroyObject(TargetIndex, Object);
				// ��ġ�� ���� �ִϸ��̼� ��ġ
				Object->SetWallTileMap(WallTileMap, TargetIndex); // Ÿ�ϸ� ���� ����
				Object->SetActorLocation(GetActorLocation() + TilePos_Location);
			}

			WallTileMap->RemoveTile(TargetPos);
		}
	}

}