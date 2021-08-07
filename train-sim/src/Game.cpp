#include "pch.h"
#include "Game.h"

#include "World.h"

namespace ott
{
Game::Game()
		:_window(sf::VideoMode(1600, 900), "Train Sim", sf::Style::Close | sf::Style::Titlebar),
		 _mainView(sf::Vector2f(0.f, 0.f), sf::Vector2f(1600, 900)),
		 _hudView(this->_window.getDefaultView()),
		 _zoomLevel(1.f),
		 _trackPreview(sf::LineStrip, 3),
		 _currentBuilding(ott_const::BuildingType::Track),
		 _currentRotation(ott_const::Rotation::Up),
		 _tileGridVisible(true),
		 _leftMouseHeld(false),
		 _rightMouseHeld(false)
{
	this->_window.setFramerateLimit(60);
	_trackPreview[0] = sf::Vertex(PREVIEW_UP, sf::Color::White);
	_trackPreview[1] = sf::Vertex(sf::Vector2f(52.f, 848.f), sf::Color::White);
	_trackPreview[2] = sf::Vertex(PREVIEW_DOWN, sf::Color::White);
}

void Game::HandleKeyPress(sf::Event::KeyEvent& key)
{
	if (this->_keys[key.code])
		return;

	this->_keys[key.code] = true;

	const bool shift = this->_keys[sf::Keyboard::LShift] || this->_keys[sf::Keyboard::RShift];
	const bool ctrl = this->_keys[sf::Keyboard::LControl] || this->_keys[sf::Keyboard::RControl];
	const bool alt = this->_keys[sf::Keyboard::LAlt] || this->_keys[sf::Keyboard::RAlt];
	const bool mod = shift || ctrl || alt;

	switch (key.code)
	{
	case sf::Keyboard::W:
		if (shift)
		{
			if (this->_keys[sf::Keyboard::A])
				this->ChangeRotation(ott_const::Rotation::UpLeft);
			else if (this->_keys[sf::Keyboard::D])
				this->ChangeRotation(ott_const::Rotation::UpRight);
			else
				this->ChangeRotation(ott_const::Rotation::Up);
		}
		break;
	case sf::Keyboard::S:
		if (shift)
		{
			if (this->_keys[sf::Keyboard::A])
				this->ChangeRotation(ott_const::Rotation::DownLeft);
			else if (this->_keys[sf::Keyboard::D])
				this->ChangeRotation(ott_const::Rotation::DownRight);
			else
				this->ChangeRotation(ott_const::Rotation::Down);
		}
		break;
	case sf::Keyboard::A:
		if (shift)
		{
			if (this->_keys[sf::Keyboard::W])
				this->ChangeRotation(ott_const::Rotation::UpLeft);
			else if (this->_keys[sf::Keyboard::S])
				this->ChangeRotation(ott_const::Rotation::DownLeft);
			else
				this->ChangeRotation(ott_const::Rotation::Left);
		}
		break;
	case sf::Keyboard::D:
		if (shift)
		{
			if (this->_keys[sf::Keyboard::W])
				this->ChangeRotation(ott_const::Rotation::UpRight);
			else if (this->_keys[sf::Keyboard::S])
				this->ChangeRotation(ott_const::Rotation::DownRight);
			else
				this->ChangeRotation(ott_const::Rotation::Right);
		}
		break;

	case sf::Keyboard::R:
		if (shift)
		{
			this->ChangeRotation(static_cast<ott_const::Rotation>(this->_currentRotation - 1));
		}
		else
		{
			this->ChangeRotation(static_cast<ott_const::Rotation>(this->_currentRotation + 1));
		}
		std::cout << static_cast<int>(this->_currentRotation) << std::endl;
		break;

	default:;
	}
}

void Game::HandleKeyRelease(sf::Event::KeyEvent& key)
{
	if (!this->_keys[key.code])
		return;

	this->_keys[key.code] = false;
}

void Game::HandleKeyHolds()
{
	const bool shift = this->_keys[sf::Keyboard::LShift] || this->_keys[sf::Keyboard::RShift];
	const bool ctrl = this->_keys[sf::Keyboard::LControl] || this->_keys[sf::Keyboard::RControl];
	const bool alt = this->_keys[sf::Keyboard::LAlt] || this->_keys[sf::Keyboard::RAlt];
	const bool mod = shift || ctrl || alt;

	for (auto&[key, state] : this->_keys)
	{
		if (!state)
			continue;

		switch (key)
		{
		case sf::Keyboard::W:
			if (!mod)
			{
				this->_mainView.move(0.f, -PAN_MULTIPLIER / this->_zoomLevel);
			}
			break;
		case sf::Keyboard::A:
			if (!mod)
			{
				this->_mainView.move(-PAN_MULTIPLIER / this->_zoomLevel, 0.f);
			}
			break;
		case sf::Keyboard::S:
			if (!mod)
			{
				this->_mainView.move(0.f, PAN_MULTIPLIER / this->_zoomLevel);
			}
			break;
		case sf::Keyboard::D:
			if (!mod)
			{
				this->_mainView.move(PAN_MULTIPLIER / this->_zoomLevel, 0.f);
			}
			break;

		default:;
		}
	}
}

void Game::HandleClick(sf::Vector2i mouse, bool right)
{
	auto click = this->_window.mapPixelToCoords(mouse, this->_mainView);
	for (auto& chunk : this->_world.GetChunks())
	{
		if (chunk.GetBounds().contains(click))
		{
			chunk.HandleClick(right, click, this->_currentBuilding, this->_currentRotation);
			return;
		}
	}
}

void Game::Run()
{
	auto font = sf::Font();
	font.loadFromFile("assets/CONSOLA.TTF");

	auto fpsText = sf::Text();
	fpsText.setFillColor(sf::Color::White);
	fpsText.setOutlineColor(sf::Color::Black);
	fpsText.setOutlineThickness(1.f);
	fpsText.setFont(font);
	fpsText.setCharacterSize(11);

	auto trackRect = new sf::RectangleShape(sf::Vector2f(72.f, 72.f));
	trackRect->setPosition(16.f, 812.f);
	trackRect->setFillColor(sf::Color::Black);
	trackRect->setOutlineThickness(1.f);
	trackRect->setOutlineColor(sf::Color::White);

	const auto clock = sf::Clock();
	unsigned lastSecond = 0;
	sf::Time lastTime = clock.getElapsedTime();

	while (this->_window.isOpen())
	{
		sf::Event evnt{ };
		while (this->_window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed: this->_window.close();
				break;
			case sf::Event::KeyPressed: this->HandleKeyPress(evnt.key);
				break;
			case sf::Event::KeyReleased: this->HandleKeyRelease(evnt.key);
				break;
			case sf::Event::MouseWheelScrolled: this->_zoomLevel /= powf(ZOOM_FACTOR, -evnt.mouseWheelScroll.delta);

				if (this->_zoomLevel >= MAX_ZOOM_IN)
					this->_zoomLevel = MAX_ZOOM_IN;
				if (this->_zoomLevel <= MAX_ZOOM_OUT)
					this->_zoomLevel = MAX_ZOOM_OUT;

				if (this->_zoomLevel <= 0.25f && this->_tileGridVisible)
				{
					this->_tileGridVisible = false;
					for (auto& chunk : this->_world.GetChunks())
					{
						chunk.SetGridVisible(false);
					}
				}
				else if (this->_zoomLevel > 0.25f && !this->_tileGridVisible)
				{
					this->_tileGridVisible = true;
					for (auto& chunk : this->_world.GetChunks())
					{
						chunk.SetGridVisible(true);
					}
				}

				this->_mainView.setSize(this->_window.getDefaultView().getSize() / this->_zoomLevel);
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					this->_leftMouseHeld = true;
					this->HandleClick(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y), false);
				}
				if (evnt.mouseButton.button == sf::Mouse::Right)
				{
					this->_rightMouseHeld = true;
					this->HandleClick(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y), true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (evnt.mouseButton.button == sf::Mouse::Left) this->_leftMouseHeld = false;
				if (evnt.mouseButton.button == sf::Mouse::Right) this->_rightMouseHeld = false;
				break;

			case sf::Event::MouseMoved:
				break;

			default: break;
			}
		}

		this->HandleKeyHolds();
		if (this->_leftMouseHeld)
			this->HandleClick(sf::Mouse::getPosition(this->_window), false);
		if (this->_rightMouseHeld)
			this->HandleClick(sf::Mouse::getPosition(this->_window), true);

		sf::Time currentTime = clock.getElapsedTime();
		if (floorf(currentTime.asSeconds()) > lastSecond)
		{
			lastSecond = static_cast<unsigned>(currentTime.asSeconds());
			fpsText.setString(std::format("FPS: {0:.0f}", 1.f / (currentTime.asSeconds() - lastTime.asSeconds())));
		}
		lastTime = currentTime;

		this->_window.clear(sf::Color::Black);

		this->_window.setView(this->_mainView);

		for (auto& chunk : this->_world.GetChunks())
		{
			this->_window.draw(chunk);
		}

		this->_window.setView(this->_hudView);

		this->_window.draw(fpsText);
		this->_window.draw(*trackRect);
		this->_window.draw(this->_trackPreview);

		this->_window.display();
	}
}

void Game::DrawHUD(float dt)
{
}
void Game::ChangeRotation(ott_const::Rotation rotation)
{
	this->_currentRotation = static_cast<ott_const::Rotation>(rotation % 8);


	switch (this->_currentRotation)
	{
	case ott_const::Up:
		this->_trackPreview[0].position = PREVIEW_UP;
		this->_trackPreview[2].position = PREVIEW_DOWN;
		break;
	case ott_const::UpRight:
		this->_trackPreview[0].position = PREVIEW_UPRIGHT;
		this->_trackPreview[2].position = PREVIEW_DOWNLEFT;
		break;
	case ott_const::Right:
		this->_trackPreview[0].position = PREVIEW_RIGHT;
		this->_trackPreview[2].position = PREVIEW_LEFT;
		break;
	case ott_const::DownRight:
		this->_trackPreview[0].position = PREVIEW_DOWNRIGHT;
		this->_trackPreview[2].position = PREVIEW_UPLEFT;
		break;
	case ott_const::Down:
		this->_trackPreview[0].position = PREVIEW_DOWN;
		this->_trackPreview[2].position = PREVIEW_UP;
		break;
	case ott_const::DownLeft:
		this->_trackPreview[0].position = PREVIEW_DOWNLEFT;
		this->_trackPreview[2].position = PREVIEW_UPRIGHT;
		break;
	case ott_const::Left:
		this->_trackPreview[0].position = PREVIEW_LEFT;
		this->_trackPreview[2].position = PREVIEW_RIGHT;
		break;
	case ott_const::UpLeft:
		this->_trackPreview[0].position = PREVIEW_UPLEFT;
		this->_trackPreview[2].position = PREVIEW_DOWNRIGHT;
		break;
	}
}
}
