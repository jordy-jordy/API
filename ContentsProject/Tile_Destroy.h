#pragma once
#include <EngineCore/Actor.h>

// ���� :
class ATile_Destroy : public AActor
{
public:
	// constrcuter destructer
	ATile_Destroy();
	~ATile_Destroy();

	// delete Function
	ATile_Destroy(const ATile_Destroy& _Other) = delete;
	ATile_Destroy(ATile_Destroy&& _Other) noexcept = delete;
	ATile_Destroy& operator=(const ATile_Destroy& _Other) = delete;
	ATile_Destroy& operator=(ATile_Destroy&& _Other) noexcept = delete;

	void SetWallTileMap(class ATileMap* _TileMap, FIntPoint _Index);

	void Object_Destroy();
	void ClearObjectTile();

	// �� ��������Ʈ

protected:
	void Tick(float _DeltaTime) override;

private:
	class ATileMap* WallTileMap = nullptr;
	class USpriteRenderer* SpriteRenderer = nullptr;

	FIntPoint ObjectTileIndex;



};
