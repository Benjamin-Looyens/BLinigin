#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm/vec2.hpp>

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const dae::Texture2D& texture, float x, float y) const;
		void RenderTexture(const dae::Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const dae::Texture2D& texture, SDL_Rect src, SDL_Rect dst) const;
		void RenderTexture(const dae::Texture2D& texture, SDL_Rect src, SDL_FRect dst) const;
		void RenderTexture(const dae::Texture2D& texture, SDL_FRect src, SDL_FRect dst) const;
		
		inline SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; };
		glm::vec2 GetWindowSize() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_pWindow{};
		SDL_Color m_ClearColor{};
	};
}

#define BL_RENDERER dae::Renderer::GetInstance()