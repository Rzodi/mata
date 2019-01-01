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
	vector< vector<float>> dist_enb; // definiujemy wektor
	vector< vector<float>> alpha; // definiujemy wektor 2 wymiarowy
	vector< vector<float>> dist_wifi; // definiujemy wektor
	vector< vector<float>> gamma; // definiujemy wektor 2 wymiarowy

private:
	int id;

	user_alpha_gamma();
	~user_alpha_gamma();
	static float user_position_demands(int users, float minDemand, float maxDemand, float x_grid, float y_grid);
	static vector<vector<float>> f_alpha(int U_count, int ENB_count, vector<float> x_enb, vector<float> y_enb, vector<float> x, vector<float> y, float R);
	static vector<vector<float>> f_gamma(int U_count, int wifi_count, vector<float> x_wifi, vector<float> y_wifi, vector<float> x, vector<float> y, float r);

	
};

