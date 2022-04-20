#ifndef FILTER_H
#define FILTER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "CSVReader.h"

namespace Search 
{
    class Filter 
    {
        public:
            // Default constructor
            Filter();
            
            // Destructor
            ~Filter();
            
            bool includes(std::string word) const;
        private:

            std::vector<std::string>* words_;
            Utils::CSVReader* csvReader_;
    };
};

#endif