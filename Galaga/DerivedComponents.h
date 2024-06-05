#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>
#include "Observer.h"
#include "ObserverComponents.h"

// TODO : Check & remove virtual where unnecessary (eg: virtual destructors in final)

namespace dae {
	// Class Forward Declarations
	class Font;
	class Texture2D;
	class IObserver;
	enum class Event;
	class IObserver;
	class ISubject;

	// TODO : Merge Text, Texture components & Move them to Engine Files
	// Text Component
	class TextComponent final : public Component
	{
	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);

		TextComponent(dae::GameObject* pParent, const std::string& text, Font* font, const std::string& internalIdentifier);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		const std::string m_ResourceFile;
		Font* m_pFont;
		Texture2D* m_pTextTexture;
	};

	// Texture Component
	class TextureComponent : public Component
	{
	public:
		virtual void Render() const override;

		void SetTexture(const std::string& filename);
		Texture2D* GetTexture() const { return m_pTexture; };

		TextureComponent(dae::GameObject* pParent, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		float GetTextureWidth() const;
		float GetTextureHeight() const;
		glm::vec2 GetTextureSize() const;

	protected:
		Texture2D* m_pTexture;
	};

	class AnimatedTextureComponent final : public TextureComponent
	{
	public:
		void Update() override;
		void Render() const override;

		glm::vec2 GetAppliedTextureSize() const;
		float GetTimeForLoop() const;
		void NextLayer();
		void ToggleAnimation() { m_DoAnimation = !m_DoAnimation; };
		void StopAnimating() { m_DoAnimation = false; };
		void StartAnimating() { m_DoAnimation = true; };

		AnimatedTextureComponent(dae::GameObject* pParent, const std::string& fileName, int numFrames, float fps, int numLayers = 1);
	
	private:
		int m_NumFrames;
		int m_CurFrame{ };

		float m_FPS;
		float m_ElapsedTime{ };
		
		int m_NumLayers;
		int m_CurLayer{ };

		glm::ivec2 m_FrameWidthHeight;

		bool m_DoAnimation = true;
	};

	// FPS Component
	class FPSComponent final : public Component
	{
	public:
		virtual void Update() override;

		FPSComponent(dae::GameObject* pParent, float displayUpdateInterval = 0.25f);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:
		dae::TextComponent* m_pTextLocation; // non-owning reference to parent's text component (if it has one)

		float m_CounterInterval;
		float m_Counter{};
	};

	class WaveMovementYComponent final : public Component
	{
	public:
		virtual void Update() override;

		WaveMovementYComponent(GameObject* pParent, float waveHeight = 10.0f, float waveSpeed = 1.0f);
		virtual ~WaveMovementYComponent() = default;
		WaveMovementYComponent(const WaveMovementYComponent& other) = delete;
		WaveMovementYComponent(const WaveMovementYComponent&& other) = delete;
		WaveMovementYComponent& operator=(const WaveMovementYComponent& other) = delete;
		WaveMovementYComponent& operator=(const WaveMovementYComponent&& other) = delete;

	private:
		glm::vec2 m_Origin;

		float m_WaveHeight;
		float m_WaveSpeed;

		float m_WaveTime{ 0.0f };
	};

	class DisplayDataComponent final : public Component
	{
	public:
		void UpdateDisplay(std::string dataStr);

		DisplayDataComponent(GameObject* pParent, std::string dataPrefix, std::string startData = "");
		virtual ~DisplayDataComponent() = default;
		DisplayDataComponent(const DisplayDataComponent& other) = delete;
		DisplayDataComponent(const DisplayDataComponent&& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent&& other) = delete;

	private:
		dae::TextComponent* m_pTextLocation; // non-owning reference to parent's text component (if it has one)
		std::string m_DataPrefix;
	};

	class FollowMouseComponent final : public Component
	{
	public:
		FollowMouseComponent(GameObject* pParent);

		void Update() override;
	};

	class ConstantMovementComponent final : public Component
	{
	public:
		ConstantMovementComponent(GameObject* pParent, float speed, glm::vec2 movDir);

		void Update() override;

	private:
		float m_Speed;
		glm::vec2 m_MovDir;
	};

	class TimedDestroyComponent final : public Component
	{
	public:
		TimedDestroyComponent(GameObject* pParent, float timeTillDestroy);
		void Update() override;

	private:
		float m_ElapsedTime{};
		float m_TimeTillDestroy;
	};

	class IncrementingScaleComponent final : public Component
	{
	public:
		IncrementingScaleComponent(GameObject* pParent, float incrementPerSecond = 1.0f);

		void Update() override;

	private:
		float m_IncrPerSecond;
	};

}

