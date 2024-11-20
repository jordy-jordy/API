#include "PreCompile.h"
#include "TileMapGameMode.h"

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>

#include "ContentsEnum.h"
#include "TestMap.h"


ATileMapGameMode::ATileMapGameMode()
{
}

ATileMapGameMode::~ATileMapGameMode()
{
}

void ATileMapGameMode::BeginPlay()
{
	Super::BeginPlay();
	{
		ATestMap* NewActor = GetWorld()->SpawnActor<ATestMap>();
		SpriteRendererSTAGE = NewActor->SpriteRenderer;
		SpriteRendererSTAGE->ChangeAnimation("Stage1_EX_BG");
	}

	{
		WallTileMap = GetWorld()->SpawnActor<ATileMap>();
		WallTileMap->Create("00_Tiles_00_STAGE", { 13, 11 }, { 32, 32 });
		WallTileMap->SetActorLocation({ 96, 64 });
	}
}

void ATileMapGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();

		FVector2D TileMapLocation = WallTileMap->GetActorLocation();
		FVector2D LocalMousePos = MousePos - TileMapLocation;

		FIntPoint TileIndex = WallTileMap->LocationToIndex(LocalMousePos);

		FVector2D Pivot = FVector2D::ZERO;
		FVector2D SpriteScale = FVector2D(32, 32); 

		if (CurrentTileType == AStageTiles::Object_Unbroken)
		{
			Pivot = FVector2D(0, -6);
			SpriteScale = FVector2D(32, 44);
		}

		int SpriteIndex = static_cast<int>(CurrentTileType);

		// SetTileIndex�� ����Ͽ� ���� Ÿ�� Ÿ�Կ� �´� �ε��� ����
		WallTileMap->SetTileIndex(TileIndex, Pivot, SpriteScale, SpriteIndex);
	}

	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();

		FVector2D TileMapLocation = WallTileMap->GetActorLocation();
		FVector2D LocalMousePos = MousePos - TileMapLocation;

		FIntPoint TileIndex = WallTileMap->LocationToIndex(LocalMousePos);

		Tile* Tile = WallTileMap->GetTileRef(LocalMousePos);
		if (nullptr != Tile->SpriteRenderer)
		{
			Tile->SpriteRenderer->Destroy();
			Tile->SpriteRenderer = nullptr;
			Tile->Pivot = FVector2D::ZERO;
			Tile->Scale = FVector2D::ZERO;
			Tile->SpriteIndex = 0;
		}
	}

	if (UEngineInput::GetInst().IsDown('Q'))
	{
		// ���� Ÿ���� ������ ��ȯ�ϰ� +1 ����
		int nextTileType = static_cast<int>(CurrentTileType) + 1;

		// Max�� �ʰ����� �ʰ� NONE�� ���ϵ��� ��ȯ
		if (nextTileType >= static_cast<int>(AStageTiles::Max))
		{
			nextTileType = static_cast<int>(AStageTiles::Object_Broken); // ��ȯ ���� ����
		}

		CurrentTileType = static_cast<AStageTiles>(nextTileType);
	}

	if (true == UEngineInput::GetInst().IsDown('L'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}



	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineSerializer _Ser;
		WallTileMap->Serialize(_Ser);
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}

		Dir.Append("Data");

		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("wb");
		NewFile.Write(_Ser);
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}

		Dir.Append("Data");

		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("rb");

		UEngineSerializer Ser;
		NewFile.Read(Ser);


		WallTileMap->DeSerialize(Ser);

	}

}

