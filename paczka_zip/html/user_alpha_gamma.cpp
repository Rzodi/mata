#include "user_alpha_gamma.h"

user_alpha_gamma::user_alpha_gamma() {
	// TODO Auto-generated constructor stub

}

user_alpha_gamma::~user_alpha_gamma() {
	// TODO Auto-generated destructor stub
}

vector<vector<float>> user_alpha_gamma::user_position_demands(int users, float minDemand, float maxDemand, float x_grid, float y_grid)
{
	float mediumDemand = (maxDemand + minDemand)/2;
	vector<vector<float>> my_vector(4, vector<float> (users));

	for (int i = 0; i < users; i++) {
		float randThroughputMin = minDemand + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxDemand - minDemand));
		float randThroughputMax = maxDemand + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / (maxDemand - mediumDemand));
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / x_grid));
		float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / y_grid));

		my_vector[0][i] = x;
		my_vector[1][i] = y;
		my_vector[2][i] = randThroughputMax;
		my_vector[3][i] = randThroughputMin;
	}
	return my_vector;
}

vector<vector<float>> user_alpha_gamma::f_alpha(int U_count, int ENB_count, vector<Position> enb_positions, vector<float> x, vector<float> y, float R, float best_alpha, float beta, float omega) // definicja funkcji dwuwymiarowej na vectorze
{
    //float best_alpha = 0.001; // najlepsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
    //int worst_alpha = 25; // najgorsza alpha, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
    float throuput_worst_alpha = 10; // przepustowosc dla najgorszej alphy
    float worst_alpha = (omega - beta) / throuput_worst_alpha;
 
    vector< vector<float> > dist_enb(U_count, vector<float>(ENB_count)); // definiujemy wektor
    vector< vector<float> > alpha(U_count, vector<float>(ENB_count)); // definiujemy wektor 2 wymiarowy
 
    for (int u = 0; u < U_count; u++)   // liczymy odleglosc i alpha'e przelatujac po kazdym userze i kazdym enb
    {
        for (int e = 0; e < ENB_count; e++) // czytaj powyzszy komentarz
        {
            dist_enb[u][e] = sqrt(pow(enb_positions[e].X - x[u], 2) + pow(enb_positions[e].Y - y[u], 2)); // liczenie odleglosci usera od ENB w ukladzie kartezjanskim
            alpha[u][e] = (((worst_alpha - best_alpha) / R) * dist_enb[u][e]) + best_alpha; // liczenie alpha'y z funkcji liniowej w zaleznosci od odleglosci od ENB
            if (alpha[u][e] > worst_alpha) // jesli user poza zasiegiem, to alpha = 26
            {
                alpha[u][e] = 26;
            }
        }
    }
    return alpha;
}
 
vector<vector<float>> user_alpha_gamma::f_gamma(int U_count, int wifi_count, vector<Position> router_positions, vector<float> x, vector<float> y, float r, float best_gamma) // definicja funkcji dwuwymiarowej na vector'ze
{
    //float best_gamma = 0.001; // najlepsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
    //int worst_gamma = 1; // najgorsza gamma, uzywana do wyznaczenia rownania funkcji liniowej - patrz komentarz po main()
    float throuput_worst_gamma = 10;
    float worst_gamma = 10 / throuput_worst_gamma;
 
    vector< vector<float> > dist_wifi(U_count, vector<float>(wifi_count)); // definiujemy wektor
    vector< vector<float> > gamma(U_count, vector<float>(wifi_count)); // definiujemy wektor 2 wymiarowy
 
    for (int u = 0; u < U_count; u++)   // liczymy odleglosc i gamma'e przelatujac po kazdym userze i kazdym routerze wifi
    {
        for (int w = 0; w < wifi_count; w++) // czytaj powyzszy komentarz
        {
            dist_wifi[u][w] = sqrt(pow(router_positions[w].X - x[u], 2) + pow(router_positions[w].Y - y[u], 2)); // liczenie odleglosci usera od routera WIFI w ukladzie kartezjanskim
            gamma[u][w] = (((worst_gamma - best_gamma) / r) * dist_wifi[u][w]) + best_gamma; // liczenie gamma'y z funkcji liniowej w zaleznosci od odleglosci od routera wifi
            if (gamma[u][w] > worst_gamma) // jesli user poza zasiegiem, to gamma = 1
            {
                gamma[u][w] = 1;
            }
        }
    }
    return gamma;
}


