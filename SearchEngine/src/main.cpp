#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>

#include "Search.h"

int balance = 100;

std::mutex mutex;

void calc(unsigned int id)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "Thread " << id << ": balance=" << balance << "\n";
    usleep(1000000);
    std::cout << "Thread " << id << ": new balance=" << balance << "\n";
}

int main(int argc, char** argv) 
{
    std::thread t1(calc, 1);
    std::thread t2(calc, 2);

    t1.join();
    t2.join();
    // Search::Search* search = new Search::Search();
    // int status = search->start();
    // delete search;
    // return status;
}