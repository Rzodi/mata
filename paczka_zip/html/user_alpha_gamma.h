#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <algorithm>
 
using namespace std;
 
class user_alpha_gamma {
public:
    float x_position;
    float y_position;
    float min_throughput;
    float max_throughput;
    vector<vector<float>> dist_enb; // definiujemy wektor
    vector<vector<float>> alpha; // definiujemy wektor 2 wymiarowy
    vector<vector<float>> dist_wifi; // definiujemy wektor
    vector<vector<float>> gamma; // definiujemy wektor 2 wymiarowy
    vector<vector<float>> user_position_demands(int users, float minDemand, float maxDemand, float x_grid, float y_grid, vector<Position> enb_positions, vector<Position> router_positions, float R, float r);
    vector<vector<float>> f_alpha(int U_count, int ENB_count, vector<Position> enb_positions, vector<float> x, vector<float> y, float R, float best_alpha, float beta, float omega);
    vector<vector<float>> f_gamma(int U_count, int wifi_count, vector<Position> router_positions, vector<float> x, vector<float> y, float r, float best_gamma);
 
 
    user_alpha_gamma();
    ~user_alpha_gamma();
private:
    int id;
 
 
};

