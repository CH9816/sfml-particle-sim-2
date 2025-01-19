#pragma once
#include "SFML/Graphics.hpp"
#include <thread>

#define fpoint float
using std::vector;
using std::thread;
typedef std::atomic_bool abool;
#define vectorList vector

#define forcount(x) for (int i = 0; i < x; i++)
#define string(x) std::to_string(x)
#define ref(x) std::ref(x)

#define scrh 1080
#define scrw 1920
#define WORLD_SIZE 2000
#define FPS 60
#define TPS 200
#define Dt .01f

#define K sf::Keyboard
#define C sf::Color
using sf::Mouse;

#define RANDOM_SEED 0

#define REPULSION_START      10
#define SECONDARY_FORCE_END  35
#define MAX_RESULSIVE_FORCE  -10
#define GENERAL_FORCE_MULTIP 3

#define LINEAR_RESISTANCE_OVER_DT 1
#define QUADRATIC_RESISTANCE pow(2, -10)

#define BORDER_FORCE pow(2, 10)

#define MAX_OBJECT_COUNT pow(10, 4)

//#define FORCE_MATRIX { \
//    { 1, 0, 0, 0, 0, 0 ,0}, /*what red is attracted to*/ \
//    { 0, 0, 0, 0, 0, 0 ,0}, /*green*/ \
//    { -1, 0, 0, 0, 0, 0 ,0} /*blue*/ \
//    /*{ 0, 0, 0, 0, 0, 0 ,0}, white*/ \
//    /*{ 0, 0, 0, 0, 0, 0 ,0}, magenta*/ \
//    /*{ 0, 0, 0, 0, 0, 0 ,0}, cyan*/ \
//    /*{ 0, 0, 0, 0, 0, 0 ,0}, yellow*/ \
//}


#define FORCE_MATRIX { \
    { 20, 10, 0}, /*what red is attracted to*/ \
    { -10, 20, 0}, /*green*/ \
    { 0, 0, 0} /*blue*/ \
}