#ifndef UTILITY_H
#define UTILITY_H

#include "data_struct.h"
#include "data_utility.h"

#include <algorithm> 
#include <cmath>
#include <cassert>

#define SLOPE_TYPE          double

// helper data structure for count_slopes
typedef struct point_order
{
    point_t *point;
    int order;
}   point_order_t;

using namespace std;

SLOPE_TYPE              compute_slope           (point_t* p1, point_t* p2);
SLOPE_TYPE              min_slope               (point_set_t* P);
int                     count_inversions        (vector<int>& arr);
int                     count_slopes            (point_set_t* P, double alpha, double beta);
point_t**               display_points_v2       (point_set_t* P, int s, double alpha, double beta, int num_iterations);
COORD_TYPE              breakpoint_one_round    (point_set_t* P, int s, double alpha, double beta);
point_t*                breakpoint              (point_set_t* P, point_t* u, int s, int maxRound);


#endif


