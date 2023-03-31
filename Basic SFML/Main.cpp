#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <numbers>

int main()
{
	// we need to make AI 
	// angular reflection
	// colliders on top and the bottom
	// main menu 
	// score for player  

	sf::RenderWindow window(sf::VideoMode(800, 600), "Ping Pong Game");
	sf::Event event; 
	sf::Font font;
	sf::Texture ballTexture;
	sf::RectangleShape player(sf::Vector2f(20,100)); 
	sf::RectangleShape enemy(sf::Vector2f(20, 100));
	sf::Text scoreText("Score: ", font, 52);
	sf::SoundBuffer hitSoundBuffer; 
	sf::SoundBuffer gameOverSoundBuffer; 
	sf::Image iconWindow; 
	//window.setVerticalSyncEnabled(true);

	float playerSpeed = 750.0f;
	float ballSpeed = 1000.0f;
	float startAgainTime = 3.0;
	int score = 0; 
	int opacity = 1; 
	int health = 3; 
	bool enemyDirection = false; 
	auto angle(1.5 * std::numbers::pi_v<float>);
	bool isRoundEnded = false; 

	if (!font.loadFromFile("Fixedsys.ttf"))
	{
		return EXIT_FAILURE;
	}

	if (!ballTexture.loadFromFile("ball.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite ballSprite(ballTexture);

	if (!iconWindow.loadFromFile("iconWindow.png"))
	{
		return EXIT_FAILURE;
	}
	window.setIcon(512, 512, iconWindow.getPixelsPtr());

	if (!hitSoundBuffer.loadFromFile("hit.ogg"))
	{
		return EXIT_FAILURE;
	}
	sf::Sound hitSound(hitSoundBuffer);
	
	if (!gameOverSoundBuffer.loadFromFile("gameOver.ogg"))
	{
		return EXIT_FAILURE;
	}
	sf::Sound gameOverSound(gameOverSoundBuffer);

	const double dt = 0.01;
	
	sf::Clock timer;
	double accumulator = 0.0f;

	player.setPosition(0, 250);
	enemy.setPosition(780, 250);
	ballSprite.setPosition((window.getSize().x - ballSprite.getGlobalBounds().width) / 2, (window.getSize().y - ballSprite.getGlobalBounds().height) / 2);
	scoreText.setPosition((window.getSize().x - scoreText.getGlobalBounds().width) / 2, 0);

	while (window.isOpen())
	{
		double frameTime = timer.restart().asSeconds();

		accumulator += frameTime;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}
		while (accumulator >= dt)
		{
			accumulator -= dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				player.move(0.f, -1.f * dt * playerSpeed);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				player.move(0.f, 1.f * dt * playerSpeed);
			}

			enemy.setPosition(sf::Vector2f(780.0f, player.getPosition().y));

			if (!isRoundEnded || health >= 0)
			{
				ballSprite.move(ballSpeed * dt * std::sin(angle), ballSpeed * dt * std::cos(angle));
				if (player.getGlobalBounds().intersects(ballSprite.getGlobalBounds()))
				{
					hitSound.play(); 
					auto position(player.getPosition().y - ballSprite.getPosition().y);
					if (position < 50 && position > -50) {
						angle -= std::numbers::pi_v<float>;
					}
					if (position >= 50) {
						angle -= std::numbers::pi_v<float>+(position - 50) / 100 * std::numbers::pi_v<float>;
					}
					if (position <= -50) {
						angle -= std::numbers::pi_v<float> +(position + 50) / 100 * std::numbers::pi_v<float>;
					}
					ballSprite.move(ballSpeed * dt, 0.0f);
				}

				if (enemy.getGlobalBounds().intersects(ballSprite.getGlobalBounds()))
				{
					hitSound.play();
					auto position(player.getPosition().y - ballSprite.getPosition().y);
					if (position < 50 && position > -50) {
						angle += std::numbers::pi_v<float>;
					}
					if (position >= 50) {
						angle += std::numbers::pi_v<float>+(position - 50) / 100 * std::numbers::pi_v<float>;
					}
					if (position <= -50) {
						angle += std::numbers::pi_v<float> +(position + 50) / 100 * std::numbers::pi_v<float>;
					}
					ballSprite.move(-ballSpeed * dt, 0.0f);
				}

				if (ballSprite.getGlobalBounds().left < 0) {
					gameOverSound.play();
					health -= 1;
					ballSprite.setPosition((window.getSize().x - ballSprite.getGlobalBounds().width) / 2, (window.getSize().y - ballSprite.getGlobalBounds().height) / 2);
					angle = 1.5 * std::numbers::pi_v<float>;
					startAgainTime = 3.0f;
					isRoundEnded = true;
				}
				if ((ballSprite.getGlobalBounds().left + ballSprite.getGlobalBounds().width) > 800) {
					hitSound.play();
					score += 1;
					scoreText.setString("Score: " + std::to_string(score));
					ballSprite.setPosition((window.getSize().x - ballSprite.getGlobalBounds().width) / 2, (window.getSize().y - ballSprite.getGlobalBounds().height) / 2);
					angle = 0.5 * std::numbers::pi_v<float>;
					startAgainTime = 3.0f; 
					isRoundEnded = true;
				}
				
			}
			else {
				startAgainTime -= dt; 
				if (startAgainTime <= 0)
				{
					isRoundEnded = false; 
				}
			}
		}

		if (player.getPosition().y < 0) {
			player.setPosition(0.f, 0.f);
		}
		if (player.getPosition().y > 500) {
			player.setPosition(0.f, 500.f);
		}

		window.clear(sf::Color(0,0,0,255));
		
		window.draw(scoreText);
		window.draw(player);
		window.draw(enemy);
		window.draw(ballSprite);

		window.display(); 
	}
}