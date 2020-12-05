#include <thread>
#include <sstream>
#include <iomanip>
#include "simulation.hpp"
#include <iostream>
#include <algorithm>

#define TIME_SPEED 1000
//#define TIME_SPEED 1

int simulation(sf::RenderWindow& window, Parcours& parcours, std::vector<Coureur>& coureurs) {
    window.clear(sf::Color(0, 48, 73));
    
    /*************************************************
       Use threads to display the different modules
    *************************************************/
    /* MODULES:
    *      1 - Top Coureurs Display (top left)
    *      2 - Live Parcours (top 5 coureurs) (middle left)
    *      3 - Stopwatch (top right)
    *      4 - Top Speeds Display + Infos on the parcours (bottom right)
    */

    /*******************
       INITIALISATION
    *******************/
    // basic UI stuff
    sf::Font firaCode;
    if(!firaCode.loadFromFile("font/FiraCode.ttf"))
        return 1;

    sf::RectangleShape horizontalSeparator(sf::Vector2f(window.getSize().x, 2));
    horizontalSeparator.setFillColor(sf::Color(252, 191, 73));
    horizontalSeparator.setPosition(0.f, window.getSize().y * 1/3);
    window.draw(horizontalSeparator);
    sf::RectangleShape verticalSeparator(sf::Vector2f(2, window.getSize().y));
    verticalSeparator.setFillColor(sf::Color(252, 191, 73));
    verticalSeparator.setPosition(window.getSize().x*2/3, 0.f);
    window.draw(verticalSeparator);

    //1-Top Coureurs
    // Display the five first runners for the init
    size_t coureurAmountToDisplay = (coureurs.size() < 5)? coureurs.size() : 5;
    sf::RectangleShape coureurDistRectangles[5];
    sf::Text coureurNames[5];
    
    for (size_t i = 0; i < coureurAmountToDisplay; ++i ) {
        coureurNames[i].setFont(firaCode);
        coureurNames[i].setCharacterSize(20);
        coureurNames[i].setString(coureurs[i].getName() + ": " + std::to_string(coureurs[i].getDistanceRan()) + "km"); 
        /*
        //DEBUG
        for (auto it = coureurNames[i].getString().begin(); it != coureurNames[i].getString().end(); ++it)
            std::cout << (char)*it;
        std::cout << "\n";
        */
        coureurNames[i].setPosition(20, 20 + 50*i);

        //Add an outline to the RectangleShapes
        coureurDistRectangles[i].setOutlineThickness(5.f);
        coureurDistRectangles[i].setFillColor(sf::Color(234, 226, 183));

        //The first coureur's name is brighter
        if (!i) {
            /* Lemon Meringue */
            //coureurNames[i].setFillColor(sf::Color(252, 191, 73));
            //coureurDistRectangles[i].setOutlineColor(sf::Color(252, 191, 73));
            /* Maximum Red */
            coureurNames[i].setFillColor(sf::Color(214, 40, 40));
            coureurDistRectangles[i].setOutlineColor(sf::Color(214, 40, 40));
        } else {
            coureurNames[i].setFillColor(sf::Color(247, 127, 0));
            coureurDistRectangles[i].setOutlineColor(sf::Color(247, 127, 0));
        }

        // Get the coureur's distance to the end to display a progress bar
        coureurDistRectangles[i].setPosition(300, coureurNames[0].getLocalBounds().height + 50*i + 3);//Adding a little 3 to center things out
        coureurDistRectangles[i].setSize(sf::Vector2f(coureurs[i].getDistanceRan()  / parcours.getTotalDistance() * 500 + 1, coureurNames[0].getLocalBounds().height));

        window.draw(coureurNames[i]);
        window.draw(coureurDistRectangles[i]);
    }
    
    
    //2-Live Parcours
    //sf::Vertex vertices[parcours.getCheckpointAmount()];
    sf::VertexArray vertices(sf::LineStrip, parcours.getCheckpointAmount());
    // Get max/min X and Y
    float tmpX, tmpY, maxX = parcours.getCheckpointX(0), minX = parcours.getCheckpointX(0), maxY = parcours.getCheckpointY(0), minY = parcours.getCheckpointY(0);
    for (size_t i = 1; i < parcours.getCheckpointAmount(); ++i) {
        tmpX = parcours.getCheckpointX(i);
        tmpY = parcours.getCheckpointY(i);
        if (maxX < tmpX)
            maxX = tmpX;
        else if (minX > tmpX)
            minX = tmpX;
        if (maxY < tmpY)
            maxY = tmpY;
        else if (minY > tmpY)
            minY = tmpY;
    }

    // Transform the parcours (x,y) to (X,Y) to fit the window scale
    // First the magic formula : |windowMax - windowMin|/|xMax - xMin| * actualCoords
    float xTransform = std::abs( (window.getSize().x * 2/3 - 20)/(maxX-minX) );
    float yTransform = std::abs( (window.getSize().y * 2/3 - 20)/(maxY-minY) );
    //-20 stands for the padding + don't forget to add 1/3 of the window.y size to place correctly
    
    for (size_t i = 0; i < parcours.getCheckpointAmount(); ++i) {
        vertices[i] = sf::Vertex(sf::Vector2f(parcours.getCheckpointX(i)*xTransform + 10, (parcours.getCheckpointY(i)*yTransform) + (window.getSize().y * 1/3) + 10), sf::Color(214, 40, 40) );
        //std::cout << vertices[i].position.x << ", " << vertices[i].position.y << std::endl;
        //std::cout << vertices[i].texCoords.x << ", " << vertices[i].texCoords.y << std::endl;
    }
    
    window.draw(vertices);
    


    //3-Stopwatch 
    sf::Time intervalle = sf::seconds(0.f);
    sf::Time realTime = sf::seconds(0.f);
    sf::Text timeDisplay("00:00:00", firaCode, 40);
    timeDisplay.setPosition(window.getSize().x - 210.f, 0.f);
    timeDisplay.setFillColor(sf::Color(214, 40, 40));
    window.draw (timeDisplay);//Set the init display
    

    //4-Parcours Info + Top Speeds
    //infos
    sf::Text totalParcoursDistance("Total length: " + std::to_string(parcours.getTotalDistance()/1000) + "km", firaCode, 20);
    totalParcoursDistance.setPosition((window.getSize().x * 2/3) + 10.f, (window.getSize().y * 1/3) + 10.f);
    totalParcoursDistance.setFillColor(sf::Color(247, 127, 0));
    window.draw(totalParcoursDistance); 
    //maxspeed
    sf::Text maxSpeedText("I'm fast", firaCode, 15);
    maxSpeedText.setPosition(totalParcoursDistance.getPosition() + sf::Vector2f(0.f, 30.f));
    maxSpeedText.setFillColor(sf::Color(247, 127, 0));
    //minspeed
    sf::Text minSpeedText("I'm slow ;(", firaCode, 15);
    minSpeedText.setPosition(totalParcoursDistance.getPosition() + sf::Vector2f(0.f, 60.f));
    minSpeedText.setFillColor(sf::Color(247, 127, 0));
    //update the starting speed
    updateSpeeds(parcours, coureurs);
    window.draw(maxSpeedText);
    window.draw(minSpeedText);


    
    window.display();//show the init display
    //-----------------------------------------------------------------------------------
    
    /* //DEBUG
    for (std::vector<Coureur>::iterator it = coureurs.begin(); it != coureurs.end(); ++it) {
        std::cout << it->getName() << ", " << it->getDistanceRan() << ", " << it->getSpeed() << std::endl;
    }
    */

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
                    return EXIT_SUCCESS;
                    break;
                default:
                    break;
            }
        }


        //1-TOP Coureurs
        //Check if someone has won (yes I know if two guys won during the same frame, the one that went further on last frame's distanceUpdate and got sorted out)
        if (updateDistances(intervalle, parcours, coureurs, realTime.asSeconds()) == -1) {
            //Win stuff
            if (exportResults(parcours, coureurs)) {
                std::cerr << "ERROR EXPORTING RESULTS\n";
            }

           return 2;
        }
        //Put the best coureurs on top of the vector
        std::sort(coureurs.begin(), coureurs.end(), [](const Coureur& a, const Coureur& b) {
            return (a.getPositionFinishedAt() != b.getPositionFinishedAt()) ? a.getPositionFinishedAt() < b.getPositionFinishedAt() : a.getDistanceRan() > b.getDistanceRan();
        });
        
        for (size_t i = 0; i < coureurAmountToDisplay; ++i ) {
            coureurNames[i].setString(coureurs[i].getName() + ": " + std::to_string(coureurs[i].getDistanceRan()/1000) + "km");  
            coureurDistRectangles[i].setSize(sf::Vector2f((coureurs[i].getDistanceRan() / parcours.getTotalDistance())*400 + 1, 20));
        }


        //2-Parcours display
        //TODO
        //Update the five best coureurs' position on the parcours
        

        //3-Stopwatch
        intervalle = stopwatch.restart();
        realTime += intervalle;
        //Update the time display
        std::thread thread3(updateTime, std::ref(realTime), std::ref(timeDisplay));
        //A thread isn't needed here but it's a neat addition nonetheless


        //4-Speeds + basic parcours' infos
        //speed update
        updateSpeeds(parcours, coureurs);
        updateMaxMinSpeed(maxSpeedText, minSpeedText, coureurs);


        //Wait for the threads to end
        thread3.join();
        


        /***************
         DISPLAY UPDATE
        ***************/
        window.clear(sf::Color(0, 48, 73));
        window.draw(horizontalSeparator);
        window.draw(verticalSeparator);
        //1
        for (size_t i = 0; i < coureurAmountToDisplay; ++i) {
            window.draw(coureurNames[i]);
            window.draw(coureurDistRectangles[i]);
        }
        //2
        window.draw(vertices);
        //3
        window.draw (timeDisplay);
        //4
        window.draw(totalParcoursDistance);
        window.draw(maxSpeedText);
        window.draw(minSpeedText);

        window.display();
    
    }

    return EXIT_FAILURE;
}


void updateTime(sf::Time& time, sf::Text& txt) {
    int timeInSeconds = time.asSeconds() * TIME_SPEED;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (int)timeInSeconds/3600 << ':' << std::setw(2) << (timeInSeconds/60)%60 << ':' << std::setw(2) << timeInSeconds%60;
    txt.setString(ss.str());
}

void centerText(sf::Text& txt, sf::RenderWindow& window) {
    sf::FloatRect textRect = txt.getLocalBounds();
    txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    txt.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f);
}

void updateSpeeds(const Parcours& p, std::vector<Coureur>& v) {
    for (size_t i = 0; i < v.size(); ++i)
        v[i].updateSpeed(p);
}

void updateMaxMinSpeed(sf::Text& txtMax, sf::Text& txtMin, const std::vector<Coureur>& v) {
    //STL(ol) - unable to make this one work...
    /*
    std::pair<std::vector<Coureur>::iterator, std::vector<Coureur>::iterator> tmp = std::minmax_element(v.begin(), v.end(), [](const Coureur& a, const Coureur& b){
        return a.getSpeed() < b.getSpeed();
    });
    */
    //max
    Coureur tmp = *std::max_element(v.begin(), v.end(), [](const Coureur& a, const Coureur& b){
        return a.getSpeed() < b.getSpeed();
    });
    txtMax.setString("Max Speed: " + tmp.getName() + ": " + std::to_string(tmp.getSpeed()*3.6) + "km/h");
    
    //min
    tmp = *std::min_element(v.begin(), v.end(), [](const Coureur& a, const Coureur& b){
        return a.getSpeed() < b.getSpeed();
    });
    txtMin.setString("Min Speed: " + tmp.getName() + ": " + std::to_string(tmp.getSpeed()*3.6) + "km/h");
    

}

int updateDistances(const sf::Time& elapsedTime, const Parcours& p, std::vector<Coureur>& v, const float& currentTime) {
    static size_t finishedRaces = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].getPositionFinishedAt() == -2) {
            if (v[i].getDistanceRan() >= p.getTotalDistance()) {
                //WIN CASE
                v[i].setPositionFinishedAt(finishedRaces);
                v[i].setTimeFinishedAt(currentTime * TIME_SPEED);
                v[i].setDistanceRan(p.getTotalDistance());
                v[i].setSpeed(0.f);
                std::cout << v[i].getName() << " just crossed the finish line!\n";
            
                ++finishedRaces;
            } else {
                // d = v * t MULTIPLY HERE TO SPEED UP THE TIME
                v[i].setDistanceRan(v[i].getDistanceRan() + (v[i].getSpeed() * elapsedTime.asSeconds()*TIME_SPEED));
                //Check if the runner has reached the end or a checkpoint
        
                if (v[i].getDistanceRan() >= p.getCheckpointDistance(v[i].getCurrentCheckpoint()+1)) {
            
                    v[i].setCurrentCheckpoint(v[i].getCurrentCheckpoint()+1);
                    std::cout << v[i].getName() << " reached checkpoint n°" << v[i].getCurrentCheckpoint() << std::endl;
                    //Hydration part
                    if (p.getCheckpointFood(v[i].getCurrentCheckpoint())) {
                        v[i].updateHydration(p);
                    }
                }
            }
        }
        

        //If everyone has finished => stop the simulation + export the results to a file
        if (finishedRaces == v.size()) {
            std::cout << "--- RACE FINISHED ---\n";
            finishedRaces = 0;//Reset to enable a game restart
            //Get out of the function to process the end screen display
            return -1;
        }
    }
    return EXIT_SUCCESS;
}

