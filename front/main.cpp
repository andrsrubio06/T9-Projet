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
							 int buttonSize, sf::Vector2f position) {
    sf::Text newButton;
    newButton.setFont(font);
    newButton.setCharacterSize( buttonSize/4);
    newButton.setStyle(sf::Text::Bold);
    newButton.setString(character);
    newButton.setPosition(position);
	newButton.setOutlineThickness(buttonSize/50);
    newButton.setOutlineColor(sf::Color::White);
    newButton.setFillColor(sf::Color::Blue);

    return newButton;
}
//create a rectangle
sf::RectangleShape crateButtonShape (const sf::Vector2f buttonSize, sf::Vector2f position, const bool& outline){
	
	sf::RectangleShape newButton;
	newButton.setSize(buttonSize);
	newButton.setPosition(position);
	newButton.setOutlineThickness(3);
	newButton.setOutlineColor(sf::Color::White);
	newButton.setFillColor(sf::Color::Transparent);
	
	return newButton;
}

bool overButton(sf::RenderWindow& window, sf::RectangleShape button){
		
		if ((sf::Mouse::getPosition(window).x - button.getGlobalBounds().left >= 0) &&
        	(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top >= 0) &&
        	(sf::Mouse::getPosition(window).x - button.getGlobalBounds().left <= button.getGlobalBounds().width) &&
        	(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top <= button.getGlobalBounds().height)){
			return 	true;
			}
		return false;

}


int main(){

    sf::RenderWindow window(sf::VideoMode(360,640), "SFML work!");


	sf::Event event;
    
        sf::Font font;
        font.loadFromFile("fonts/Amatic-Bold.ttf");

		sf::Vector2f sizeObjects=sf::Vector2f(100,70);
		
		int totalButtons=15;
		//sf::Vector2f position[12];
		sf::Vector2f initPos=sf::Vector2f(30,250);
		std::vector<sf::RectangleShape> buttonsShape;

		for (int i = 0; i < totalButtons; i++)
			//position[i]=sf::Vector2f(initPos.x+sizeObjects.x*(i%3),initPos.y+sizeObjects.y*(i/3));
			buttonsShape.push_back(crateButtonShape(sizeObjects, sf::Vector2f(initPos.x+sizeObjects.x*(i%3),
													initPos.y+sizeObjects.y*(i/3)), true));	
		
		
		//sf::Vector2f positionLabel1[12];
		std::vector<sf::Text> buttonsLabel1;	
		for (int i = 0; i < totalButtons; i++)
			buttonsLabel1.push_back( createButtonLabel(font,std::to_string(1+i), sizeObjects.x,
						sf::Vector2f(initPos.x+sizeObjects.x*(i%3)+sizeObjects.x*0.45,initPos.y+sizeObjects.y*(i/3)+sizeObjects.y*0.1)));
		
		std::vector<sf::Text> buttonsLabel2;	
		for (int i = 0; i < totalButtons; i++)
			buttonsLabel2.push_back( createButtonLabel(font,std::to_string(1+i), sizeObjects.x/2,
						sf::Vector2f(initPos.x+sizeObjects.x*(i%3)+sizeObjects.x*0.3,initPos.y+sizeObjects.y*(i/3)+sizeObjects.y*0.5)));



		//fenetre


		int auxiliar=0;

    while(window.isOpen()){

        	
	

            while (window.pollEvent(event))
            {
                switch ( event.type ){      //SWITCH
			        case sf::Event::Closed:
			        	window.close( );
			        	break;
			        case sf::Event::MouseButtonPressed:
					{
			        	int identifier=0;

						

						if(event.key.code== sf::Mouse::Left){

							////counter of time********
			    		    for(sf::RectangleShape button : buttonsShape)
							{
							bool over = overButton(window,button);
							if (over)
							auxiliar=identifier;
							identifier++;
							//Connecter avec l'algorithm du ecriture
							}
			    		    buttonsLabel1[auxiliar].setOutlineColor(sf::Color::Green);
			    		}

			    	break;
					}
			    	case sf::Event::MouseButtonReleased:
					{
			    		if(event.key.code==sf::Mouse::Left){
							buttonsLabel1[auxiliar].setOutlineColor(sf::Color::White);
						}
			    		    		///ecriture blablal, function

			    	break;
					}


					
						//bool overButton = false;
						//for(sf::RectangleShape button : buttonsShape){
						//	if ((sf::Mouse::getPosition(window).x - button.getGlobalBounds().left >= 0) &&
        				//			(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top >= 0) &&
        				//			(sf::Mouse::getPosition(window).x - button.getGlobalBounds().left <= button.getGlobalBounds().width) &&
        				//			(sf::Mouse::getPosition(window).y - button.getGlobalBounds().top <= button.getGlobalBounds().height)){
						//			overButton = true;
						//			break;
						//			}
						//	identifier++;
						//}
						//if (overButton){
						//	int i=0;
						//	for(sf::Text button : buttonsLabel1){
						//		
						//		if (i==identifier)
						//		{
						//		button.setOutlineColor(sf::Color::Green);
						//		}
						//	}
						//
						//}
				}
			}
        window.clear();
		for(sf::RectangleShape buttonShape : buttonsShape)
			window.draw(buttonShape);
		for(sf::Text buttonLabel : buttonsLabel1){
			if(buttonLabel.getString()=="1"){
				buttonLabel.setString("C");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			else if(buttonLabel.getString()=="2"){	
				buttonLabel.setString("<-");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			else if(buttonLabel.getString()=="3"){	
				buttonLabel.setString("->");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			else if(buttonLabel.getString()=="13"){	
				buttonLabel.setString("*");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			else if(buttonLabel.getString()=="14"){	
				buttonLabel.setString("0");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			else if(buttonLabel.getString()=="15"){	
				buttonLabel.setString("#");
				sf::Vector2f temporal= buttonLabel.getPosition();
				buttonLabel.setPosition(temporal.x,temporal.y+sizeObjects.y*0.2);
			}
			window.draw(buttonLabel);
		}
        window.display();
	}
    return EXIT_SUCCESS;
	
}
	