cmake_minimum_required(VERSION 3.10)

# set the project name
project(SearchEngine)

#Bring the headers, such as Student.h into the project
include_directories(include/Search include/Synonyms include/CSVReader)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "src/*.cpp")
 
add_executable(MySearchEngine ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(MySearchEngine PRIVATE Threads::Threads)