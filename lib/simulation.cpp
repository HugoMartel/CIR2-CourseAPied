#include <thread>
#include <sstream>
#include <iomanip>
#include "simulation.hpp"
#include <iostream>


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
    
    //1-Top Coureurs
    // Display the five first runners for the init
    size_t coureurAmountToDisplay = (coureurs.size() < 5)? coureurs.size() : 5;
    sf::RectangleShape coureurDistRectangles[5];
    sf::Text coureurNames[5];
    
    for (size_t i = 0; i < coureurAmountToDisplay; ++i ) {
        coureurNames[i].setFont(firaCode);
        coureurNames[i].setCharacterSize(20);
        coureurNames[i].setString(coureurs[i].getName() + ": " + std::to_string(coureurs[i].getDistanceRan()));
        //DEBUG
        /*
        for (auto it = coureurNames[i].getString().begin(); it != coureurNames[i].getString().end(); ++it)
            std::cout << (char)*it;
        std::cout << "\n";
        */
        coureurNames[i].setPosition(20, 20 + 50*i);
        //The first coureur's name is brighter
        if (!i)
            coureurNames[i].setFillColor(sf::Color(252, 191, 73));
        else 
            coureurNames[i].setFillColor(sf::Color(247, 127, 0));

        // Get the coureur's distance to the end to display a progress bar
        coureurDistRectangles[i].setPosition(coureurNames[i].getGlobalBounds().width + 20, coureurNames[i].getGlobalBounds().height + 50*i + 20);
        coureurDistRectangles[i].setSize(sf::Vector2f(coureurs[i].getDistanceRan()  / parcours.getTotalDistance() * 500 + 100, 30));

        window.draw(coureurNames[i]);
        window.draw(coureurDistRectangles[i]);
    }
    
    
    //2
    //3-Stopwatch 
    sf::Time realTime = sf::seconds(0.f);
    sf::Text timeDisplay("00:00:00", firaCode, 40);
    timeDisplay.setPosition(window.getSize().x - 210.f, 0.f);
    timeDisplay.setFillColor(sf::Color(214, 40, 40));
    window.draw (timeDisplay);//Set the init display
    //4
    

    
    window.display();//show the init display
    //-----------------------------------------------------------------------------------

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


        //1-TOP Coureurs



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
        //1
        for (size_t i = 0; i < coureurAmountToDisplay; ++i) {
            window.draw(coureurNames[i]);
            window.draw(coureurDistRectangles[i]);
        }
        //2
        //3
        window.draw (timeDisplay);

        window.display();
    
    }

    return 0;
}


void updateTime(sf::Time& time, sf::Text& txt) {
    int timeInSeconds = time.asSeconds();
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (int)timeInSeconds/3600 << ':' << std::setw(2) << (timeInSeconds/60)%60 << ':' << std::setw(2) << timeInSeconds%60;
    txt.setString(ss.str());
}

void centerText(sf::Text& txt, sf::RenderWindow& window) {
    sf::FloatRect textRect = txt.getLocalBounds();
    txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    txt.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f);
}
