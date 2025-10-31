#include <SFML/Graphics.hpp>
#include <optional>       
#include <vector>         
#include <random>         
#include <cmath>          
#include <iostream> 

// Структура для Кулі
struct Bullet
{
    sf::RectangleShape shape;   
    sf::Vector2f velocity;      
};


int main()
{
    // --- 1. НАЛАШТУВАННЯ ---
    const unsigned int WINDOW_WIDTH = 800u;
    const unsigned int WINDOW_HEIGHT = 600u;
    sf::RenderWindow window(sf::VideoMode( {WINDOW_WIDTH, WINDOW_HEIGHT} ), "Bullet Hell");
    window.setFramerateLimit(60); 

    // --- 2. НАЛАШТУВАННЯ ГРАВЦЯ ---
    sf::RectangleShape player( {30.f, 30.f} ); 
    player.setFillColor(sf::Color::Green);      
    player.setPosition( {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} ); 
    float playerSpeed = 5.0f; 

    // --- 3. НАЛАШТУВАННЯ ГРИ ---
    bool isGameOver = false; 
    float bulletSpeed = 4.0f; 
    std::vector<Bullet> bullets;

    // --- 4. НАЛАШТУВАННЯ РАНДОМУ ТА ЧАСУ ---
    std::random_device rd;
    std::mt19937 rng(rd()); 
    std::uniform_int_distribution<int> edgeDist(0, 3); 
    std::uniform_real_distribution<float> posXDist(0.f, WINDOW_WIDTH);
    std::uniform_real_distribution<float> posYDist(0.f, WINDOW_HEIGHT);

    sf::Clock spawnClock;
    sf::Time spawnInterval = sf::seconds(0.3f); 

    // Спрощений таймер (годинник для підрахунку часу гри)
    sf::Clock scoreClock; 

    // --- 5. ГОЛОВНИЙ ЦИКЛ ГРИ ---
    while (window.isOpen())
    {
        // --- ОБРОБКА ПОДІЙ ---
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // -----------------------------------------------------------
        // == ЛОГІКА ГРИ ==
        // -----------------------------------------------------------
        if (!isGameOver)
        {
            // === 5A. РУХ ГРАВЦЯ ===
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) player.move( {0.f, -playerSpeed} ); 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) player.move( {0.f, playerSpeed} ); 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) player.move( {-playerSpeed, 0.f} );
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) player.move( {playerSpeed, 0.f} ); 
            
            // === 5B. МЕЖІ ЕКРАНА ДЛЯ ГРАВЦЯ ===
            sf::Vector2f playerPos = player.getPosition();
            sf::Vector2f playerSize = player.getSize();
            if (playerPos.x < 0.f) playerPos.x = 0.f;
            if (playerPos.y < 0.f) playerPos.y = 0.f;
            if (playerPos.x + playerSize.x > WINDOW_WIDTH) playerPos.x = WINDOW_WIDTH - playerSize.x;
            if (playerPos.y + playerSize.y > WINDOW_HEIGHT) playerPos.y = WINDOW_HEIGHT - playerSize.y;
            player.setPosition(playerPos);

            // === 5C. СПАВН КУЛЬ ===
            if (spawnClock.getElapsedTime() > spawnInterval)
            {
                spawnClock.restart(); 
                Bullet newBullet;
                newBullet.shape.setSize( {10.f, 10.f} );
                newBullet.shape.setFillColor(sf::Color::Red);
                int edge = edgeDist(rng); 
                sf::Vector2f spawnPos;
                switch (edge)
                {
                    case 0: spawnPos = { posXDist(rng), -20.f }; break; // Спавн зверху
                    case 1: spawnPos = { WINDOW_WIDTH + 20.f, posYDist(rng) }; break; // Спавн справа
                    case 2: spawnPos = { posXDist(rng), WINDOW_HEIGHT + 20.f }; break; // Спавн знизу
                    case 3: spawnPos = { -20.f, posYDist(rng) }; break; // Спавн зліва
                }
                newBullet.shape.setPosition(spawnPos);
                sf::Vector2f direction = player.getPosition() - spawnPos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                direction = direction / length;
                newBullet.velocity = direction * bulletSpeed;
                bullets.push_back(newBullet); 
            }

            // === 5D. РУХ КУЛЬ ===
            for (auto& bullet : bullets)
            {
                bullet.shape.move(bullet.velocity);
            }

            // === 5E. ПЕРЕВІРКА ЗІТКНЕНЬ ===
            sf::FloatRect playerBounds = player.getGlobalBounds();
            for (const auto& bullet : bullets)
            {
                if (bullet.shape.getGlobalBounds().findIntersection(playerBounds))
                {
                    if (!isGameOver) // Перевіряємо, щоб вивести повідомлення лише один раз
                    {
                        isGameOver = true; 
                        player.setFillColor(sf::Color::Red); 

                        // Вивід в консоль
                        sf::Time finalTime = scoreClock.getElapsedTime();
                        std::cout << "=========================" << std::endl;
                        std::cout << "GAME OVER!" << std::endl;
                        std::cout << "Ви протрималися: " << finalTime.asSeconds() << " секунд." << std::endl;
                        std::cout << "=========================" << std::endl;
                    }
                }
            }
        }
        // Кінець блоку `if (!isGameOver)`


        // -----------------------------------------------------------
        // == МАЛЮВАННЯ (РЕНДЕРИНГ) ==
        // -----------------------------------------------------------
        window.clear(sf::Color(50, 50, 50)); 
        window.draw(player);
        for (const auto& bullet : bullets)
        {
            window.draw(bullet.shape);
        }
        window.display();
    }

    return 0;
}

// Допоможи зарефакторити код покроково, з поясненнями щоб я зміг поступово змінювати код і розуміти як цей код покращується