#pragma once

#include <functional>
#include <SFML/Window/Keyboard.hpp>

namespace ott
{
	class Input
	{
	public:
		Input();

		struct Key
		{
			bool pressed = false;

			std::function<void()> onPress;
			std::function<void()> onHold;
			std::function<void()> onRelease;
		};

		void AddPressFunction(sf::Keyboard::Key key, std::function<void()>& onPress);
		void ClearPressFunction(sf::Keyboard::Key key);

		void AddHoldFunction(sf::Keyboard::Key key, std::function<void()>& onHold);
		void ClearHoldFunction(sf::Keyboard::Key key);

		void AddReleaseFunction(sf::Keyboard::Key key, std::function<void()>& onRelease);
		void ClearReleaseFunction(sf::Keyboard::Key key);

		void ClearKeyFunctions(sf::Keyboard::Key key);
		void Clear();

		void HandleKeyPress(sf::Keyboard::Key key);
		void HandleKeyRelease(sf::Keyboard::Key key);

		bool IsKeyPressed(sf::Keyboard::Key key);

		void Update();

	private:
		std::map<sf::Keyboard::Key, Key> _keys;
	};
}
