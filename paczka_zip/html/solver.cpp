#include "solver.h"

solver::solver() {
	// TODO Auto-generated constructor stub

}

solver::~solver() {
	// TODO Auto-generated destructor stub
}

string solver::exec(const char* cmd) {
	char buffer[128];
	string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}
void solver::create_gmpl_with_data(vector<vector<float>> Alpha, float Beta,
		vector<vector<float>> Gamma, vector<float> Min_demand,
		vector<float> Max_demand, int Omega) {

	ifstream source("model_template.mod", ios::binary);
	ofstream dest("model_tmp.mod", ios::binary);

	dest << source.rdbuf();

	source.close();
	dest.close();

	dest.open("model_tmp.mod", std::ios::app);

	dest << "param U_count := " << Alpha.size() << ";" << endl;
	dest << "param ENB_count := " << Alpha[0].size() << ";" << endl;
	dest << "param wifi_count := " << Gamma[0].size() << ";" << endl;
	dest << "param Omega := " << Omega << ";" << endl;
	dest << endl << "param : Alpha :=" << endl;

	for (int i = 0; i < Alpha.size(); i++) {
		for (int j = 0; j < Alpha[0].size(); j++) {
			dest << " " << i + 1 << "," << j + 1 << "     " << Alpha[i][j]
					<< endl;
		}
	}

	dest << ";" << endl << "param : Beta :=" << endl;

	for (int i = 0; i < Alpha.size(); i++) {
		for (int j = 0; j < Alpha[0].size(); j++) {
			dest << " " << i + 1 << "," << j + 1 << "     " << Beta << endl;
		}
	}

	dest << ";" << endl << "param : Gamma :=" << endl;

	for (int i = 0; i < Gamma.size(); i++) {
		for (int j = 0; j < Gamma[0].size(); j++) {
			dest << " " << i + 1 << "," << j + 1 << "     " << Gamma[i][j]
					<< endl;
		}
	}
	dest << ";" << endl << "param : Delta  :=" << endl;
	for (int i = 0; i < Min_demand.size(); i++) {
		dest << " " << i + 1 << "     " << Min_demand[i] << endl;

	}
	dest << ";" << endl << "param : Delta_2  :=" << endl;
	for (int i = 0; i < Max_demand.size(); i++) {
		dest << " " << i + 1 << "     " << Max_demand[i] << endl;

	}
	dest << ";" << endl << "end;" << endl;

	dest.close();

}

solver solver::solve_problem() {
	string line;
	string tmp_line;
	ifstream cbc_file("output.txt");
	vector<string> lines_numbers;
	string tmp_value;
	smatch regex_match;
	solver output;
	vector<float> tmp_vector_lte;
	vector<float> tmp_vector_wifi;
	const char* cmd;
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;

	cmd = "cbc model_tmp.mod% -solve -solu output.txt";
	output.unexpected_error = false;
	t1 = high_resolution_clock::now();
	solver::exec(cmd);
	t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	output.time_in_microseconds = (float) duration;
	if (cbc_file.is_open()) {
		getline(cbc_file, line);
		if (line.find("Infeasible") < 10000) {
			output.is_problem_feasible = false;
		} else {
			output.is_problem_feasible = true;
			regex_search(line, regex_match,
					regex("[+-]?\\d+(\\.\\d+)?([Ee][+-]?\\d+)?"));
			tmp_value = regex_match[0].str();
			tmp_value = tmp_value.substr(1, tmp_value.length());
			output.optimal_solution = stof(tmp_value);
			while (getline(cbc_file, line)) {
				tmp_line = line;
				while (regex_search(tmp_line, regex_match,
						regex("[+-]?\\d+(\\.\\d+)?([Ee][+-]?\\d+)?"))) {
					lines_numbers.push_back(regex_match[0].str());
					tmp_line = regex_match.suffix().str();
				}
				lines_numbers.clear();
				if (line.find("X[") < 10000 && stof(lines_numbers[3]) > 0) {
					tmp_vector_lte.push_back(stof(lines_numbers[1]));
					tmp_vector_lte.push_back(stof(lines_numbers[2]));
					tmp_vector_lte.push_back(stof(lines_numbers[3]));
					output.LTE_throughput_per_user.push_back(tmp_vector_lte);
					tmp_vector_lte.clear();

				}

				if (line.find("Y[") < 10000 && stof(lines_numbers[3]) > 0) {
					tmp_vector_wifi.push_back(stof(lines_numbers[1]));
					tmp_vector_wifi.push_back(stof(lines_numbers[2]));
					tmp_vector_wifi.push_back(stof(lines_numbers[3]));
					output.WIFI_throughput_per_user.push_back(tmp_vector_wifi);
					tmp_vector_wifi.clear();

				}

			}
		}
		cbc_file.close();

	}

	else
		output.unexpected_error = true;

//	for (int j = 0; j < output.LTE_throughput_per_user.size(); j++) {
//		for (int k = 0; k < output.LTE_throughput_per_user[j].size(); k++) {
//
//			cout << "userr " << j + 1 << " bts " << k + 1 << " "
//					<< output.LTE_throughput_per_user[j][k] << endl;
//		}
//
//	}
//	for (int z = 0; z < output.WIFI_throughput_per_user.size(); z++) {
//		for (int x = 0; x < output.WIFI_throughput_per_user[x].size(); x++) {
//
//			cout << "userr " << z + 1 << " wifi " << x + 1 << " "
//					<< output.WIFI_throughput_per_user[z][x] << endl;
//		}
//
//	}
	return output;
}

