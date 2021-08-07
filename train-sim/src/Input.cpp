#include "pch.h"
#include "Input.h"

namespace ott
{
	Input::Input() = default;

	void Input::AddPressFunction(sf::Keyboard::Key key, std::function<void()>& onPress)
	{
		if (!this->_keys.contains(key))
		{
			this->_keys.insert({key, Key()});
		}
		this->_keys[key].onPress = onPress;
	}

	void Input::ClearPressFunction(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key)) { return; }
		this->_keys[key].onPress = nullptr;
	}

	void Input::AddHoldFunction(sf::Keyboard::Key key, std::function<void()>& onHold)
	{
		if (!this->_keys.contains(key))
		{
			this->_keys.insert({key, Key()});
		}
		this->_keys[key].onHold = onHold;
	}

	void Input::ClearHoldFunction(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key)) { return; }
		this->_keys[key].onHold = nullptr;
	}

	void Input::AddReleaseFunction(sf::Keyboard::Key key, std::function<void()>& onRelease)
	{
		if (!this->_keys.contains(key))
		{
			this->_keys.insert({key, Key()});
		}
		this->_keys[key].onRelease = onRelease;
	}

	void Input::ClearReleaseFunction(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key)) { return; }
		this->_keys[key].onRelease = nullptr;
	}

	void Input::ClearKeyFunctions(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key)) { return; }
		this->_keys.erase(key);
	}

	void Input::Clear()
	{
		this->_keys.clear();
	}

	void Input::HandleKeyPress(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key) || this->_keys[key].pressed) return;

		this->_keys[key].pressed = true;
		this->_keys[key].onPress();
	}

	void Input::HandleKeyRelease(const sf::Keyboard::Key key)
	{
		if (!this->_keys.contains(key) || !this->_keys[key].pressed) return;

		this->_keys[key].pressed = false;
		this->_keys[key].onRelease();
	}

	bool Input::IsKeyPressed(const sf::Keyboard::Key key)
	{
		return this->_keys[key].pressed;
	}

	void Input::Update()
	{
		for (auto it = this->_keys.begin(); it != this->_keys.end(); ++it)
		{
			auto& [pressed, onPress, onHold, onRelease] = it->second;
			if (!pressed) continue;
			onHold();
		}
	}
}
