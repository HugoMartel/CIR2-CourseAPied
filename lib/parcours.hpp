#ifndef __PARCOURS__
#define __PARCOURS__
#include <string>
#include <iostream>

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
    bool getFood() {return hasFood;};
    void setX(const float& X) {x = X;};
    void setY(const float& Y) {y = Y;};
    void setZ(const float& Z) {z = Z;};
    void setFood(const bool& hF) {hasFood = hF;};
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    };
};

class Parcours {
private:
    Checkpoint* checkpoints;    //Stores a path of n checkpoints
    float* slopes;              //Stores the n slopes between checkpoints
    //I use slopes to adapt the runners speed to the way
    float* angles;              //Stores the angles of the road to ease the calculations of speed
    float* distances;           //Stores each checkpoint's distance from the start
    size_t checkpointAmount;    // = n checkpoints
    float windDirection;        // using enum Direction
    float windStrength;         // in m/s !!!
    float totalDistance;        // used for smooth displays
public:
    int genWind();
    //GETTERS
    float getCheckpointX(const size_t&) const;
    float getCheckpointY(const size_t&) const;
    bool getCheckpointFood(const size_t&) const;
    size_t getCheckpointAmount() const; 
    float getWindDirection() const;
    float getWindStrength() const;
    float getSlope(const size_t&) const;
    float getAngle(const size_t&) const;
    float getCheckpointDistance(const size_t&) const;
    float getTotalDistance() const;
    //CONSTRUCTORS
    Parcours(const std::size_t&, Checkpoint*);//using a known circuit
    Parcours();//Generates a random circuit
    Parcours(const std::string&);//Loads the circuit from a file
    ~Parcours();
    //DEBUG
    void printWind() const;
    void printCheckpoints() const {
        std::cout << "{\n";
        for (size_t i = 0; i < checkpointAmount; ++i) {
            std::cout << "\t";
            checkpoints[i].print();
            std::cout << std::endl;
        }
        std::cout << "}\n";
    };
    void printDistances() const {
        for (size_t i = 0; i < checkpointAmount; ++i)
            std::cout << distances[i] << std::endl;
        std::cout << "-----------------\n";
    }
    void printAngles() const {
        for (size_t i = 0; i < checkpointAmount; ++i)
            std::cout << angles[i] << std::endl;
        std::cout << "-----------------\n";
    }
    void printSlopes() const {
        for (size_t i = 0; i < checkpointAmount; ++i)
            std::cout << slopes[i] << std::endl;
        std::cout << "-----------------\n";
    }
};



#endif
