#include "PreCompile.h"
#include "BombmanCore.h"
#include <EngineCore/EngineAPICore.h>

#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>

#include "TitleGameMode.h"
#include "PlayGameMode.h"
#include "TileMapGameMode.h"
#include "Player.h"


BombmanCore::BombmanCore()
{
}

BombmanCore::~BombmanCore()
{
}

void BombmanCore::BeginPlay()
{

	UEngineDirectory Dir;

	if (false == Dir.MoveParentToDirectory("Resources//Imgs"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}


	{
		UEngineDirectory Title_Basic_Dir;
		Title_Basic_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_Basic_Dir.Append("01_TITLE//00_00_BASIC");
		UImageManager::GetInst().LoadFolder(Title_Basic_Dir.GetPathToString());
	}

	{
		UEngineDirectory Title_NeoGeo_Dir;
		Title_NeoGeo_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_NeoGeo_Dir.Append("01_TITLE//00_Neo-Geo_LOGO");
		UImageManager::GetInst().LoadFolder(Title_NeoGeo_Dir.GetPathToString());
	}
	{

		UEngineDirectory Title_Warning_Dir;
		Title_Warning_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_Warning_Dir.Append("01_TITLE//01_WARNING");
		UImageManager::GetInst().LoadFolder(Title_Warning_Dir.GetPathToString());
	}
	{

		UEngineDirectory Title_Hudson_Dir;
		Title_Hudson_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_Hudson_Dir.Append("01_TITLE//02_HUDSON_LOGO");
		UImageManager::GetInst().LoadFolder(Title_Hudson_Dir.GetPathToString());
	}
	{

		UEngineDirectory Title_OP_Animation_Dir;
		Title_OP_Animation_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_OP_Animation_Dir.Append("01_TITLE//03_OP_ANIMATION");
		UImageManager::GetInst().LoadFolder(Title_OP_Animation_Dir.GetPathToString());
	}
	{

		UEngineDirectory Title_CoinInsert_Dir;
		Title_CoinInsert_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_CoinInsert_Dir.Append("01_TITLE//04_COIN_INSERT");
		UImageManager::GetInst().LoadFolder(Title_CoinInsert_Dir.GetPathToString());
	}
	{

		UEngineDirectory Title_ChooseStage_Dir;
		Title_ChooseStage_Dir.MoveParentToDirectory("Resources//Imgs");
		Title_ChooseStage_Dir.Append("01_TITLE//05_CHOOSE_STAGE");
		UImageManager::GetInst().LoadFolder(Title_ChooseStage_Dir.GetPathToString());
	}



	{
		UEngineDirectory Play_TILE_Dir;
		Play_TILE_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_TILE_Dir.Append("02_PLAY//00_Tiles");
		UImageManager::GetInst().LoadFolder(Play_TILE_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_BG_Dir;
		Play_BG_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_BG_Dir.Append("02_PLAY//01_Background");
		UImageManager::GetInst().LoadFolder(Play_BG_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Bomb_Dir;
		Play_Bomb_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Bomb_Dir.Append("02_PLAY//02_Objects//01_Bomb");
		UImageManager::GetInst().LoadFolder(Play_Bomb_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Idle_Dir;
		Play_Mushroom_Idle_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Idle_Dir.Append("02_PLAY//03_Monster//01_Mushroom_00_Idle");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Idle_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Left_Dir;
		Play_Mushroom_Left_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Left_Dir.Append("02_PLAY//03_Monster//01_Mushroom_01_Left");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Left_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Right_Dir;
		Play_Mushroom_Right_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Right_Dir.Append("02_PLAY//03_Monster//01_Mushroom_02_Right");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Right_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Up_Dir;
		Play_Mushroom_Up_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Up_Dir.Append("02_PLAY//03_Monster//01_Mushroom_03_Up");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Up_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Down_Dir;
		Play_Mushroom_Down_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Down_Dir.Append("02_PLAY//03_Monster//01_Mushroom_04_Down");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Down_Dir.GetPathToString());
	}
	{
		UEngineDirectory Play_Mushroom_Uniq_Dir;
		Play_Mushroom_Uniq_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_Mushroom_Uniq_Dir.Append("02_PLAY//03_Monster//01_Mushroom_05_Uniq");
		UImageManager::GetInst().LoadFolder(Play_Mushroom_Uniq_Dir.GetPathToString());
	}

	{
		UEngineDirectory Play_TestBG_Dir;
		Play_TestBG_Dir.MoveParentToDirectory("Resources//Imgs");
		Play_TestBG_Dir.Append("02_PLAY//EX_BG");
		UImageManager::GetInst().LoadFolder(Play_TestBG_Dir.GetPathToString());
	}


	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle(":: LHJ :: Neo-Bomberman ::");

	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 200,200 }, { 608, 448 });

	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");

	UEngineAPICore::GetCore()->CreateLevel<ATileMapGameMode, AActor>("Tile");


	UEngineAPICore::GetCore()->OpenLevel("Tile");
}

void BombmanCore::Tick()
{
}