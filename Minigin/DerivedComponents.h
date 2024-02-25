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
}

