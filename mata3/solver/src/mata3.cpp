#include "solver.h"
#include <vector>

using namespace std;

int main() {
	solver output;
	vector<vector<float>> Alpha;
	float Beta = 5;
	vector<vector<float>> Gamma;
	vector<float> Min_demand;
	vector<float> Max_demand;
	vector<float> tmp_vector;
	float tmp=1;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 12; j++) {
			tmp_vector.push_back((float)j * 0.0001 * tmp + 0.0001);
			cout << tmp_vector[j] << " ,";
		}
		Alpha.push_back(tmp_vector);
		tmp_vector.clear();
		cout << endl;
		tmp++;
	}
	cout << endl;
	cout << endl;
	cout << endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 15; j++) {
			tmp_vector.push_back((float)j * 0.001 * tmp + 0.0001);
			cout << tmp_vector[j] << " ,";
		}
		Gamma.push_back(tmp_vector);
		tmp_vector.clear();
		cout << endl;
		tmp++;
	}

	cout << endl;
	cout << endl;
	cout << endl;

	for (int j = 0; j < 10; j++) {
				Max_demand.push_back(rand() % 9500 + 500);
				cout << Max_demand[j] << " ,";
			}

	cout << endl;
	cout << endl;
	cout << endl;

	for (int j = 0; j < 10; j++) {
				Min_demand.push_back(rand() % (int)Max_demand[j] + 100);
				cout << Min_demand[j] << " ,";
			}
	output.create_gmpl_with_data(Alpha, Beta, Gamma, Min_demand, Max_demand);
	output=output.solve_problem();
	cout << endl << endl;
	cout << "Max thorughput " << output.optimal_solution << endl;

	for (int j = 0; j < output.LTE_throughput_per_user.size(); j++) {


			cout << "userr " << output.LTE_throughput_per_user[j][0] << " bts " << output.LTE_throughput_per_user[j][1] << " "
					<< output.LTE_throughput_per_user[j][2] << endl;


	}
	for (int z = 0; z < output.WIFI_throughput_per_user.size(); z++) {


			cout << "userr " << output.WIFI_throughput_per_user[z][0] << " wifi " << output.WIFI_throughput_per_user[z][1] << " "
					<< output.WIFI_throughput_per_user[z][2] << endl;

	}

	return 0;
}

