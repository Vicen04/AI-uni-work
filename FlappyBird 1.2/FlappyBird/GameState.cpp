#pragma once

#include <sstream>
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include "GameOverState.hpp"
#include "AIController.h"
#include <iostream>

#define PLAY_WITH_AI 1

namespace Sonar
{
	GameState::GameState(GameDataRef data, std::vector <AIController*> AIControllers) : _data(data), m_pAIControllers(AIControllers)
	{
		for (int i = 0; i < birdsCreated; i++)
		{
			m_pAIControllers.at(i)->setGameState(this);
		}
	}

	GameState::~GameState()
	{
		delete pipe;
		delete land;
		delete flash;
		delete hud;
	}

	void GameState::Init()
	{
		if (!_hitSoundBuffer.loadFromFile(HIT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Hit Sound Effect" << std::endl;
		}

		if (!_wingSoundBuffer.loadFromFile(WING_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Wing Sound Effect" << std::endl;
		}

		if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Point Sound Effect" << std::endl;
		}

		_hitSound.setBuffer(_hitSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);
		_pointSound.setBuffer(_pointSoundBuffer);

		this->_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);
		this->_data->assets.LoadTexture("Land", LAND_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 1", BIRD_FRAME_1_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 2", BIRD_FRAME_2_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 3", BIRD_FRAME_3_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 4", BIRD_FRAME_4_FILEPATH);
		this->_data->assets.LoadTexture("Scoring Pipe", SCORING_PIPE_FILEPATH);
		this->_data->assets.LoadFont("Flappy Font", FLAPPY_FONT_FILEPATH);

		pipe = new Pipe(_data);
		land = new Land(_data);
		for (int i = 0; i < birdsCreated; i++)
		{
			birds.push_back(new Bird(_data));
			m_pAIControllers.at(i)->setValues(i);
		}
		flash = new Flash(_data);
		hud = new HUD(_data);
		

		_background.setTexture(this->_data->assets.GetTexture("Game Background"));

		_score = 0;
		hud->UpdateScore(_score);

		_gameState = GameStates::eReady;
	}

	void GameState::HandleInput()
	{
#if PLAY_WITH_AI
		if (GameStates::eGameOver != _gameState)
		{
			_gameState = GameStates::ePlaying;
			for (int i = 0; i < birds.size(); i++)
			{
				m_pAIControllers.at(i)->update();

				if (m_pAIControllers.at(i)->shouldFlap())
				{				
					birds.at(i)->Tap();
					_wingSound.play();
				}
			}
		}

#endif

		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}

			if (this->_data->input.IsSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
			{
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					for (int i = 0; i < birds.size(); i++)
					{
						birds.at(i)->Tap();
					}

					_wingSound.play();
				}
			}
		}
	}

	void GameState::Update(float dt)
	{
		if (GameStates::eGameOver != _gameState)
		{
			for (int i = 0; i < birds.size(); i++)
			{
				birds.at(i)->Animate(dt);
			}
			land->MoveLand(dt);
		}

		if (GameStates::ePlaying == _gameState)
		{
			pipe->MovePipes(dt);

			if (clock.getElapsedTime().asSeconds() > PIPE_SPAWN_FREQUENCY)
			{
				pipe->RandomisePipeOffset();
				

				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
				pipe->SpawnScoringPipe();

				clock.restart();
			}

			for (int i = 0; i < birds.size(); i++)
			{
				birds.at(i)->Update(dt);
				m_pAIControllers.at(i)->setValues(i);
			}

			std::vector<sf::Sprite> landSprites = land->GetSprites();

			for (unsigned int i = 0; i < landSprites.size(); i++)
			{
				for (unsigned int j = 0; j < birds.size(); j++)
				{
					if (collision.CheckSpriteCollision(birds.at(j)->GetSprite(), 0.7f, landSprites.at(i), 1.0f, false))
					{
						m_pAIControllers.at(j)->SetSurvivalTime(lifetime.getElapsedTime().asSeconds());
						if (birds.size() < 26)
						std::cout << "bird " << birds.size() << " lasted: " <<lifetime.getElapsedTime().asSeconds() << " seconds. Score: " << _score << std::endl;
						std::swap(birds.at(j), birds.at(birds.size() - 1));
						delete birds.at(birds.size() - 1);
						birds.pop_back();
						std::swap(m_pAIControllers.at(j), m_pAIControllers.at(birds.size()));
						_hitSound.play();
				
						if (birds.size() <= 0)
						{
							_gameState = GameStates::eGameOver;
						}
	
					}
				}
				
			}

			std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();

			for (unsigned int i = 0; i < pipeSprites.size(); i++)
			{
				for (unsigned int j = 0; j < birds.size(); j++)
				{
					if (collision.CheckSpriteCollision(birds.at(j)->GetSprite(), 0.625f, pipeSprites.at(i), 1.0f, true))
					{
						m_pAIControllers.at(j)->SetSurvivalTime(lifetime.getElapsedTime().asSeconds());
						if (birds.size() < 26)
							std::cout << "bird " << birds.size() << " lasted: " << lifetime.getElapsedTime().asSeconds() << " seconds. Score: " << _score << std::endl;
						std::swap(birds.at(j), birds.at(birds.size() - 1));
						delete birds.at(birds.size() - 1);
						birds.pop_back();
						std::swap(m_pAIControllers.at(j), m_pAIControllers.at(birds.size()));
						_hitSound.play();
						j--;
						if (birds.size() <= 0)
						{
							_gameState = GameStates::eGameOver;
						}
					}
				}
			}

			if (GameStates::ePlaying == _gameState)
			{
				std::vector<sf::Sprite> &scoringSprites = pipe->GetScoringSprites();

				for (unsigned int i = 0; i < scoringSprites.size(); i++)
				{
					for (unsigned int j = 0; j < birds.size(); j++)
					{
						if (collision.CheckSpriteCollision(birds.at(j)->GetSprite(), 0.625f, scoringSprites.at(i), 1.0f, false))
						{
							_score++;

							hud->UpdateScore(_score);

							scoringSprites.erase(scoringSprites.begin() + i);

							_pointSound.play();
						}
					}
				}
			}
		}

		if (GameStates::eGameOver == _gameState)
		{
			flash->Show(dt);			

			if (clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS)
			{
				CreateNewGeneration();
				this->_data->machine.AddState(StateRef(new GameOverState(_data, _score, m_pAIControllers)), true);
			}

		}
	}

	void GameState::Draw(float dt)
	{
			this->_data->window.clear(sf::Color::Red);

			this->_data->window.draw(this->_background);

			pipe->DrawPipes();
			land->DrawLand();
			for (int i = 0; i < birds.size(); i++)
			{
				birds.at(i)->Draw();
			}
			flash->Draw();

			hud->Draw();
		

		this->_data->window.display();
	}

	void GameState::CreateNewGeneration()
	{
		for (int i = 0; i < birdsCreated; i++)
		{
			for (int j = i + 1; j < birdsCreated; j++)
			{
				if (m_pAIControllers.at(i)->GetScore() < m_pAIControllers.at(j)->GetScore())
					std::swap(m_pAIControllers.at(i), m_pAIControllers.at(j));
			}
		}
		std::vector<AIController*> temp;
		for (int i = 0; i < birdsCreated; i++)
		{
			m_pAIControllers.at(i)->setGameState(nullptr);
			m_pAIControllers.at(i)->SetGeneration();
			temp.push_back(m_pAIControllers.at(i));
		}

		int rand1 = 0, rand2 = 1;

		for (int i = 0; i < birdsCreated / 2; i++)
		{
			rand1 = rand() % temp.size();
			rand2 = rand() % temp.size();
			while (rand1 == rand2)
				rand2 = rand() % temp.size();

			m_pAIControllers.push_back(new AIController(m_pAIControllers.at(rand1)->GetWeights(), m_pAIControllers.at(rand2)->GetWeights()));
			m_pAIControllers.at(m_pAIControllers.size() - 1)->SetGeneration(m_pAIControllers.at(0)->GetGeneration());

			temp.erase(temp.begin() + rand1);
			if (rand1 > rand2)
				temp.erase(temp.begin() + rand2);
			else
				temp.erase(temp.begin() + rand2 - 1);
			
		}
		for (int i = birdsCreated / 4; i < birdsCreated; i++)
		{
			delete m_pAIControllers.at(i);
		}
		m_pAIControllers.erase(m_pAIControllers.begin() + (birdsCreated / 4), m_pAIControllers.begin() + birdsCreated);

		m_pAIControllers.shrink_to_fit();

		temp.clear();
		temp.shrink_to_fit();

		for (int i = 0; i < birdsCreated / 4; i++)
		{
			m_pAIControllers.push_back(new AIController());
			m_pAIControllers.at(m_pAIControllers.size() - 1)->SetGeneration(m_pAIControllers.at(0)->GetGeneration());
		}

		std::cout << std::endl;
		std::cout << "Generation " << m_pAIControllers.at(0)->GetGeneration() << std::endl;
		std::cout << std::endl;
	}
}