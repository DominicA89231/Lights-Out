#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Light : public sf::Drawable
{
    sf::RectangleShape light;
    bool state = true;
    
public:
    int row, col;
    bool pressed = false;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(light);
    }
    Light(int width, int height, int posX, int posY, int _row, int _col)
    {
        light.setSize(sf::Vector2f(width, height));
        light.setPosition(sf::Vector2f(posX, posY));
        light.setFillColor(sf::Color(230, 230, 230, 255));
        row = _row, col = _col;
    }
    bool inBounds(sf::Vector2f position)
    {
        return position.x >= light.getPosition().x && position.x <= light.getPosition().x + light.getSize().x
            && position.y >= light.getPosition().y && position.y <= light.getPosition().y + light.getSize().y;
    }
    void switchState()
    {
        if (state)
        {
            light.setFillColor(sf::Color(130, 130, 130, 255));
            state = false;
        }
        else
        {
            light.setFillColor(sf::Color(230, 230, 230, 255));
            state = true;
        }
    }
    void highlight()
    {
        light.setFillColor(sf::Color::Red);
    }
    void reset()
    {
        light.setFillColor(sf::Color(230, 230, 230, 255));
        state = true;
        pressed = false;
    }
};

int main()
{ 
    int game_width = 7;
    int game_length = 3;
    int light_width = 100;
    int light_length = 100;
    sf::RenderWindow window(sf::VideoMode(game_width * light_width + (game_width + 1) * 5, game_length * light_length + (game_length + 1) * 5), "SFML works!");
    std::vector<Light*> lights;
    for (int i = 0; i < game_width; i++)
    {
        for (int j = 0; j < game_length; j++)
        {
            Light* light = new Light(light_width, light_length, light_width * i + 5 * (i + 1), light_length * j + 5 * (j + 1), i, j);
            lights.push_back(light);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    for (Light* light : lights)
                    {
                        if (light->inBounds(mouse_position))
                        {
                            int i = light->row, j = light->col;
                            lights[i * game_length + j]->switchState();
                            lights[i * game_length + j]->pressed = !lights[i * game_length + j]->pressed;
                            if (i > 0)
                                lights[(i - 1) * game_length + j]->switchState();
                            if (i < game_width - 1)
                                lights[(i + 1) * game_length + j]->switchState();
                            if (j > 0)
                                lights[i * game_length + j - 1]->switchState();
                            if (j < game_length - 1)
                                lights[i * game_length + j + 1]->switchState();
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F)
                {
                    for (Light* light : lights)
                    {
                        if (light->pressed)
                        {
                            light->highlight();
                            break;
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    int count = 0, idx = 0;
                    while (idx < lights.size() && count < lights.size() * 2 / 5)
                    {
                        if (rand() % 3 == 0)
                        {
                            int i = lights[idx]->row, j = lights[idx]->col;
                            lights[i * game_length + j]->switchState();
                            lights[i * game_length + j]->pressed = !lights[i * game_length + j]->pressed;
                            if (i > 0)
                                lights[(i - 1) * game_length + j]->switchState();
                            if (i < game_width - 1)
                                lights[(i + 1) * game_length + j]->switchState();
                            if (j > 0)
                                lights[i * game_length + j - 1]->switchState();
                            if (j < game_length - 1)
                                lights[i * game_length + j + 1]->switchState();
                            count++;
                        }
                        idx++;
                    }
                }
            }
        }
        
        window.clear(sf::Color(180, 180, 180, 255));
        for (Light* light : lights)
            window.draw(*light);
        window.display();
    }

    return 0;
}