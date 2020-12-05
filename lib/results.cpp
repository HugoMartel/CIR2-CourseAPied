#include "results.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

int exportResults(const Parcours& p, std::vector<Coureur>& v) {
    //Generate the file name
    const std::time_t now = std::time(nullptr);
    std::string fileName = "results/";
    fileName.append(std::asctime(std::localtime(&now)));
    
    //Remove the spaces in the string
    for (size_t i = 8; i < fileName.length(); ++i) {
        if (fileName[i] == '-')
            fileName.erase(i, 1);//Remove the double space after the month
        else if (fileName[i] == ' ' or fileName[i] == '\t' or fileName[i] == '\r')
            fileName[i] = '-';
    }
    fileName.pop_back();
    fileName.append(".txt");

    std::cout << fileName << std::endl;

    //Open the file to write in it
    std::ofstream file(fileName, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Couldn't open the file provided... (exportResults)\n";
        //return EXIT_FAILURE;//Disabled to properly reset the coureurs properties
    } else {
        //TABLE EXEMPLE
        /*
        +------------+----+----------+----------+
        |   Names    | Id | Position |   Time   |
        +------------+----+----------+----------+
        | H. Martel  |  1 |        1 | 04:00:01 |
        | N. Contini |  2 |        - | DNF      |
        +------------+----+----------+----------+
        */
        {
        //Ascii Presentation
        std::string resultsASCII = " _______                             __   __              \n";
        resultsASCII            += "|       \\                           |  \\ |  \\             \n";
        resultsASCII            += "| ▓▓▓▓▓▓▓\\ ______   _______ __    __| ▓▓_| ▓▓_    _______ \n";
        resultsASCII            += "| ▓▓__| ▓▓/      \\ /       \\  \\  |  \\ ▓▓   ▓▓ \\  /       \\\n";
        resultsASCII            += "| ▓▓    ▓▓  ▓▓▓▓▓▓\\  ▓▓▓▓▓▓▓ ▓▓  | ▓▓ ▓▓\\▓▓▓▓▓▓ |  ▓▓▓▓▓▓▓\n";
        resultsASCII            += "| ▓▓▓▓▓▓▓\\ ▓▓    ▓▓\\▓▓    \\| ▓▓  | ▓▓ ▓▓ | ▓▓ __ \\▓▓    \\ \n";
        resultsASCII            += "| ▓▓  | ▓▓ ▓▓▓▓▓▓▓▓_\\▓▓▓▓▓▓\\ ▓▓__/ ▓▓ ▓▓ | ▓▓|  \\_\\▓▓▓▓▓▓\\\n";
        resultsASCII            += "| ▓▓  | ▓▓\\▓▓     \\       ▓▓\\▓▓    ▓▓ ▓▓  \\▓▓  ▓▓       ▓▓\n";
        resultsASCII            += " \\▓▓   \\▓▓ \\▓▓▓▓▓▓▓\\▓▓▓▓▓▓▓  \\▓▓▓▓▓▓ \\▓▓   \\▓▓▓▓ \\▓▓▓▓▓▓▓ \n\n";
                                                          
                                                          
        file << resultsASCII;
        }
        //TABLE HEADER (15+6+10+10)
        file << "+---------------+------+----------+----------+\n";
        file << "|     Names     |  Id  | Position |   Time   |\n";
        file << "+---------------+------+----------+----------+\n";

        //Quick sort to arrange the coureur vector nicely
        std::sort(v.begin(), v.end(), [](const Coureur& a, const Coureur& b){
            //different cases if positionFinishedAt == -1 (abandon)
            long aP = a.getPositionFinishedAt();
            long bP = b.getPositionFinishedAt();
            if (aP == bP) {
                //both gave up
                return a.getDistanceRan() < b.getDistanceRan();
            } else {
                return aP < bP;
            }
        });

        /*
        for (std::vector<Coureur>::iterator it = v.begin(); it != v.end(); ++it)
            std::cout << it->getName() << ", " << it->getPositionFinishedAt() << std::endl;
        */

        //TABLE BODY
        //WHO FINISHED
        for (std::vector<Coureur>::iterator it = v.begin(); it != v.end(); ++it) {
            file << "| " << std::setfill(' ') << std::setw(13) << it->getName();
            file << " | " << std::setw(4) << it->getId();
            file << " | " << std::setw(8);
            if (it->getPositionFinishedAt() != -1)
                file << it->getPositionFinishedAt()+1;
            else 
                file << "DNF";
            file << " | ";
            if (it->getPositionFinishedAt() != -1)
                file << std::setfill('0') << std::setw(2) << (int)it->getTimeFinishedAt()/3600 << ":" << std::setw(2) << ((int)it->getTimeFinishedAt()/60)%60 << ":" << std::setw(2) << (int)it->getTimeFinishedAt()%60; 
            else
                file << std::setw(8) << it->getDistanceRan();
            file << " |\n+---------------+------+----------+----------+\n";
        }


        //TABLE FOOTER


        //END WRITING
        file.close();
        std::cout << "Successfully exported the results to " << fileName << std::endl; 
    }


    //Reset the coureur's parcours' specific variables : checkpointsReached, etc...
    for (std::vector<Coureur>::iterator it = v.begin(); it != v.end(); ++it) {
        it->setCurrentCheckpoint(0);
        it->setDistanceRan(0);
        it->setPositionFinishedAt(-2);
        it->setTimeFinishedAt(0);
    }

    
    return EXIT_SUCCESS;
}
