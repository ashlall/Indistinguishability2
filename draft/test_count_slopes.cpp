#include "utility.h"
#include "read_write.h"


//==============================================================================================
// compute_slope
// Computes the slope between points p1 and p2
// Parameters:
//      p1 - point 1
//      p2 - point 2
// Return: 
//      Slope between p1 and p2 
//==============================================================================================
SLOPE_TYPE compute_slope(point_t* p1, point_t* p2) {
    if (p2->coord[0] == p1->coord[0]) {
        return INF; 
    }
    return (p2->coord[1] - p1->coord[1]) / (p2->coord[0] - p1->coord[0]);
}

//==============================================================================================
// Helper function for comparison in min_slope for x - coord
//==============================================================================================
bool compare_points_x(const point_t* p1, const point_t* p2) {
    // compares x values first, if same, then smaller y gets sort first
    if (abs(p1->coord[0] - p2->coord[0]) < .0001) { 
         return p1->coord[1] < p2->coord[1];
    } 
    else {
        return p1->coord[0] < p2->coord[0];
    }
}


//==============================================================================================
// min_slope
// Computes the minimum slope in a set of points P
// Parameters:
//      P       - input data set
//      0   - anchor dimension
//      dim_i   - current dimension
// Return: 
//      minimum slope in P in dimension a and i
//==============================================================================================
SLOPE_TYPE min_slope(point_set_t* P) {
    if (P->numberOfPoints < 2) {
        return INF;
    }

    // sort points based on x-coordinate
    sort(P->points, P->points + P->numberOfPoints, compare_points_x);

    if (DEBUG) {
        cout << "Sorting in min slope 2-d test" << endl;
        for (int i = 0; i < 100; i++) {
            cout << "Point " << i << ": (" << P->points[i]->coord[0] << ", " << P->points[i]->coord[1] << ")\n";
        }
    }

    // compute slopes of adjacent points
    SLOPE_TYPE min_slope = INF;
    for (int i = 0; i < P->numberOfPoints - 1; i++) {
        SLOPE_TYPE slope = compute_slope(P->points[i], P->points[i+1]);
        // cout << "2-d - Slope computed is " << slope << 
        // " generated by point: (" << P->points[i]->coord[0] << ", " << P->points[i]->coord[1] << ")" <<
        // " and point: (" << P->points[i+1]->coord[0] << ", " << P->points[i+1]->coord[1] << endl;
        if (slope < min_slope) {
            min_slope = slope;
            // cout << "Min slope is found at position " << i << endl;
        }
    }
    return min_slope;
}

//==============================================================================================
// Helper function for comparison in count_slopes - y at alpha
//==============================================================================================
bool compare_points_alpha(const point_t& p1, const point_t& p2, double alpha) {
    return (p1.coord[0] * alpha - p1.coord[1]) < (p2.coord[0] * alpha - p2.coord[1]);
}

struct Comparator_Alpha {
    double alpha;
    Comparator_Alpha(double f) : alpha(f) {}

    bool operator()(const point_t* p1, const point_t* p2) const {
        return compare_points_alpha(*p1, *p2, alpha);
    }
};

//==============================================================================================
// Helper function for comparison in count_slopes - y at beta
//==============================================================================================

bool compare_points_beta(const point_order_t& p1, const point_order_t& p2, double beta) {
    return (p1.point->coord[0] * beta - p1.point->coord[1]) < (p2.point->coord[0] * beta - p2.point->coord[1]);
}

struct Comparator_Beta {
    double beta;
    Comparator_Beta(double f) : beta(f) {}

    bool operator()(const point_order_t& p1, const point_order_t& p2) const {
        return compare_points_beta(p1, p2, beta);
    }
};

//==============================================================================================
// count_slopes
// Counts the number of slopes in range [alpha, beta]
// Parameters: 
//      P       - input data set
//      alpha   - lower threshold
//      beta    - upper threshold
// Return:
//      number of slopes in range [alpha, beta]
//==============================================================================================
int count_slopes(point_set_t* P, double alpha, double beta, bool adjust) {
    // sort points based on y_at_alpha value
    sort(P->points, P->points + P->numberOfPoints, Comparator_Alpha(alpha));

    // copy original points into vector with order
    vector<point_order_t> point_order;
    for (int i = 0; i < P->numberOfPoints; ++i) {
        point_order_t p_order;
        // shallow copy for simplicity
        p_order.point = P->points[i];
        p_order.order = i + 1;
        point_order.push_back(p_order);
    }

    // sort points based on y_at_beta value
    sort(point_order.begin(), point_order.end(), Comparator_Beta(beta));

    // put order of y_at_beta into vector inverted_order
    vector<int> inverted_order;
    for (int i = 0; i < point_order.size(); i++) {
        int order = point_order[i].order;
        inverted_order.push_back(order);
    }

    // perform count_inversion / sort and count on inverted order array
    int inversionCount = count_inversions(inverted_order);

    return inversionCount;
}


void test_count_slopes() {
    // read in dataset
    point_set_t *P;
    int count_brute_force = 0;
    char* filename = "a3-1000.txt";  
    P = read_points(filename);
    SLOPE_TYPE minSlope = INF;

    // brute force count slopes
    for (int i = 0; i < P->numberOfPoints; i++) {
        for (int j = i; j < P->numberOfPoints; j++) {
            SLOPE_TYPE slope = compute_slope(P->points[i], P->points[j]);
            if (slope >= -1 && slope <= 0) {
                count_brute_force++;
            }
            // min slope
            if (slope < minSlope) {
                minSlope = slope;
            }
        }
    }

    // call count slopes and min slope
    int count_slope_call = count_slopes(P, -1, 0, true, 0, 1);
    int count_slope_compare_call = count_slopes(P, -1, 0, true);
    SLOPE_TYPE min_slope_call = min_slope(P, 0, 1);
    SLOPE_TYPE min_slope_compare_call = min_slope(P);

    cout << "Slope count brute force method gives: " << count_brute_force << endl;
    cout << "Slope count function gives: " << count_slope_compare_call << endl;

    cout << "Min slope brute force method gives: " << minSlope << endl;
    cout << "Min slope function gives: " << min_slope_compare_call << endl;
}

int main() {
    test_count_slopes();
    return 0;
}