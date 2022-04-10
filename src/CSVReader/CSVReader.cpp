#include <string>
#include <iostream>

#include "CSVReader.h"

Utils::CSVReader::CSVReader() {}

Utils::CSVReader::~CSVReader() {}

int Utils::CSVReader::open(std::string filename)
{
    try {
        file_.open(filename.c_str(), std::ios::in);
        return 1;
    }
    catch(...)
    {
        return 0;
    }
}

int Utils::CSVReader::close()
{
    try {
        file_.close();
        return 1;
    }
    catch(...)
    {
        return 0;
    }
}

// Get row of data from csv file, parse each column
// Return as string array
int Utils::CSVReader::getRow(std::string& row)
{
    if (file_.is_open())
    {
        std::string line;
        if(std::getline(file_, line))
        {
            row = line;
            return 1;
        }
        else
        {
            return 0;
        }
    } 
    else
    {
        return 0;
    }
}