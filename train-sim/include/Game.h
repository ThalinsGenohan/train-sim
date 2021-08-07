#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "World.h"
#include "Constants.h"

namespace ott
{
class Game
{
public:
	inline static const float PAN_MULTIPLIER = 10.f;
	inline static const float ZOOM_FACTOR = sqrtf(2.f);
	inline static const float MAX_ZOOM_IN = 4.f;
	inline static const float MAX_ZOOM_OUT = 0.0625f;

	inline static const sf::Vector2f PREVIEW_UP        = sf::Vector2f(52.f, 816.f);
	inline static const sf::Vector2f PREVIEW_UPRIGHT   = sf::Vector2f(84.f, 816.f);
	inline static const sf::Vector2f PREVIEW_RIGHT     = sf::Vector2f(84.f, 848.f);
	inline static const sf::Vector2f PREVIEW_DOWNRIGHT = sf::Vector2f(84.f, 880.f);
	inline static const sf::Vector2f PREVIEW_DOWN      = sf::Vector2f(52.f, 880.f);
	inline static const sf::Vector2f PREVIEW_DOWNLEFT  = sf::Vector2f(20.f, 880.f);
	inline static const sf::Vector2f PREVIEW_LEFT      = sf::Vector2f(20.f, 848.f);
	inline static const sf::Vector2f PREVIEW_UPLEFT    = sf::Vector2f(20.f, 816.f);

	Game();

	void HandleKeyPress(sf::Event::KeyEvent& key);
	void HandleKeyRelease(sf::Event::KeyEvent& key);
	void HandleKeyHolds();
	void HandleClick(sf::Vector2i mouse, bool right);

	void ChangeRotation(ott_const::Rotation rotation);

	void Run();

private:
	sf::RenderWindow _window;
	sf::View _mainView;
	sf::View _hudView;

	float _zoomLevel;
	bool _tileGridVisible;

	World _world;

	std::map<sf::Keyboard::Key, bool> _keys;
	bool _leftMouseHeld;
	bool _rightMouseHeld;

	sf::VertexArray _trackPreview;

	ott_const::BuildingType _currentBuilding;
	ott_const::Rotation _currentRotation;

	void DrawHUD(float dt);
};
}
