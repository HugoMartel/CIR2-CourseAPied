#include "parcours.hpp"
#include <iostream>
#include <random>

Checkpoint::Checkpoint(const float& nx = 0.0, const float& ny = 0.0, const float& nz = 0.0, const bool& hF = false, const Direction& wD = N, const float& wS = 0.0) {
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

    if(checkpoints == nullptr or slopes == nullptr) {
        std::cout << "-- ERROR ALLOCATING MEMORY FOR THE CHECKPOINTS --" << std::endl;
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

    windDirection = N;
    windStrength = 0.0;
}

Parcours::~Parcours() {
    delete[] checkpoints;
    delete[] slopes;
}


//Wind Generation
int Parcours::genWind() {
    std::default_random_engine generator;
    
    std::normal_distribution<float> ranNormalFloat(40, 20);//mu = 40, sigma = 20
    //Normal random float between 0 and 80 km/h -> 80/3.6 m/s
    do {
        windStrength = ranNormalFloat(generator);
    } while (windStrength <= 0.0 or windStrength > 80.0);
    
    //Uniform random int between 0 and 7 (windDirection)
    std::uniform_int_distribution<int> ranUniformInt(0,7);
    switch (ranUniformInt(generator)) {
    case 0:
        windDirection = N;
        break;
    case 1:
        windDirection = NE;
        break;
    case 2:
        windDirection = E;
        break;
    case 3:
        windDirection = SE;
        break;
    case 4:
        windDirection = S;
        break;
    case 5:
        windDirection = SW;
        break;
    case 6:
        windDirection = W;
        break;
    case 7:
        windDirection = NW;
        break;
    default:
        return 2;
    }

    return EXIT_SUCCESS; 
}
