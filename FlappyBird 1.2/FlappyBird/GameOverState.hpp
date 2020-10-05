#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace Sonar
{
    class GameOverState : public State
    {
    public:
        GameOverState(GameDataRef data, int score, std::vector <AIController*> AIControllers);
		~GameOverState();
        
        void Init();
        
        void HandleInput();
        void Update(float dt);
        void Draw(float dt);
        
    private:
        GameDataRef _data;
        
        sf::Sprite _background;
        
        sf::Sprite _gameOverTitle;
        sf::Sprite _gameOverContainer;
        sf::Sprite _retryButton;
        sf::Sprite _medal;
        
        sf::Text _scoreText;
        sf::Text _highScoreText;

		sf::Clock clock;
        
        int _score;
        int _highScore;
		std::vector <AIController*> m_pAIControllers;
    };
}










