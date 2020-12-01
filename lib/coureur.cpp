#include "coureur.hpp"
#include <fstream>
#include <iostream>


/*------------
 SPEED UPDATE
------------*/
int Coureur::updateSpeed(const Parcours& p) {
    //Calculate the actual feltWindSpeed for the runner : front/back wind
    float anglesRelativePosition = std::abs(p.getWindDirection() - p.getAngle(currentCheckpoint));
    float feltWindSpeed; 
    
    if (anglesRelativePosition <= 45)
        feltWindSpeed = -p.getWindStrength();//wind is boosting
    else if (anglesRelativePosition <= 225 and anglesRelativePosition >= 135)
        feltWindSpeed = p.getWindStrength();//wind is an obstacle

    float Pr = Ptmax - (0.5 * 1.205 * height)*(speed + feltWindSpeed)*(speed + feltWindSpeed)*speed;
    
    // Case of extreme wind conditions, the runner barely being able to move (the formula gives a negative speed which is wrong, especially for the distances and displays)
    if (Pr < 0) {
        std::cout << "WTF-" << name << "\n";
        Pr = 0.1;
    }

    speed = Pr / (mass * 0.98);

    float slope = p.getSlope(currentCheckpoint);
    if (slope < 0) {
        // SPEED BOOST -> 0.35% per 1.5% slope
        speed *= 1 + 0.035 * ((int)(-slope / 1.5));
    } else {
        // SPEED REDUCED -> 1% per 1.5% slope
        speed *= 1 + 0.015 * ((int)(slope / 1.5));
    }
    speed -= ((int)(shoeWeight - 100.0))*0.00011;// speed deduced from the shoeWeight 1.1e-4 deduced per gram
    std::cout << name << ": " << speed << "\n";
    return EXIT_SUCCESS;
}



/*------------
 CONSTRUCTORS
------------*/
//Default
Coureur::Coureur(const std::string& Nname = "I. Ranfast", const unsigned int& Nid = 0, const float& Nmass = 80.0, const float& Nheight = 1.7, const float& NshoeWeight = 200.0, const float& Nspeed = 14.0, const int& Nprep = 12.0) {
    name = (Nname != "")? Nname : "I. Ranfast";
    id = Nid;
    mass = (Nmass >= 44.99 and Nmass <= 120.0001)? Nmass : 80.0;
    height = (Nheight >= 1.299 and Nheight <= 2.0001)? Nheight : 1.7;
    shoeWeight = NshoeWeight;
    averageSpeed = (Nspeed >= 7.99 and Nspeed <= 20.01)? Nspeed/3.6 : 14.0/3.6;// from km/h to m/s
    prepWeeks = (Nprep >= 7.99 and Nprep <= 16.0001)? Nprep : 12.0;
    hydration = 0;// Sum of every water drank
    distanceRan = 0.0;
    speed = averageSpeed;
    currentCheckpoint = 0;
    finishedAt = 0;//-1 if abandons or time in seconds if finished
    Ptmax = (averageSpeed * mass * 0.98) + 0.5*1.205*height*averageSpeed*averageSpeed*averageSpeed;
    // 0 windSpeed
}

//-------------------------------------------------------------------------------------

//Load a Coureur config from a file
int loadCoureurFromFile(const std::string& fileName, std::vector<Coureur>& v) {
    //Open the file
    std::ifstream toLoad(fileName);
    if (!toLoad.is_open()) {
        std::cerr << "Couldn't open the file provided... (loadCoureurFromFile)\n";
        return -1;
    }
    
    //Read content from it
    std::string line;

    /*
    +---------------+
    | File Template |
    +===============+
    | name          |
    | id            |
    | mass          |
    | height        |
    | shoeWeight    |
    | averageSpeed  |
    | prepWeeks     |
    +===============+
    |   Separator   |
    +---------------+
    */

    /*\------------------------------------------------------------/!\
    The first line contains the amount of Coureurs written in the file
    \!/------------------------------------------------------------\*/
    std::getline(toLoad, line);
    int coureurAmount = std::stoi(line);//WTF C++ no stoui really ???
    if (coureurAmount <= 0) {
        std::cout << "File's first line wrong or corrupted... (loadCoureurFromFile)" << std::endl;
        return -2;
    }
    if (v.size() != (size_t) coureurAmount)
        v.resize(coureurAmount);
    
    //We now are ready to read the file actual content!
    size_t lineCounter = 0, index = 0;

    while (std::getline(toLoad, line))  { 
        if (line[0] == '{' or line[0] == '}')
            continue;//SKIP THE SEPARATORS
        
        //std::cout << line << "|" << std::endl;//DEBUG

        //Case for each
        switch (lineCounter) {
            case 0://String name
                if (line != "")
                    v[index].name = line;
                else
                    return 1;
                ++lineCounter;
                break;
            case 1://unsigned int id
                if (std::stoi(line) <= 0)
                    return 2;
                v[index].id = std::stoi(line); 
                ++lineCounter;
                break;
            case 2://float mass 45-120
                if (std::stof(line) <= 44.999 or std::stof(line) >= 120.001)
                    return 3;
                v[index].mass = std::stof(line);
                ++lineCounter;
                break;
            case 3://float height 1.3-2
                if (std::stof(line) <= 1.299 or std::stof(line) >= 2.001)
                    return 4;
                v[index].height = std::stof(line);
                ++lineCounter;
                break;
            case 4://float shoeWeight 100-300
                if (std::stof(line) <= 99.999 or std::stof(line) >= 300.001)
                    return 5;
                v[index].shoeWeight = std::stof(line);
                ++lineCounter;
                break;
            case 5://float averageSpeed 7-20
                if (std::stof(line) <= 7.999 or std::stof(line) >= 20.001)
                    return 6;
                v[index].averageSpeed = std::stof(line) / 3.6;
                ++lineCounter;
                break;
            case 6://int prepWeeks 8-16
                if (std::stoi(line) < 8 or std::stoi(line) > 16)
                    return 7;
                v[index].prepWeeks = std::stoi(line);
                lineCounter = 0;
                v[index].hydration = 0.f;
                v[index].distanceRan = 0.f;
                v[index].currentCheckpoint = 0;
                v[index].finishedAt = 0;//-1 if abandons or time in seconds if finished
                v[index].Ptmax = (v[index].averageSpeed * v[index].mass * 0.98) + (0.5 * 1.205 * v[index].height * v[index].averageSpeed * v[index].averageSpeed * v[index].averageSpeed);// 0 windSpeed
                ++index;
                break;
            default:
                std::cout << "File corrupted or not correctly written... (loadCoureurFromFile)" << std::endl;
                return -3;//File corrupted or not correctly written
        }//Switch end

    }// While end
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------

//GETTERS
std::string Coureur::getName() const {
    return name;
}

unsigned int Coureur::getId() const {
    return id;
}

float Coureur::getDistanceRan() const {
    return distanceRan;
}

size_t Coureur::getCurrentCheckpoint() const {
    return currentCheckpoint;
}

float Coureur::getSpeed() const {
    return speed;
}

float Coureur::getFinishedAt() const {
    return finishedAt;
}


//SETTERS
bool Coureur::setName(const std::string& s) {
    if (s == "")
        return false;
    name = s;
    return true;
}

bool Coureur::setId(const unsigned int& i) {
    id = i;
    return true;
}

bool Coureur::setDistanceRan(const float& dr) {
    if (dr < 0)
        return false;
    distanceRan = dr;
    return true;
}

bool Coureur::setCurrentCheckpoint(const size_t& cC) {
    currentCheckpoint = cC;
    return true;
}

bool Coureur::setFinishedAt(const float& fA) {
    if (fA != -1 and fA < 0)
        return false;
    finishedAt = fA;
        return true;
}
