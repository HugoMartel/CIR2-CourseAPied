#ifndef __SIMULATION__
#define __SIMULATION__
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "parcours.hpp"
#include "coureur.hpp"
#include "results.hpp"

//
int simulation(sf::RenderWindow&, Parcours&, std::vector<Coureur>&);
//
void updateTime(sf::Time&, sf::Text&);
//
void centerText(sf::Text&, sf::RenderWindow&);
//
void updateSpeeds(const Parcours&, std::vector<Coureur>&);
//
void updateMaxMinSpeed(sf::Text&, sf::Text&, const std::vector<Coureur>&);
//
int updateDistances(const sf::Time&, const Parcours&, std::vector<Coureur>&, const float&);

#endif //__SIMULATION__
