#include "Flash.hpp"

namespace Sonar
{
	Flash::Flash(GameDataRef data) : _data(data)
	{
		_shape = sf::RectangleShape(sf::Vector2f((float)_data->window.getSize().x, (float)_data->window.getSize().y));
		_shape.setFillColor(sf::Color(255, 255, 255, 0));

		_flashOn = true;
	}

	Flash::~Flash()
	{
	}

	void Flash::Show(float dt)
	{
		if (_flashOn)
		{
			int alpha = (int)(_shape.getFillColor().a + (FLASH_SPEED * dt));

			if (alpha >= 255)
			{
				alpha = 255;
				_flashOn = false;
			}

			_shape.setFillColor(sf::Color(255, 255, 255, alpha));
		}
		else
		{
			int alpha = (int)(_shape.getFillColor().a - (FLASH_SPEED * dt));

			if (alpha <= 0)
			{
				alpha = 0;
			}

			_shape.setFillColor(sf::Color(255, 255, 255, alpha));
		}
	}

	void Flash::Draw()
	{
		_data->window.draw(_shape);
	}
}