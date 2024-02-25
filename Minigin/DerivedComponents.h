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
		void Render(const Transform& transform) const override;

		void SetText(const std::string& text);

		TextComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};

	// Texture Component
	class TextureComponent : public Component
	{
	public:
		void Update() override;
		void Render(const Transform& transform) const override;

		void SetTexture(const std::string& filename);

		TextureComponent(const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};
}

