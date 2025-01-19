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
#define WORLD_SIZE 4000
#define FPS 48
#define TPS 100
#define Dt 0.03f

#define K sf::Keyboard
#define C sf::Color
using sf::Mouse;

#define RANDOM_SEED 0

#define REPULSION_START      5
#define SECONDARY_FORCE_END  40
#define MAX_RESULSIVE_FORCE  -2
#define GENERAL_FORCE_MULTIP 20

//#define LINEAR_RESISTANCE_OVER_DT 0.6f
//#define QUADRATIC_RESISTANCE pow(2, -8)
#define LINEAR_RESISTANCE 1

#define BORDER_FORCE pow(2, 10)

#define MAX_OBJECT_COUNT pow(10, 4)

#define FORCE_MATRIX { \
    { 1, 0, 0, 0, 0, 0 ,0}, /*what red is attracted to*/ \
    { 0, 0, 0, 0, 0, 0 ,0}, /*green*/ \
    { -1, 0, 0, 0, 0, 0 ,0}, /*blue*/ \
    { 0, 0, 0, 0, 0, 0 ,0}, /*white*/ \
    { 0, 0, 0, 0, 0, 0 ,0}, /*magenta*/ \
    { 0, 0, 0, 0, 0, 0 ,0}, /*cyan*/ \
    { 0, 0, 0, 0, 0, 0 ,0} /*yellow*/ \
}


//#define FORCE_MATRIX { \
//    { 2, 1, 0}, /*what red is attracted to*/ \
//    { -1, 2, 0}, /*green*/ \
//    { 0, 0, 0} /*blue*/ \
//}