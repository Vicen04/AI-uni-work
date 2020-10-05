#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace Sonar
{
	class MainMenuState : public State
	{
	public:
		MainMenuState(GameDataRef data, std::vector <AIController*> AIControllers);
		~MainMenuState();

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;
		std::vector <AIController*> m_pAIControllers;
		sf::Sprite _background;
		sf::Sprite _title;
		sf::Sprite _playButton;
	};
}