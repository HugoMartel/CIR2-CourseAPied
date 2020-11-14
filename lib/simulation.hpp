#ifndef __SIMULATION__
#define __SIMULATION__
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "parcours.hpp"
#include "coureur.hpp"

int simulation(sf::RenderWindow&, Parcours&, std::vector<Coureur>&);
void updateTime(sf::Time&, sf::Text&);
void centerText(sf::Text&, sf::RenderWindow&);

#endif //__SIMULATION__
