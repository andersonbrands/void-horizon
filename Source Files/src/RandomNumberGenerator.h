#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <stdlib.h>    //for srand and rand routines
#include <cassert>    //for assert
#include <ctime>    //for time used in RandomNumberGenerator::seed routines
using namespace std;

class RandomNumberGenerator
{
    public:
        //constructors
        RandomNumberGenerator();
        //assessors
        int getRandomValue( int) const;
    private:
        //supporting functions
        void seed();
};
#endif // !defined(RandomNumberGeneratorH)