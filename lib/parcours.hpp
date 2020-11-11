#ifndef __PARCOURS__
#define __PARCOURS__
#include <string>

enum Direction {
    N  = 0,
    NE = 1,
    W  = 2,
    SE = 3,
    S  = 4,
    SW = 5,
    E  = 6,
    NW = 7
};

class Checkpoint {
private:
    float x;//horizontal
    float y;//vertical
    float z;//used to process the slopes
    bool hasFood;
    
public:
    Checkpoint(const float&, const float&, const float&, const bool&, const Direction&, const float&);
    float getX() {return x;};
    float getY() {return y;};
    float getZ() {return z;};
};

class Parcours: public Checkpoint {
private:
    Checkpoint* checkpoints;    //Stores a path of n checkpoints
    float* slopes;              //Stores the n slopes between checkpoints
    //I use slopes to adapt the runners speed to the way
    std::size_t checkpointAmount;    // = n checkpoints
    Direction windDirection;         // using enum Direction
    float windStrength;              // in m/s !!!
public:
    int genWind();
    Parcours(const std::size_t&, const Checkpoint*);//using a known circuit
    Parcours();//Generates a random circuit
    Parcours(const std::string&);//Loads the circuit from a file
    ~Parcours();
};



#endif
