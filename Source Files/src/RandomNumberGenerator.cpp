#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() {
    seed();
}

int RandomNumberGenerator::getRandomValue( int max) const {
    return ( rand() % max) + 1;
}

void RandomNumberGenerator::seed() {
    srand( static_cast<unsigned>(time(0)));
}