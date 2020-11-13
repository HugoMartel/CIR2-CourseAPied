#ifndef __PARCOURS__
#define __PARCOURS__
#include <string>


class Checkpoint {
private:
    float x;//horizontal
    float y;//vertical
    float z;//used to process the slopes
    bool hasFood;
    
public:
    Checkpoint(const float& nx = 0.0, const float& ny = 0.0, const float& nz = 0.0, const bool& hF = false);
    float getX() {return x;};
    float getY() {return y;};
    float getZ() {return z;};
};

class Parcours {
private:
    Checkpoint* checkpoints;    //Stores a path of n checkpoints
    float* slopes;              //Stores the n slopes between checkpoints
    //I use slopes to adapt the runners speed to the way
    float* angles;              //Stores the angles of the road to ease the calculations of speed
    size_t checkpointAmount;    // = n checkpoints
    float windDirection;        // using enum Direction
    float windStrength;         // in m/s !!!
public:
    int genWind();
    //GETTERS
    float getCheckpointX(const size_t&) const;
    float getCheckpointY(const size_t&) const;
    size_t getCheckpointAmount() const; 
    float getWindDirection() const;
    float getWindStrength() const;
    float getSlope(const size_t&) const;
    float getAngle(const size_t&) const;
    //CONSTRUCTORS
    Parcours(const std::size_t&, const Checkpoint*);//using a known circuit
    Parcours();//Generates a random circuit
    Parcours(const std::string&);//Loads the circuit from a file
    ~Parcours();
    //DEBUG
    void printWind() const;
};



#endif
