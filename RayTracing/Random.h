#pragma once

#include <random>

// TODO : Find a solution to handle this properly
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0f, 1.0f);

std::random_device rdu;     // only used once to initialise (seed) engine
std::mt19937 rng(rdu());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased