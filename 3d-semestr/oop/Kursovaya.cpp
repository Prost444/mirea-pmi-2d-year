#include <cmath>
#include <ctime>
#include "SFML/Graphics.hpp"

// Константы для определения параметров игры
const int windowWidth = 800;
const int windowHeight = 600;
const float puckRadius = 15.0f;
const float sliderWidth = 15.0f;
const float sliderHeight = 100.0f;
const float puckSpeed = 200.0f; // Скорость шайбы

// Функция для обновления состояния игры
void update(sf::RectangleShape& slider, sf::CircleShape& puck, float deltaTime) {
    // Здеь будет код для обновления позиций слайдеров и шайбы
    // Например, простейшее движение шайбы вдоль оси X:
    sf::Vector2f movement(0.0f, puckSpeed * deltaTime);
    puck.move(movement);
}

int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Air Hockey");

    // Инициализация первого слайдера (агента)
    sf::RectangleShape slider1(sf::Vector2f(sliderWidth, sliderHeight));
    slider1.setPosition(30, windowHeight / 2 - sliderHeight / 2);
    slider1.setFillColor(sf::Color::Red);

    // Инициализация второго слайдера (противника)
    sf::RectangleShape slider2(sf::Vector2f(sliderWidth, sliderHeight));
    slider2.setPosition(windowWidth - 30 - sliderWidth, windowHeight / 2 - sliderHeight / 2);
    slider2.setFillColor(sf::Color::Blue);

    // Инициализация шайбы
    sf::CircleShape puck(puckRadius);
    puck.setPosition(windowWidth / 2, windowHeight / 2);
    puck.setFillColor(sf::Color::White);

    // Часы для управления временем
    sf::Clock clock;

    // Главный цикл игры
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Вычисление прошедшего времени
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();

        // Обновление игры
        update(slider1, puck, deltaTime);

        // Очистка экрана
        window.clear();

        // Рисование объектов
        window.draw(slider1);
        window.draw(slider2);
        window.draw(puck);

        // Отображение кадра в окне
        window.display();
    }

    return 0;
}
