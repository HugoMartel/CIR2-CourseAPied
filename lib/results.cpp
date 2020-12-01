#include "results.hpp"
#include <iostream>
#include <string>
#include <ctime>

int exportResults(const Parcours& p, const std::vector<Coureur>& v) {
    //Generate the file name
    std::string fileName = "Results" + std::to_string(std::time(NULL)) + ".txt";
    std::cout << fileName << std::endl;

    //Open the file to write in it
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Couldn't open the file provided... (exportResults)\n";
        return EXIT_FAILURE;
    }

    std::cout << "Successfully exported the results to " << fileName << std::endl; 
    return EXIT_SUCCESS;
}
