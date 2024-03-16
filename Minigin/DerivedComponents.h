#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include "Transform.h"

namespace dae {
	// Class Forward Declarations
	class Font;
	class Texture2D;

	// Text Component
	class TextComponent : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);

		TextComponent(dae::GameObject * parent, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;
	};

	// Texture Component
	class TextureComponent : public Component
	{
	public:
		void Render() const override;

		void SetTexture(const std::string& filename);

		TextureComponent(dae::GameObject* parent, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};

	// FPS Component
	class FPSComponent : public Component
	{
	public:
		void Update() override;

		FPSComponent(dae::GameObject* parent, float displayUpdateInterval = 0.25f);
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
}

