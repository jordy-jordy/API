#pragma once
#include <EngineCore/Actor.h>

// ���� :
class APortal : public AActor
{
public:
	// constrcuter destructer
	APortal();
	~APortal();

	// delete Function
	APortal(const APortal& _Other) = delete;
	APortal(APortal&& _Other) noexcept = delete;
	APortal& operator=(const APortal& _Other) = delete;
	APortal& operator=(APortal&& _Other) noexcept = delete;

protected:

private:
	class USpriteRenderer* SpriteRenderer;
};
