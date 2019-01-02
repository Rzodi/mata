//kompilowanie: sudo g++ solve.cpp -o solve.cgi -lcgicc

#include <iostream>
#include <string>

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "solver.cpp"
//#include "position.h"
#include "position.cpp"
//#include "formmodel.h"
#include "formmodel.cpp"
#include "user_alpha_gamma.cpp"

using namespace std;
using namespace cgicc;

FormModel ReciveData()
{
    Cgicc formData;
        

    FormModel model (
          std::stof(formData("alpha"))
        , std::stof(formData("beta"))
        , std::stof(formData("gamma"))
        , std::stoi(formData("omega"))
        , std::stof(formData("router_range"))
        , std::stof(formData("enb_range"))
        , std::stof(formData("size_x"))
        , std::stof(formData("size_y"))
        , std::stof(formData("minimum_user_demand"))
        , std::stof(formData("maximum_user_demand"))
        , std::stof(formData("user_counter")));

    int routerCounter = std::stoi(formData("router_counter"));  
    int enbCounter = std::stoi(formData("enb_counter"));  

    for (int i = 0; i < routerCounter; ++i)
    {
        Position position (std::stof(formData("routers[" + std::to_string(i) + "].x")), std::stof(formData("routers[" + std::to_string(i) + "].y")));
        model.AddRouterPosition(position);
    }
    for (int i = 0; i < enbCounter; ++i)
    {
        model.AddEnbPosition(Position (std::stof(formData("enbs[" + std::to_string(i) + "].x")), std::stof(formData("enbs[" + std::to_string(i) + "].y"))));
    }
    
    return model;
}

inline const char * const BoolToString(bool b)
{
  return b ? "true" : "false";
}



//vector<vector<float>> userVector, solver outputFromModel - jako dodatkowy argument oraz
void sendJson(FormModel model, vector<vector<float>> userVector, solver outputFromModel){

        cout << "Content-Type: application/json\r\n\r\n";
        cout << "{";
        cout << "\"enb_range\": " + to_string(model.EnbRange) + ",";
        cout << "\"router_range\": " + to_string(model.RouterRange) + ",";
        cout << "\"time_in_microseconds\": " + to_string(outputFromModel.time_in_microseconds) + ",";
        if(outputFromModel.is_problem_feasible){
            cout << "\"is_problem_feasible\": true,";
        }
        else{
            cout << "\"is_problem_feasible\": false,";
        }

        if(outputFromModel.unexpected_error){
            cout << "\"unexpected_error\": true,";
        }
        else{
            cout << "\"unexpected_error\": false,";
        }
    
        
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
            if(i != sizeOfPositions -1){
                cout << ",";
            }
        }

        cout << "],";
        //tutaj wrzucenie danych odpowiedzi solvera oraz informacji wylosowanych o użytownikach
        cout << "\"users\" : [";   

        //poniższy kod działa w oparciu o tablicę userVector od Wojtka oraz obiekt od Łukasza
        float lteThroughput = 0;
        float wifiThroughput = 0;
 
        for(int i = 0; i < userVector.size(); i++){
            for(int z = 0; z < outputFromModel.LTE_throughput_per_user.size(); z++ ){
                if(outputFromModel.LTE_throughput_per_user[z][1] == i + 1){
                    lteThroughput = outputFromModel.LTE_throughput_per_user[z][2];
                    break;
                }
            }
            for(int x = 0; x < outputFromModel.WIFI_throughput_per_user.size(); x++ ){
                if(outputFromModel.WIFI_throughput_per_user[x][1] == i + 1){
                    wifiThroughput = outputFromModel.WIFI_throughput_per_user[x][2];
                    break;
                }
            }
            cout << "{\"x\": " + to_string(userVector[i][0]) + ", \"y\": " + to_string(userVector[i][1]) + ", \"min_demand\" : " + to_string(userVector[i][2]) + ", \"max_demand\" : " + to_string(userVector[i][3]) + ", \"wifi\" : "+to_string(wifiThroughput) +", \"lte\" : "+to_string(lteThroughput) +"}";
            if(i != userVector.size() -1){
                cout << ",";
            }
        }
                //do usunięcia 
        //cout << "{\"x\": 180, \"y\": 55, \"min_demand\" : 1, \"max_demand\" : 4, \"wifi\" : 2, \"lte\" : 3},";
        //cout << "{\"x\": 430, \"y\": 235, \"min_demand\" : 129, \"max_demand\" : 2200, \"wifi\" : 32, \"lte\" : 1444}";

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
        user_alpha_gamma calculator;
        vector<vector<float>> userVector = calculator.user_position_demands(model.UserCounter, model.MinDemand, model.MaxDemand, model.SizeX, model.SizeY);
        vector<vector<float>> alphaVector = calculator.f_alpha(model.UserCounter, model.GetEnbPositions().size(), model.GetEnbPositions(), userVector[0], userVector[1], model.EnbRange, model.Alpha, model.Beta, model.Omega);
        vector<vector<float>> gammaVector = calculator.f_gamma(model.UserCounter, model.GetRouterPositions().size(), model.GetRouterPositions(), userVector[0], userVector[1], model.RouterRange, model.Gamma);
            
        //zbiórka danych z wowałania optymalizatora do jakiejś klasy
        //result następnie ten obiekt zostaje przekazany do funkcji sendJson

        solver output = solver();
        output.create_gmpl_with_data(alphaVector, model.Beta, gammaVector, userVector[2], userVector[3], model.Omega);
        output = output.solve_problem();

        // tutaj dochodzi również przekazanie obiektu output oraz tablicy od Wojtka
        //, userVector, output
        sendJson(model, userVector, output);    


    }
    catch( exception & e )
    {
        cout << "Wystąpił błąd!: " << e.what() << endl;
    }
    
    return 0;
}
