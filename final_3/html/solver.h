#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <chrono>

using namespace std;
using namespace std::chrono;

class solver {
public:
	vector<vector<float>> LTE_throughput_per_user;
	vector<vector<float>> WIFI_throughput_per_user;
	float optimal_solution;
	bool is_problem_feasible;
	bool unexpected_error;
	float time_in_microseconds;
	solver();
	virtual ~solver();
	static string exec(const char* cmd);
	static void create_gmpl_with_data(vector<vector<float>> Alpha, float Beta,
			vector<vector<float>> Gamma, vector<float> Min_demand,
			vector<float> Max_demand, int Omega);
	solver solve_problem();
};

#endif /* SOLVER_H_ */
