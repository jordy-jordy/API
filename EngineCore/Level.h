#pragma once
#include "GameMode.h"

// ���� :
class ULevel
{
public:
	friend class UEngineAPICore;
	// constrcuter destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);
	void Render();

	template<typename ActorType>
	ActorType* SpawnActor()
	{
		ActorType* NewActor = new ActorType();

		AActor* ActorPtr = dynamic_cast<AActor*>(NewActor);
		// ���� �� ���� �����̾�.
		ActorPtr->World = this;

		NewActor->BeginPlay();
		AllActors.push_back(NewActor);
		return NewActor;
	}

protected:

private:
	void ScreenClear();
	void DoubleBuffering();


	// ���ӷ����� �������� ���� ������ �غ��Ű�� �Լ��ε� �������.
	template<typename GameModeType, typename MainPawnType>
	void CreateGameMode()
	{
		GameMode = new GameModeType();

		// ȭ���� �ٶ���ִ� ī�޶��� �����ϰ� ����ŵ� �˴ϴ�.
		MainPawn = new MainPawnType();

		// ���弼���� �����Ǵ°��� ������ �߿��ϴ�.
		MainPawn->World = this;
		GameMode->World = this;

		GameMode->BeginPlay();
		MainPawn->BeginPlay();

		AllActors.push_back(GameMode);
		AllActors.push_back(MainPawn);
	}

	// �밡���� ǥ���
	// �̸��� �������
	// �ɹ������� �̸��� �빮��
	// ��������������
	// dujumsaigury
	// �����ǹ̷� ���ֽø� �˴ϴ�.
	// �Ǿո� 
	class AGameMode* GameMode = nullptr;

	// ���ΰ�
	class AActor* MainPawn = nullptr;

	std::list<AActor*> AllActors;
};