#include <thread>
#include <sstream>
#include <iomanip>
#include "simulation.hpp"



int simulation(sf::RenderWindow& window, Parcours& parcours, std::vector<Coureur>& coureurs) {
    window.clear(sf::Color(0, 48, 73));
    
    /*************************************************
       Use threads to display the different modules
    *************************************************/
    /* MODULES:
    *      1 - Top Coureurs Display (top left)
    *      2 - Live Parcours (and top 5 coureurs ?) (middle left)
    *      3 - Stopwatch (top right)
    *      4 - Top Speeds Display (bottom right)
    */

    /*******************
       INITIALISATION
    *******************/
    sf::Font firaCode;
    if(!firaCode.loadFromFile("font/FiraCode.ttf"))
        return 1;
    
    //1
    //2
    //3-Stopwatch 
    sf::Time realTime = sf::seconds(0.f);
    sf::Text timeDisplay("0:0", firaCode, 40);
    timeDisplay.setPosition(window.getSize().x - 210.f, 0.f);
    timeDisplay.setFillColor(sf::Color(214, 40, 40));
    //4
    
    /*
     UPDATE
    */
    sf::Clock stopwatch;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    return 2;
                    break;
                default:
                    break;
            }
        }


        //1
        //2
        //3-Stopwatch
        realTime += stopwatch.restart();
        //Update the time display
        std::thread thread3(updateTime, std::ref(realTime), std::ref(timeDisplay));

        //4

        //Wait for the threads to end
        thread3.join();
        
        /***************
         DISPLAY UPDATE
        ***************/
        window.clear(sf::Color(0, 48, 73));

        window.draw (timeDisplay);

        window.display();
    
    }

    return 0;
}


void updateTime(sf::Time& time, sf::Text& txt) {
    int timeInSeconds = time.asSeconds();
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (int)timeInSeconds/3600 << ':' << std::setw(2) << (timeInSeconds/60)%3600 << ':' << std::setw(2) << timeInSeconds%60;
    txt.setString(ss.str());
}

void centerText(sf::Text& txt, sf::RenderWindow& window) {
    sf::FloatRect textRect = txt.getLocalBounds();
    txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    txt.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f);
}
