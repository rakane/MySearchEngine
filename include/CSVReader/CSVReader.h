#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Utils 
{
    class CSVReader 
    {
        public:
            // Default constructor
            CSVReader();
            ~CSVReader();
            
            // Open file
            int open(std::string);

            // Close file
            int close();

            // Parse csv file and return array of string representing each
            // column in row data
            int getRow(std::string&);
        private: 
            std::fstream file_;
    };
};

#endif