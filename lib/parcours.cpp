#include "parcours.hpp"
#include <iostream>
#include <random>
#include <cmath>

Checkpoint::Checkpoint(const float& nx, const float& ny, const float& nz, const bool& hF) {
    x = nx;
    y = ny;
    z = nz;
    hasFood = hF;
}

Parcours::Parcours(const std::size_t& n, const Checkpoint* points) {
    if (!n or points == nullptr) {
        std::cout << "-- BAD ARGS PASSED TO PARCOURS CONSTRUCTOR --" << std::endl;
        return;
    }
    
    checkpointAmount = n;
    checkpoints = new Checkpoint[checkpointAmount];
    slopes = new float[checkpointAmount];
    angles = new float[checkpointAmount];

    if(checkpoints == nullptr or slopes == nullptr or angles == nullptr) {
        std::cout << "-- ERROR ALLOCATING MEMORY FOR THE PARCOURS --" << std::endl;
        return;
    }

    //Copying the checkpoints into the array
    size_t i;
    for (i = 0; i < checkpointAmount; ++i) {
        checkpoints[i] = points[i];
    }
    //Process and fill the slopes values
    for (i = 0; i < checkpointAmount - 1; ++i) {
        slopes[i] = checkpoints[i].getZ() - checkpoints[i+1].getZ();
    }
    slopes[i] = checkpoints[i].getZ() - checkpoints[0].getZ();

    //Process and fill the angle values
    for (i = 0; i < checkpointAmount - 1; ++i) {
        //atan of each parcours part the ease the calculation of windSpeed's influence on runners
        angles[i] = std::atan2( std::abs(checkpoints[i+1].getY()-checkpoints[i].getY()), std::abs(checkpoints[i+1].getX()-checkpoints[i].getX()) ) * 180/3.14159265;
    }
    slopes[i] = std::atan2( std::abs(checkpoints[0].getY()-checkpoints[i].getY()), std::abs(checkpoints[0].getX()-checkpoints[i].getX()) ) * 180/3.14159265;
 

    //Generate the parcours' base wind conditions
    genWind();
}

Parcours::~Parcours() {
    delete[] checkpoints;
    delete[] slopes;
    delete[] angles;
}


//Wind Generation
int Parcours::genWind() {
    //Setting up a seed c++11 style
    std::default_random_engine generator(std::random_device{}());
    
    std::normal_distribution<float> ranNormalFloat(30, 10);//mu = 30, sigma = 10
    // Which means 95% of the values between 10 and 50
    //Normal random float between 0 and 80 km/h -> 80/3.6 m/s
    do {
        windStrength = ranNormalFloat(generator);
    } while (windStrength <= 0.0 or windStrength > 80.0);
    windStrength /= 3.6;//Convert to m/s


    //Uniform random float to gen the wind Angle
    std::uniform_real_distribution<float> ranUniformFloat(0.f, 359.99f);
    windDirection = ranUniformFloat(generator);

    return EXIT_SUCCESS; 
}

float Parcours::getCheckpointX(const size_t& n) const {
    return (n >= checkpointAmount)? checkpoints[n].getX() : 0;
}

float Parcours::getCheckpointY(const size_t& n) const {
    return (n >= checkpointAmount)? checkpoints[n].getY() : 0;
}

float Parcours::getSlope(const size_t& n) const {
    return (n >= checkpointAmount)? slopes[n] : 0;
}

float Parcours::getAngle(const size_t& n) const {
    return (n >= checkpointAmount)? angles[n] : 0;
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
