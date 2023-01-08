#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const float min_distance = .5f;
const float dist_length = 5;

struct Ball
{
    float radius;
    float charge;
    sf::Vector2f position;
    sf::Vector2f velocity;
};


int main()
{
    srand(time(0));

    const int width = 1000;
    const int height = 800;
    int n_balls = 0;

    char sign = 1;

    const float delta_t = .1f;
    sf::RenderWindow window(sf::VideoMode(width, height), "My window");
    window.setFramerateLimit(60);
    sf::CircleShape circle(50.0f);

    std::vector<Ball> balls;
    balls.resize(n_balls);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::LShift) {
                    sign = -1;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LShift) {
                    sign = 1;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                Ball b;
                if (event.mouseButton.button == sf::Mouse::Right)
                    b.radius = 30;
                else if (event.mouseButton.button == sf::Mouse::Left)
                    b.radius = 7;
                b.charge = sign * b.radius * b.radius;
                b.position = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                b.velocity = { .0f, .0f };
                balls.push_back(b);
                ++n_balls;
            }
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < n_balls; i++)
        {
            balls[i].position += balls[i].velocity * delta_t;
            for (int j = 0; j < n_balls; ++j) {
                if (i == j)
                    continue;
                float distance = std::sqrt((balls[j].position.x - balls[i].position.x) 
                    * (balls[j].position.x - balls[i].position.x) 
                    + (balls[j].position.y - balls[i].position.y) 
                    * (balls[j].position.y - balls[i].position.y));
                if (distance < min_distance)
                    continue;
                sf::Vector2f direction = balls[j].position - balls[i].position;
                if (balls[i].charge * balls[j].charge > 0)
                    balls[i].velocity -= (direction * dist_length * delta_t * std::abs(balls[j].charge)) 
                    / (distance * distance);
                else
                    balls[i].velocity += (direction * dist_length * delta_t * std::abs(balls[j].charge)) 
                    / (distance * distance);
            }

            if (balls[i].position.x - balls[i].radius < 0 
                || balls[i].position.x + balls[i].radius > width) {
                balls[i].velocity.x *= -1;
                balls[i].position.x = balls[i].position.x - balls[i].radius < 0 ? 
                    balls[i].radius : width - balls[i].radius;
            }
            if (balls[i].position.y - balls[i].radius < 0 
                || balls[i].position.y + balls[i].radius > height) {
                balls[i].velocity.y *= -1;
                balls[i].position.y = balls[i].position.y - balls[i].radius < 0 ?
                    balls[i].radius : height - balls[i].radius;
            }

            circle.setRadius(balls[i].radius);
            circle.setOrigin(balls[i].radius, balls[i].radius);
            circle.setPosition(balls[i].position);

            if (balls[i].charge > 0)
                circle.setFillColor({ 255, 0, 0 });
            else
                circle.setFillColor({ 0, 0, 255 });
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}