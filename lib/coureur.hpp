#ifndef __COUREUR__
#define __COUREUR__
#include "parcours.hpp"
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
    float hydration;    // 0 - 1, see pdf for more info on its drawbacks
    float distanceRan;  // Updated in real time (SFML clock)
    float Speed;        // Updated at each frame
public:
    int updateSpeed(const Parcours&, const size_t&, const float&, const float&);
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
    std::string getName();
    unsigned int getId();
    float getDistanceRan();
    //SETTERS
    bool setName(const std::string&);
    bool setId(const unsigned int&);
    bool setDistanceRan(const float&);
    //FRIENDS
    friend int loadCoureurFromFile(const std::string&, std::vector<Coureur>&);
};


int loadCoureurFromFile(const std::string&, std::vector<Coureur>&);//Load from a file

#endif
