#ifndef TEST_H
#define TEST_H

#include "castar.hpp"
#include "field.hpp"
#include <iostream>
#include "math.h"
#include <chrono> 
#include <bits/stdc++.h> 

bool is_in_circle(Coordinates pos, Circle c);
void board_visualize(Field board, std::vector<Coordinates> path);
void test(void);
#endif