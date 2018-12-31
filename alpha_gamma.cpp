#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <algorithm>
#include <vector>

using namespace std;

vector<vector<float>> f_alpha(int U_count, int ENB_count, vector<float> x_enb, vector<float> y_enb, vector<float> x, vector<float> y, float R) // definicja funkcji dwuwymiarowej na vector'ze
{
	float best_alpha = 0.001; // najlepsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
	int worst_alpha = 25; // najgorsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()

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

vector<vector<float>> f_gamma(int U_count, int wifi_count, vector<float> x_wifi, vector<float> y_wifi, vector<float> x, vector<float> y, float r) // definicja funkcji dwuwymiarowej na vector'ze
{
	float best_gamma = 0.001; // najlepsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
	int worst_gamma = 1; // najgorsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()

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
	int U_count; // ilosc userow
	int ENB_count; // ilosc ENB
	int wifi_count; // ilosc routerow wifi
	int numb_enb = 0, numb_wifi = 0; // zmienne pomocnicze do petli
	
	float R; // zasieg ENB
	float r; // zasieg routera wifi

	cout << "Please set a number of users: " << endl;
	cin >> U_count;
	cout << endl;

	cout << "Please set a number of ENBs: " << endl;
	cin >> ENB_count;
	cout << endl;

	cout << "Please set a number of WIFI routers: " << endl;
	cin >> wifi_count;
	cout << endl;

	cout << "Please set a range of ENB: " << endl;
	cin >> R;
	cout << endl;

	cout << "Please set a range of WIFI: " << endl;
	cin >> r;
	cout << endl;

	vector<float> x_enb(ENB_count); // definiujemy wektor - wspolrzedna X dla ENB
	vector<float> y_enb(ENB_count); // definiujemy wektor - wspolrzedna Y dla ENB
	vector<float> x_wifi(wifi_count); // definiujemy wektor - wspolrzedna X dla WIFI
	vector<float> y_wifi(wifi_count); // definiujemy wektor - wspolrzedna Y dla WIFI
	vector<float> x(U_count); // definiujemy wektor - wspolrzedna X dla usera
	vector<float> y(U_count); // definiujemy wektor - wspolrzedna Y dla usera

	for (int nu = 0; nu < U_count; nu++)
	{
		cout << "Please set coordinates (X,Y) of user: " << endl;
		cin >> x[nu] >> y[nu];
		cout << endl;
	}

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


	vector<vector<float>> vect_alpha = f_alpha(U_count, ENB_count, x_enb, y_enb, x, y, R); // przypisanie wartosci funkcji f_alpha do vector'a vect_alpha

	for (int u = 0; u < U_count; u++)	// liczymy odleglosc i alpha'e przelatujac po kazdym userze i kazdym enb
	{
		for (int e = 0; e < ENB_count; e++) // czytaj powyzszy komentarz
		{ 
			cout << "alpha[" << u + 1 << "][" << e + 1 << "] = " << vect_alpha[u][e] << endl;
		}
		cout << endl;
	}


	vector<vector<float>> vect_gamma = f_gamma(U_count, wifi_count, x_wifi, y_wifi, x, y, r); // przypisanie wartosci funkcji f_alpha do vector'a vect_gamma

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


/* funkcja liniowa do obliczania alfy w zaleznosci od odleglosci od ENB
y = ax + b
26 = a * R + b //najgorsza wartosc alfa = 26 dla odleglosci rownej R, czyli zasiegu ENB
0.01 = a * 0 + b => b = 0.01 //najlepsza wartosc alfa = 0.01 dla odleglosci rownej 0

26 = a * R + 0.01
a = 25.99/R

y = 25.99/R * distans_od_ENB + 0.01 //funkcja liniowa
*/

/* funkcja liniowa do obliczania gammy w zaleznosci od odleglosci od routera WIFI
y = ax + b
1 = a * r + b //najgorsza wartosc gamma = 1 dla odleglosci rownej r, czyli zasiegu routera WIFI
0.001 = a * 0 + b => b = 0.001 //najlepsza wartosc gamma = 0.001 dla odleglosci rownej 0

1 = a * r + 0.001
a = 0.999/r

y = 0.999/r * distans_od_rotera_WIFI + 0.001 //funkcja liniowa
*/

/* funkcja uzywajac klasy vector
vector<int> makeVector(int aNumber)
{
   return 1234567890;
}

wywolanie funkcji
vector<int> vect = makeVector(number);
*/