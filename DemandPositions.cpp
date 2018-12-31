// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include<fstream>

using namespace std;

class User {
public:
	User(int id_, float x, float y, float min, float max) {
		id = id_;
		x_position = x;
		y_position = y;
		min_throughput = min;
		max_throughput = max;
	}

	int getId() const {
		return id;
	}


	float x_position;
	float y_position;
	float min_throughput;
	float max_throughput;

private:
	int id;
};
int main()
{
	ofstream file;
	int users;
	cout << "wprowadz liczbe uzytkownikow " << endl;
	cin >> users;
	int x_grid, y_grid;
	cout << "wprowadz rozmiary pola, lysego pola, chyba ze masz plan: " << endl;
	cin >> x_grid >> y_grid;

	int maxThrough2 = 1000;
	int maxThrough1 = 500;
	int minThrough = 100;

	std::vector<User> my_vector;
	for (int i = 0; i < users; i++) {
		float randThroughputMin = minThrough + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxThrough1 - minThrough));
		float randThroughputMax = maxThrough1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxThrough2 - maxThrough1));
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / x_grid));
		float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / y_grid));
		User tmp(i, x, y, randThroughputMin, randThroughputMax);
		my_vector.emplace_back(tmp);
	}


	file.open("model_2.mod", ios::app);
	if (file.is_open()) {
		file << "/* Input data */" << "\n" << "data;" << "\n" << "\n";
		file << "param : wspolrzedne :=" << "\n";
		for (int i = 0; i < my_vector.size(); i++) {			
			file << i + 1 << " " << i + 1 << "\t" << my_vector.at(i).x_position << "\n";
			file << i + 1 << " " << i + 2 << "\t" << my_vector.at(i).y_position << "\n";
		}
		file << ";" << "\n";
		file << "param : minmaxthroughput :=" << "\n";
		for (int i = 0; i < my_vector.size(); i++) {
			file << i + 1 << " " << i + 1 << "\t" << my_vector.at(i).min_throughput << "\n";
			file << i + 1 << " " << i + 2 << "\t" << my_vector.at(i).max_throughput << "\n";
		}
		file << ";" << "\n" << "\n" << "endl;" << "\n";
		file.close();
	}
	


}