#pragma once

// To handle variable in multithreading
#include <atomic>

// Performance Stat
extern std::atomic_int numberOfGeometry = 0;
extern std::atomic_int numberOfPrimaryRay = 0;
extern std::atomic_int numberOfRaySphereTest = 0;
extern std::atomic_int numberOfRaySphereIntersection = 0;