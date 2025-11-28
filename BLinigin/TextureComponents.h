#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <SDL.h>
#include <string>

namespace dae {
	class Font;
	class Texture2D;

	// TODO : Merge Text, Texture components
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
		Texture2D* m_pTexture{};
	};

	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		inline std::string GetText() const { return m_Text; };
		void SetText(const std::string& text);
		void SetCentered(bool setCentered) { m_Centered = setCentered; };

		TextComponent(dae::GameObject* pParent, const std::string& text, dae::Font* font, const std::string& internalIdentifier, bool isCentered = false, SDL_Color color = { 255, 255, 255, 255 });
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_Centered;
		bool m_NeedsUpdate;
		std::string m_Text;
		const std::string m_ResourceFile;
		Font* m_pFont{};
		Texture2D* m_pTextTexture{};
		SDL_Color m_Color;
	};

	class AnimatedTextureComponent final : public TextureComponent
	{
	public:
		void Update() override;
		void Render() const override;

		glm::vec2 GetAppliedTextureSize() const;
		float GetTimeForLoop() const;
		void NextLayer();
		inline void ToggleAnimation() { m_DoAnimation = !m_DoAnimation; };
		inline void StopAnimating() { m_DoAnimation = false; };
		inline void StartAnimating() { m_DoAnimation = true; };

		AnimatedTextureComponent(dae::GameObject* pParent, const std::string& fileName, int numFrames, float fps, int numLayers = 1);
		AnimatedTextureComponent(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent(const AnimatedTextureComponent&& other) = delete;
		AnimatedTextureComponent& operator=(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent& operator=(const AnimatedTextureComponent&& other) = delete;

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
}