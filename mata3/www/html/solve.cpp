//kompilowanie: sudo g++ solve.cpp -o solve.cgi -lcgicc

#include <iostream>
#include <string>

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "solver.h"

using namespace std;
using namespace cgicc;

class Position{
    public:
    float X;
    float Y; 

    Position(float x_position, float y_position){
        X = x_position;
        Y = y_position;
    }
};


class FormModel{
    private:        
        std::vector<Position> routerPositions;
        std::vector<Position> enbPositions;
        std::vector<int> userDemands;
    public:
        float Alpha;
        float Beta;
        float Gamma;
        float Omega;        
        float RouterRange;
        float EnbRange;
        float SizeX;
        float SizeY;
        float MinDemand;
        float MaxDemand;

        std::vector<Position> GetRouterPositions()
        {
            return routerPositions;
        }
        std::vector<Position> GetEnbPositions()
        {
            return enbPositions;
        }
        std::vector<int> GetUserDemands()
        {
            return userDemands;
        }
        void AddRouterPosition(Position position)
        {
            routerPositions.push_back(position);
        }
        void AddREnbPosition(Position position)
        {
            enbPositions.push_back(position);
        }
        void AddUserDemands(int demand)
        {
            userDemands.push_back(demand);
        }

    FormModel(float alpha, float beta, float gamma, float omega, float routerRange, float enbRange, float sizeX, float sizeY, float minDemand, float maxDemand){
        Alpha = alpha;
        Beta = beta;
        Gamma = gamma;
        Omega = omega;
        RouterRange = routerRange;
        EnbRange = enbRange;
        SizeX = sizeX;
        SizeY = sizeY;
        MinDemand = minDemand;
        MaxDemand = maxDemand;
    }
};

FormModel ReciveData()
{
    Cgicc formData;
        

    FormModel model (
          std::stof(formData("alpha"))
        , std::stof(formData("beta"))
        , std::stof(formData("gamma"))
        , std::stof(formData("omega"))
        , std::stof(formData("router_range"))
        , std::stof(formData("enb_range"))
        , std::stof(formData("size_x"))
        , std::stof(formData("size_y"))
        , std::stof(formData("minimum_user_demand"))
        , std::stof(formData("maximum_user_demand")));


    int userCounter = std::stoi(formData("user_counter"));  
    int routerCounter = std::stoi(formData("router_counter"));  
    int enbCounter = std::stoi(formData("enb_counter"));  

    for (int i = 0; i < routerCounter; ++i)
    {
        Position position (std::stof(formData("routers[" + std::to_string(i) + "].x")), std::stof(formData("routers[" + std::to_string(i) + "].y")));
        model.AddRouterPosition(position);
    }
    for (int i = 0; i < enbCounter; ++i)
    {
        model.AddREnbPosition(Position (std::stof(formData("enbs[" + std::to_string(i) + "].x")), std::stof(formData("enbs[" + std::to_string(i) + "].y"))));
    }
    
    return model;
}

void sendJson(FormModel model){
        cout << "Content-Type: application/json\r\n\r\n";
        cout << "{";
        cout << "\"enb_range\": " + to_string(model.EnbRange) + ",";
        cout << "\"router_range\": " + to_string(model.RouterRange) + ",";
        cout << "\"is_problem_feasible\": true,";
        cout << "\"unexpected_error\": false,";
        cout << "\"size_x\": " + to_string(model.SizeX) + ",";
        cout << "\"size_y\": " + to_string(model.SizeY) + ",";
        cout << "\"positions\":{";
        cout << "\"routers\":[";
        
        int sizeOfPositions = model.GetRouterPositions().size();

        for(int i =0; i < sizeOfPositions; i++){
            cout << "{\"x\":" + to_string(model.GetRouterPositions()[i].X) + ",\"y\":" + to_string(model.GetRouterPositions()[i].Y) + "}";
            if(i != sizeOfPositions -1){
                cout << ",";
            }
        }
        cout << "],";
        cout << "\"enbs\":[";

        sizeOfPositions = model.GetEnbPositions().size();

        for(int i =0; i < sizeOfPositions; i++){
            cout << "{\"x\":" + to_string(model.GetEnbPositions()[i].X) + ",\"y\":" + to_string(model.GetEnbPositions()[i].Y) + "}";
            if(i != sizeOfPositions){
                cout << ",";
            }
        }
        cout << "{\"x\":220,\"y\":300}";
        cout << "],";
        //tutaj wrzucenie danych odpowiedzi solvera oraz informacji wylosowanych o użytownikach
        cout << "\"users\" : [";        
        cout << "{\"x\": 180, \"y\": 55, \"min_demand\" : 1, \"max_demand\" : 4, \"wifi\" : 2, \"lte\" : 3},";
        cout << "{\"x\": 430, \"y\": 235, \"min_demand\" : 129, \"max_demand\" : 2200, \"wifi\" : 32, \"lte\" : 1444}";

        cout << "]}";
        cout << "}";
}


int main( int argc, char ** argv, char ** envp )
{
    try
    {
        //odebranie danych z formularza
        FormModel model = ReciveData();

        //tutaj należy dodać wywołanie randomizowania użytkowników, obliczanie współczynników, tworzenie i odpalanie modelu

        //zbiórka danych z wowałania optymalizatora do jakiejś klasy
        //result następnie ten obiekt zostaje przekazany do funkcji sendJson
	//output.create_gmpl_with_data(Alpha, Beta, Gamma, Min_demand, Max_demand, Omega);
	//output=output.solve_problem();
        sendJson(model);    
    }
    catch( exception & e )
    {
        cout << "Wystąpił błąd!: " << e.what() << endl;
    }
    
    return 0;
}
