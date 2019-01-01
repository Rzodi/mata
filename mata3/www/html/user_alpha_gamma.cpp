#include "user_alpha_gamma.h"

user_alpha_gamma::user_alpha_gamma() {
	// TODO Auto-generated constructor stub

}

user_alpha_gamma::~user_alpha_gamma() {
	// TODO Auto-generated destructor stub
}

vector<vector<float>> user_position_demands(int users, float minDemand, float maxDemand, float x_grid, float y_grid)
{
	float mediumDemand = (maxDemand + minDemand)/2;
	vector< vector<float>> my_vector;
	for (int i = 0; i < users; i++) {
		float randThroughputMin = minDemand + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxDemand - minDemand));
		float randThroughputMax = maxDemand + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxDemand - mediumDemand));
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / x_grid));
		float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / y_grid));
		my_vector[0][i]= x;
		my_vector[1][i] = y;
		my_vector[2][i] = randThroughputMax;
		my_vector[3][i] = randThroughputMin;
	}
	return my_vector;
}

vector<vector<float>> f_alpha(int U_count, int ENB_count, vector<float> x_enb, vector<float> y_enb, vector<float> x, vector<float> y, float R, float worst_alpha) // definicja funkcji dwuwymiarowej na vector'ze
{
	float best_alpha = 0.001; // najlepsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
	//int worst_alpha = 25; // najgorsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()

	vector< vector<float> > dist_enb(U_count, vector<float>(ENB_count)); // definiujemy wektor
	vector< vector<float> > alpha(U_count, vector<float>(ENB_count)); // definiujemy wektor 2 wymiarowy

	for (int u = 0; u < U_count; u++)	// liczymy odleglosc i alpha'e przelatujac po kazdym userze i kazdym enb
	{
		for (int e = 0; e < ENB_count; e++) // czytaj powyzszy komentarz
		{
			dist_enb[u][e] = sqrt(pow(x_enb[e] - x[u], 2) + pow(y_enb[e] - y[u], 2)); // liczenie odleglosci usera od ENB w ukladzie kartezjanskim
			alpha[u][e] = (((worst_alpha - best_alpha) / R) * dist_enb[u][e]) + best_alpha; // liczenie alpha'y z funkcji liniowej w zaleznosci od odleglosci od ENB
		}
	}
	return alpha;
}

vector<vector<float>> f_gamma(int U_count, int wifi_count, vector<float> x_wifi, vector<float> y_wifi, vector<float> x, vector<float> y, float r, float worst_gamma) // definicja funkcji dwuwymiarowej na vector'ze
{
	float best_gamma = 0.001; // najlepsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
	//int worst_gamma = 1; // najgorsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()

	vector< vector<float> > dist_wifi(U_count, vector<float>(wifi_count)); // definiujemy wektor
	vector< vector<float> > gamma(U_count, vector<float>(wifi_count)); // definiujemy wektor 2 wymiarowy

	for (int u = 0; u < U_count; u++)	// liczymy odleglosc i gamma'e przelatujac po kazdym userze i kazdym routerze wifi
	{
		for (int w = 0; w < wifi_count; w++) // czytaj powyzszy komentarz
		{
			dist_wifi[u][w] = sqrt(pow(x_wifi[w] - x[u], 2) + pow(y_wifi[w] - y[u], 2)); // liczenie odleglosci usera od routera WIFI w ukladzie kartezjanskim
			gamma[u][w] = (((worst_gamma - best_gamma) / r) * dist_wifi[u][w]) + best_gamma; // liczenie gamma'y z funkcji liniowej w zaleznosci od odleglosci od routera wifi
		}
	}
	return gamma;
}


int main()
{
	int U_count = 3, ENB_count = 2, wifi_count = 2;
	float R = 3, r = 1;

	vector<float> x_enb(ENB_count); // definiujemy wektor - wspolrzedna X dla ENB
	vector<float> y_enb(ENB_count); // definiujemy wektor - wspolrzedna Y dla ENB
	vector<float> x_wifi(wifi_count); // definiujemy wektor - wspolrzedna X dla WIFI
	vector<float> y_wifi(wifi_count); // definiujemy wektor - wspolrzedna Y dla WIFI


	for (int ne = 0; ne < ENB_count; ne++)
	{
		cout << "Please set coordinates (X,Y) of ENB: " << endl;
		cin >> x_enb[ne] >> y_enb[ne];
		cout << endl;
	}

	for (int nw = 0; nw < wifi_count; nw++)
	{
		cout << "Please set coordinates (X,Y) of WIFI router: " << endl;
		cin >> x_wifi[nw] >> y_wifi[nw];
		cout << endl;
	}

	vector<vector<float>> my_vector = user_position_demands(U_count, 100, 1000, 20, 20);

	for (int u = 0; u < 4; u++)
	{
		for (int e = 0; e < U_count; e++) 
		{
			cout << "my_vector[" << u + 1 << "][" << e + 1 << "] = " << my_vector[u][e] << endl;
		}
		cout << endl;
	}

	vector<vector<float>> vect_alpha = f_alpha(U_count, ENB_count, x_enb, y_enb, my_vector[0], my_vector[1], R, 25);

	for (int u = 0; u < U_count; u++)	// liczymy odleglosc i alpha'e przelatujac po kazdym userze i kazdym enb
	{
		for (int e = 0; e < ENB_count; e++) // czytaj powyzszy komentarz
		{
			cout << "alpha[" << u + 1 << "][" << e + 1 << "] = " << vect_alpha[u][e] << endl;
		}
		cout << endl;
	}

	vector<vector<float>> vect_gamma = f_gamma(U_count, wifi_count, x_wifi, y_wifi, my_vector[0], my_vector[1], r, 1); // przypisanie wartosci funkcji f_alpha do vector'a vect_gamma

	for (int u = 0; u < U_count; u++)	// liczymy odleglosc i gamma'e przelatujac po kazdym userze i kazdym routerze wifi
	{
		for (int w = 0; w < wifi_count; w++) // czytaj powyzszy komentarz
		{
			cout << "gamma[" << u + 1 << "][" << w + 1 << "] = " << vect_gamma[u][w] << endl;
		}
		cout << endl;
	}

	return 0;
}