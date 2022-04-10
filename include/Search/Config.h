#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>

#include "Synonyms.h"

namespace Search 
{
    class Config 
    {
        public:
            // Constructor
            Config();
            // Destructor
            ~Config();

            Utils::Synonyms* getSynonymDict();
        private: 
            Utils::Synonyms* synonymDict_;
    };
};

#endif