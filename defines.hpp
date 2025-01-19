#pragma once
#include "SFML/Graphics.hpp"

#define fpoint float
using std::vector;
#define vectorList vector

#define forcount(x) for (int i = 0; i < x; i++)

#define scrh 1080
#define scrw 1920
#define Dt .01f

#define K sf::Keyboard
#define C sf::Color
using sf::Mouse;

#define RANDOM_SEED 0

#define REPULSION_START      10
#define SECONDARY_FORCE_END  35
#define MAX_RESULSIVE_FORCE  -10
#define GENERAL_FORCE_MULTIP 1

#define LINEAR_RESISTANCE .1

#define BORDER_FORCE pow(2, 8)

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