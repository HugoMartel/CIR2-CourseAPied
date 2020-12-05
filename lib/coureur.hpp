#ifndef __COUREUR__
#define __COUREUR__
#include "parcours.hpp"
#include "coureur.hpp"
#include <string>
#include <vector>

class Coureur {
private:
    std::string name;
    unsigned int id;
    float mass;         // 45 - 120kg
    float height;       // 1m30 - 2m
    float shoeWeight;   // 100 - 300gr, +100gr => averageSpeed * 0.9989
    float averageSpeed; // 8 - 20km/h
    int prepWeeks;      // 8 - 16, see pdf for more info on its drawbacks
    float hydration;    // 0 - oo, TOTAL drank water
    float hydrationImpactOnSpeed;
    float distanceRan;  // Updated in real time
    float speed;        // Updated at each frame
    size_t currentCheckpoint;
    long positionFinishedAt;//-2 default, -1 if abandon, position if finished
    float timeFinishedAt;   // Time at the finish line -1 if gives up
    float Ptmax;        // Used to update to speed
public:
    //UPDATES
    int updateSpeed(const Parcours&);
    int updateHydration(const Parcours&);
    //CONSTRUCTORS
    Coureur(const std::string&,
            const unsigned int&,
            const float&,
            const float&,
            const float&,
            const float&,
            const int&
    );
    //GETTERS
    std::string getName() const;
    unsigned int getId() const;
    float getDistanceRan() const;
    size_t getCurrentCheckpoint() const;
    float getSpeed() const;
    float getTimeFinishedAt() const;
    long getPositionFinishedAt() const;
    float getHydrationImpactOnSpeed() const;
    int getPrepWeeks() const;
    //SETTERS
    bool setName(const std::string&);
    bool setId(const unsigned int&);
    bool setSpeed(const float&);
    bool setDistanceRan(const float&);
    bool setCurrentCheckpoint(const size_t&);
    bool setPositionFinishedAt(const long&);
    bool setTimeFinishedAt(const float&);
    //FRIENDS
    friend int loadCoureurFromFile(const std::string&, std::vector<Coureur>&);
};


int loadCoureurFromFile(const std::string&, std::vector<Coureur>&);//Load from a file

#endif
