#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Button
{
private:
    //sf::Vector2f size;
    //sf::Vector2f pos;
    //char character;
    //sf::Font font;
    //float sizeChar;
    sf::RectangleShape buttonShape;
    sf::Text buttonLabel;
public:
    Button(sf::Vector2f size, sf::Vector2f pos,std::string character, sf::Font font);
    void drawing(sf::RenderTarget& target) const;
    sf::RectangleShape getButtonShape() const;
    sf::Text getButtonLabel() const;
    void mouseOver(sf::RenderTarget& target);

    ~Button();
};

Button::Button(sf::Vector2f size, sf::Vector2f pos, std::string character, sf::Font font)
{
        
        buttonShape.setSize(size);
        buttonShape.setFillColor(sf::Color::Transparent);
        buttonShape.setOutlineColor(sf::Color::White);
        buttonShape.setPosition(pos);
        buttonShape.setOutlineThickness(3);

              
        buttonLabel.setFont(font);
        buttonLabel.setCharacterSize(30);
        buttonLabel.setStyle(sf::Text::Bold);
        buttonLabel.setString(character);  
        buttonLabel.setFillColor(sf::Color::White);
        buttonLabel.setOutlineThickness(1);
        buttonLabel.setOutlineColor(sf::Color::Red);
        buttonLabel.setPosition(pos);
}

void Button::drawing(sf::RenderTarget& target) const {
    target.draw(buttonShape);
  //  target.draw(buttonLabel, sf::RenderStates::Default);
}

void mouseOver(sf::RenderTarget& target){

    

}

sf::RectangleShape Button::getButtonShape() const {
    return buttonShape;
}

sf::Text Button::getButtonLabel() const {
    return buttonLabel;
}

Button::~Button()
{
}
