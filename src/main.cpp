#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../lib/coureur.hpp"
#include "../lib/parcours.hpp"
#include "../lib/simulation.hpp"
#include <iostream>
#include <thread>
#include <cstring>


//#define COUREURS_FILE "ressources/coureurListTest.txt"
#define COUREURS_FILE "ressources/coureurList.txt"

//#define PARCOURS_FILE "ressources/marathon.txt"
//#define PARCOURS_FILE "ressources/parcoursTest.txt"
#define PARCOURS_FILE "ressources/star.txt"

int main(int argc, char* argv[]) {

    bool debug = false;
    if (!std::strcmp(argv[argc-1] , "--debug"))
        debug = true;

    /*
    * LOAD FILES
    * Parcours & Coureur
    */
    std::vector<Coureur> test;
    if (loadCoureurFromFile(COUREURS_FILE, test)) {
        std::cerr << "-- ERROR LOADING THE FILE --\n";
        return EXIT_FAILURE;
    }
    
    

    Parcours p(PARCOURS_FILE);
    if (debug) {
        p.printWind();
        p.printCheckpoints();
        p.printDistances();
        std::cout << "total: " << p.getTotalDistance() << std::endl;
        p.printSlopes();
        p.printAngles();
    }
    
    /****************************
                INIT
    ****************************/

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1080, 720), "Simulation Course", sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    sf::Image icon;
    if (!icon.loadFromFile("img/Sun.png"))
        return EXIT_FAILURE;
    window.setIcon (icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(150);
 
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("img/Sun.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
  
 
    // Load a font to use
    sf::Font firaCode;
    if (!firaCode.loadFromFile("font/FiraCode.ttf"))
        return EXIT_FAILURE;
    
    // Setup the Main Menu Texts
    sf::Text playText("Play", firaCode, 60);
    sf::Text settingsText("Settings", firaCode, 60);
    playText.setFillColor(sf::Color(247, 127, 0));
    settingsText.setFillColor(sf::Color(214, 40, 40));
    centerText(playText, window);
    centerText(settingsText, window);
    playText.setPosition(playText.getPosition().x, playText.getPosition().y - 50);
    settingsText.setPosition(settingsText.getPosition().x, settingsText.getPosition().y + 50);

    // Option selector
    sf::CircleShape selector(20.f, 3);
    selector.setPosition(playText.getPosition() - sf::Vector2f(100.f, 25.f)); 
    selector.setFillColor(sf::Color::Transparent);
    selector.setOutlineThickness(5.f);
    selector.setOutlineColor(sf::Color(247, 127, 0));
    //selector.setOutlineColor(sf::Color(252, 191, 73));
    selector.setRotation(90.f);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("music/mii-channel-music.ogg"))
        return EXIT_FAILURE;
    music.setVolume(50.f);
    music.setLoop(true);
    /******************
       COLOR PALETTE
    ******************/
    /*
    +--------------------+--------+---------------+
    | Prussian Blue      | 003049 |   0,  48,  73 |
    | Maximum Red        | D62828 | 214,  40,  40 |
    | Orange             | F77F00 | 247, 127,   0 |
    | Maximum Yellow Red | FCBF49 | 252, 191,  73 |
    | Lemon Meringue     | EAE2B7 | 234, 226, 183 |
    +--------------------+--------+---------------+
    */

    /****************************
             START MENU
    ****************************/

    // Play the music
    music.play();
    
    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            // Close window: exit
                case sf::Event::Closed:
                    music.stop();
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    // Arrows and Enter/Space
                    if (event.key.code == sf::Keyboard::Escape) {
                        music.stop();
                        window.close();
                        break;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        // Down case 
                        selector.setPosition(settingsText.getPosition() - sf::Vector2f(170.f, 25.f)); 
                        settingsText.setFillColor(sf::Color(247, 127, 0));
                        playText.setFillColor(sf::Color(214, 40, 40));
                    } else if (event.key.code == sf::Keyboard::Up) {
                        // Up case
                        selector.setPosition(playText.getPosition() - sf::Vector2f(100.f, 25.f));
                        playText.setFillColor(sf::Color(247, 127, 0));
                        settingsText.setFillColor(sf::Color(214, 40, 40));
                    } else if (event.key.code == sf::Keyboard::Enter or event.key.code == sf::Keyboard::Space) {
                        // Enter or Space to select an option
                        // Check the text's color to get the selected option 
                        if (settingsText.getFillColor() != sf::Color(247, 127, 0)) {
                            /*************
                                 PLAY
                            *************/
                            std::cout << "PLAY\n";
                        switch (simulation(window, p, test)) {
                                case -1:
                                    std::cerr << "BROKE OUT OF THE WHILE????\n";
                                    return EXIT_FAILURE;
                                    break;
                                case 0:
                                    std::cerr << "Window closed during a simulation...\n";
                                    return EXIT_SUCCESS;
                                case 1:
                                    std::cerr << "ERROR LOADING FONT\n";
                                    return EXIT_FAILURE;
                                case 2:
                                    std::cout << "BACK TO THE MAIN MENU\n";
                                    break;
                                default:
                                    break;
                            }
                        } else {
                            // Settings Case
                            /*************
                                 TODO
                            *************/
                            std::cout << "SETTINGS\n";
                        }
                    } 
                    break;
                default:
                    break;
            }

        }
        // Clear screen
        window.clear(sf::Color(0, 48, 73));

        // Draw objects
        window.draw(sprite);
        window.draw(playText);
        window.draw(settingsText);
        window.draw(selector);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
