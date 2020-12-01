#include "parcours.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

/***********
 EXCEPTIONS
***********/
struct NegativeValue{};
struct BadLineRead{};


Checkpoint::Checkpoint(const float& nx, const float& ny, const float& nz, const bool& hF) {
    x = nx;
    y = ny;
    z = nz;
    hasFood = hF;
}


/*************
 CONSTRUCTORS
*************/
//Basic one
Parcours::Parcours(const std::size_t& n, Checkpoint* points) {
    if (!n or points == nullptr) {
        std::cout << "-- BAD ARGS PASSED TO PARCOURS CONSTRUCTOR --" << std::endl;
        return;
    }
    
    checkpointAmount = n;
    checkpoints = new Checkpoint[checkpointAmount];
    slopes = new float[checkpointAmount];
    angles = new float[checkpointAmount];
    distances = new float[checkpointAmount];
    totalDistance = 0;//To increment during the filling of distances

    if(checkpoints == nullptr or slopes == nullptr or angles == nullptr or distances == nullptr) {
        std::cout << "-- ERROR ALLOCATING MEMORY FOR THE PARCOURS --" << std::endl;
        return;
    }
    
    size_t i;

    //Copying the checkpoints into the array 
    for (i = 0; i < checkpointAmount; ++i) {
        checkpoints[i] = points[i];
    }

    //Process and fill the slopes values
    for (i = 0; i < checkpointAmount - 1; ++i) {
        slopes[i] = checkpoints[i].getZ() - checkpoints[i+1].getZ();
        slopes[i] = 100 * ((slopes[i] < 0)? -std::atan2(std::abs(slopes[i]), std::sqrt( std::pow(checkpoints[i+1].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[i+1].getY()-checkpoints[i].getY(),2) )) : std::atan2(slopes[i], std::sqrt( std::pow(checkpoints[i+1].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[i+1].getY()-checkpoints[i].getY(),2) )) )/45;
    }
    slopes[i] = checkpoints[i].getZ() - checkpoints[0].getZ();
    slopes[i] = 100 * ((slopes[i] < 0)? -std::atan2(std::abs(slopes[i]), std::sqrt( std::pow(checkpoints[0].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[0].getY()-checkpoints[i].getY(),2) )) : std::atan2(slopes[i], std::sqrt( std::pow(checkpoints[0].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[0].getY()-checkpoints[i].getY(),2) )) )/45;
 

    //Process and fill the angle values (to get how the path is oriented in comparaison to the x axis -->)
    for (i = 0; i < checkpointAmount - 1; ++i) {
        //atan of each parcours part the ease the calculation of windSpeed's influence on runners
        angles[i] = std::atan2( std::abs(checkpoints[i+1].getY()-checkpoints[i].getY()), std::abs(checkpoints[i+1].getX()-checkpoints[i].getX()) ) * 180/3.14159265;
    }
    angles[i] = std::atan2( std::abs(checkpoints[0].getY()-checkpoints[i].getY()), std::abs(checkpoints[0].getX()-checkpoints[i].getX()) ) * 180/3.14159265;
    
    //Fill the distances values (
    for (i = 0; i < checkpointAmount - 1; ++i) {
        distances[i] = totalDistance + std::sqrt(std::pow(checkpoints[i+1].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[i+1].getY()-checkpoints[i].getY(),2));
        
    }
    distances[i] = totalDistance + std::sqrt( std::pow(checkpoints[0].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[0].getY()-checkpoints[i].getY(),2) ) * 1000;//Convert to meters
    totalDistance = distances[i];//we could use distances[max] but having it separated is easier

    //Generate the parcours' base wind conditions
    genWind();
}

// Constructor from file
Parcours::Parcours(const std::string& fileName) {
    //Open the file
    std::ifstream toLoad(fileName);
    if (!toLoad.is_open()) {
        std::cerr << "Couldn't open the file provided... (ParcoursFromFile)\n";
        return;
    }

    //Read content from it
    std::string line;

    /*
    +---------------+
    | File Template |
    +===============+
    | x             |
    | y             |
    | z             |
    | hasFood       |
    +===============+
    |   Separator   |
    +---------------+
    */

    /*\---------------------------------------------------------------/!\
    The first line contains the amount of Checkpoints written in the file
    \!/---------------------------------------------------------------\*/
    std::getline(toLoad, line);
    try {
        int checkpointAmountFromFile = std::stoi(line);
        if (checkpointAmountFromFile <= 0) {
            throw NegativeValue();
        } else
            checkpointAmount = checkpointAmountFromFile;
    } catch (NegativeValue& e) {
        std::cerr << "File's first line wrong or corrupted... (ParcoursFromFile)\n";
        Parcours();//Get the default constructor to prevent an error
        return; 
    }

    checkpoints = new Checkpoint[checkpointAmount];
    slopes = new float[checkpointAmount];
    angles = new float[checkpointAmount];
    distances = new float[checkpointAmount];
    totalDistance = 0.f;//To increment during the filling of distances

    if(checkpoints == nullptr or slopes == nullptr or angles == nullptr or distances == nullptr) {
        std::cout << "-- ERROR ALLOCATING MEMORY FOR THE PARCOURS --" << std::endl;
        Parcours();
        return;
    }
    
    size_t i = 0, lineCounter = 0;
    //Read the checkpoints into the array from the file
    //We assume that the parcours in the file is correct (ex: just a straight line)
    while (std::getline(toLoad, line)) {
        if (line[0] == '{' or line[0] == '}')
            continue;//SKIP THE SEPARATORS
        //std::cout << line << "|\n";//DEBUG
        
        try {
            //Case for each checkpoint value
            switch (lineCounter) {
                case 0://float x
                    if (line != "")
                        checkpoints[i].setX(std::stof(line));
                    else
                        throw BadLineRead();
                    ++lineCounter;
                    break;
                case 1://float y
                    if (line != "")
                        checkpoints[i].setY(std::stof(line));
                    else
                        throw BadLineRead();
                    ++lineCounter;
                    break;
                case 2://float z
                    if (line != "")
                        checkpoints[i].setZ(std::stof(line));
                    else
                        throw BadLineRead();
                    ++lineCounter;
                    break;
                case 3://bool hasFood
                    if (line != "")
                        checkpoints[i].setFood((bool) std::stoi(line));
                    else
                        throw BadLineRead();
                    ++i;
                    lineCounter = 0;
                    break;
                default:
                    throw BadLineRead();
            }//END Switch
        } catch (BadLineRead& e) {
            std::cerr << "File corrupted or not correctly written... (ParcoursFromFile)\n";
            Parcours();
            return;
        }
    }//END While


    //Process and fill the slopes values
    for (i = 0; i < checkpointAmount - 1; ++i) {
        slopes[i] = checkpoints[i].getZ() - checkpoints[i+1].getZ();
        slopes[i] = 100 * ((slopes[i] < 0)? -std::atan2(std::abs(slopes[i]), std::sqrt( std::pow(checkpoints[i+1].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[i+1].getY()-checkpoints[i].getY(),2) )) : std::atan2(slopes[i], std::sqrt( std::pow(checkpoints[i+1].getX()-checkpoints[i].getX(),2) + std::pow(checkpoints[i+1].getY()-checkpoints[i].getY(),2) )) )/45;
    }
    slopes[i] = 0;//We don't care about the finish line slope
 

    //Process and fill the angle values (to get how the path is oriented in comparaison to the x axis -->)
    for (i = 0; i < checkpointAmount - 1; ++i) {
        //atan of each parcours part the ease the calculation of windSpeed's influence on runners
        angles[i] = std::atan2( std::abs(checkpoints[i+1].getY()-checkpoints[i].getY()), std::abs(checkpoints[i+1].getX()-checkpoints[i].getX()) ) * 180/3.14159265;
    }
    angles[i] = 0;//we don't care about the finish line angle
    

    //Fill the distances values => sqrt(x^2 + y^2 + z^2)
    distances[0] = 0;
    for (i = 1; i < checkpointAmount; ++i) {
        totalDistance += std::sqrt(std::pow(checkpoints[i].getX()-checkpoints[i-1].getX(),2) + std::pow(checkpoints[i].getY()-checkpoints[i-1].getY(),2) + std::pow(checkpoints[i].getZ()-checkpoints[i-1].getZ(),2) ) * 1000;//Convert to meters
        distances[i] = totalDistance;
    }
    

    //Generate the parcours' base wind conditions
    genWind();
}


//Default one, builds a square path
Parcours::Parcours() {
    Checkpoint c[4] = {   Checkpoint(0.f, 0.f, 0.f),
                          Checkpoint(0.f, 1.f, 1.f),
                          Checkpoint(1.f ,1.f, 2.f),
                          Checkpoint(1.f, 0.f, 0.f) };
    Parcours(4, c);
    return;
}

/***********
 DESTRUCTOR
***********/
Parcours::~Parcours() {
    delete[] checkpoints;
    delete[] slopes;
    delete[] angles;
    delete[] distances;
}


//Wind Generation
int Parcours::genWind() {
    //Setting up a seed c++11 style
    std::default_random_engine generator(std::random_device{}());
    
    std::normal_distribution<float> ranNormalFloat(10, 5);//mu = 10, sigma = 5
   
    // Which means 95% of the values between 10 and 50
    //Normal random float between 0 and 80 km/h -> 80/3.6 m/s
    do {
        windStrength = ranNormalFloat(generator);
    } while (windStrength <= 0.0 or windStrength > 40.0);
    windStrength /= 3.6;//Convert to m/s


    //Uniform random float to gen the wind Angle
    std::uniform_real_distribution<float> ranUniformFloat(0.f, 359.99f);
    windDirection = ranUniformFloat(generator);

    return EXIT_SUCCESS; 
}


float Parcours::getCheckpointX(const size_t& n) const {
    return (n < checkpointAmount)? checkpoints[n].getX() : 0;
}

float Parcours::getCheckpointY(const size_t& n) const {
    return (n < checkpointAmount)? checkpoints[n].getY() : 0;
}

float Parcours::getSlope(const size_t& n) const {
    return (n < checkpointAmount)? slopes[n] : 0;
}

float Parcours::getAngle(const size_t& n) const {
    return (n < checkpointAmount)? angles[n] : 0;
}

float Parcours::getCheckpointDistance(const size_t& n) const {
    return (n < checkpointAmount)? distances[n] : 0;
}

float Parcours::getTotalDistance() const {
    return totalDistance;
}

size_t Parcours::getCheckpointAmount() const {
    return checkpointAmount;
}

float Parcours::getWindDirection() const {
    return windDirection;
}

float Parcours::getWindStrength() const {
    return windStrength;
}

/********
  DEBUG
********/
void Parcours::printWind() const {
    std::cout << windDirection << ", " << windStrength << std::endl;
}
