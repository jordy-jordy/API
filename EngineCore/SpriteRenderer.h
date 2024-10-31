#pragma once
#include "SceneComponent.h"

// ���� :
class USpriteRenderer : public USceneComponent
{
	// �ְ� �� ����Ѵ�.
	// Ŭ������ ��ȭ�з��ؼ�
public:
	class FrameAnimation
	{
		//int Start = 0;
		//int End = 0;

		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;

		float Inter;
	};

public:
	// constrcuter destructer
	USpriteRenderer();
	~USpriteRenderer();

	// delete Function
	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	void Render();
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

	// int�� �ִ� �Լ����� �Ϲ������� Enum���� ��ü�ؼ� �ְ� �������� ����.
	// �׷��� �װ� Ŀ���� ����� ����� ����� �ڽŸ��� enum�� ���̱� ������ 
	// ���ø��� ����Ͽ� � enum�̰� �ް� ����� ����� ��ȣ�Ѵ�.
	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	int GetOrder()
	{
		return Order;
	}

	FVector2D SetSpriteScale(float _Ratio = 1.0f, int _CurIndex = 0);

	void CreateAnimation(std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame);

protected:

public:
	int Order = 0;
	class UEngineSprite* Sprite = nullptr;
	int CurIndex = 0;
	void SetSprite(std::string_view _Name, int _CurIndex = 0);
};

