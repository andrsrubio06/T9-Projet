/**
 *  IN-204 Project T9 clavier
 *  main.cpp
 *  Purpose: Principal function. Run the program
 *
 *  @author Marcelo
 *  @email andres-eloy.rubio-carvajal@ensta-paris.fr
 * 	@author 
 * 	@email @ensta-paris.fr 
 */


#include <iostream>
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>

#include "Button.hpp"


sf::Text createButtonLabel(sf::Font& font, const std::string character,
							sf::Vector2f buttonSize, sf::Vector2f position) {
    sf::Text newButton;
    newButton.setFont(font);
    newButton.setCharacterSize( buttonSize.y/2);
    newButton.setStyle(sf::Text::Bold);
    newButton.setString(character);
    newButton.setPosition(position);
	newButton.setOutlineThickness(buttonSize.x /50);
    newButton.setOutlineColor(sf::Color::White);
    newButton.setFillColor(sf::Color::Blue);

    return newButton;
}

sf::RectangleShape crateButtonShape (const sf::Vector2f buttonSize, sf::Vector2f position, const bool& outline){
	
	sf::RectangleShape newButton;
	newButton.setSize(buttonSize);
	newButton.setPosition(position);
	newButton.setOutlineThickness(3);
	newButton.setOutlineColor(sf::Color::White);
	newButton.setFillColor(sf::Color::Transparent);
	
	return newButton;
}



int main(){

    sf::RenderWindow window(sf::VideoMode(360,640), "SFML work!");

    while(window.isOpen()){

        sf::Event event;
    
        sf::Font font;
        font.loadFromFile("fonts/Amatic-Bold.ttf");

		sf::Vector2f sizeObjects=sf::Vector2f(100,70);
		
		//sf::Vector2f position[12];
		sf::Vector2f initPos=sf::Vector2f(30,300);
		std::list<sf::RectangleShape> buttonsShape;	

		for (int i = 0; i < 12; i++)
			//position[i]=sf::Vector2f(initPos.x+sizeObjects.x*(i%3),initPos.y+sizeObjects.y*(i/3));
			buttonsShape.push_back(crateButtonShape(sizeObjects, sf::Vector2f(initPos.x+sizeObjects.x*(i%3),
													initPos.y+sizeObjects.y*(i/3)), true));	
		
		
		//sf::Vector2f positionLabel1[12];
		std::list<sf::Text> buttonsLabel1;	
		for (int i = 0; i < 12; i++)
			buttonsLabel1.push_back( createButtonLabel(font,std::to_string(1+i), sizeObjects,
						sf::Vector2f(initPos.x+sizeObjects.x*(i%3)+10,initPos.y+sizeObjects.y*(i/3))));
		
		std::list<sf::Text> buttonsLabel1;	
		for (int i = 0; i < 12; i++)
			buttonsLabel1.push_back( createButtonLabel(font,std::to_string(1+i), sizeObjects,
						sf::Vector2f(initPos.x+sizeObjects.x*(i%3)+10,initPos.y+sizeObjects.y*(i/3))));	

            while (window.pollEvent(event))
            {
                switch ( event.type ){      //SWITCH
			        case sf::Event::Closed:
			        	window.close( );
			        	break;
			        case sf::Event::MouseButtonPressed:
			        	int i=0;
						for(sf::RectangleShape button : buttonsShape)
						{
					
							if ((sf::Mouse::getPosition(window).x - button.getGlobalBounds().left >= 0) &&
        						(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top >= 0) &&
        						(sf::Mouse::getPosition(window).x - button.getGlobalBounds().left <= button.getGlobalBounds().width) &&
        						(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top <= button.getGlobalBounds().height))
								std::cout << "You clicked the button "<< i+1 << std::endl;
							i++;
						}
					break;
						
				}
            
			}
        window.clear();
		for(sf::RectangleShape buttonShape : buttonsShape)
			window.draw(buttonShape);
		for(sf::Text buttonLabel : buttonsLabel1)
			window.draw(buttonLabel);

        window.display();
	}
    return EXIT_SUCCESS;
	
}
	