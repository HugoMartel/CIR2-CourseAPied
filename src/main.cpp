#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../lib/coureur.hpp"
#include "../lib/parcours.hpp"
#include "../lib/simulation.hpp"
#include <iostream>
#include <thread>


int main()
{
    /*
    * DEBUGS
    * file loader tests
    */
    std::vector<Coureur> test;
    if (loadCoureurFromFile("ressources/coureurListTest.txt", test)) {
        std::cout << "-- ERROR LOADING THE FILE --" << std::endl;
        return EXIT_FAILURE;
    }

    /****************************
                INIT
    ****************************/

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1080, 720), "Simulation Course");
    
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("img/laikaLOGO.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
   
    //Time to use for updating the clock at every loop
    sf::Time realTime = sf::seconds(0.0f);

    // Create a graphical text to display the Time
    sf::Font firaCode;
    if (!firaCode.loadFromFile("font/FiraCode.ttf"))
        return EXIT_FAILURE;
    sf::Text timeDisplay("0:0", firaCode, 40);
    timeDisplay.setPosition(window.getSize().x - 150.f, 0.f);
    timeDisplay.setFillColor(sf::Color::Red);
 
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("music/mii-channel-music.ogg"))
        return EXIT_FAILURE;
    

    /****************************
               START
    ****************************/

    // Play the music
    music.play();
    
    //Start the clock 
    sf::Clock chrono;
    std::string timeWithMinutes;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear(sf::Color::Black);

        //Update the timer
        realTime += chrono.restart();
        timeWithMinutes = std::to_string((int) (realTime.asSeconds() / 60) ) + ":" + std::to_string((int)realTime.asSeconds() % 60);
        timeDisplay.setString(timeWithMinutes);
        //TODO: Better printing with separated mins/sec etc, use a thread to do it ?

        // Draw the sprite
        window.draw(sprite);
        // Draw the realTime
        window.draw(timeDisplay);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
