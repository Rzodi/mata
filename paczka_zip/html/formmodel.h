#ifndef FORMMODEL_H_
#define FORMMODEL_H_

#include <iostream>
#include <string>

using namespace std;


class FormModel{
    private:        
        std::vector<Position> routerPositions;
        std::vector<Position> enbPositions;
    public:
        float Alpha;
        float Beta;
        float Gamma;
        int Omega;        
        float RouterRange;
        float EnbRange;
        float SizeX;
        float SizeY;
        float MinDemand;
        float MaxDemand;
        int UserCounter;

        std::vector<Position> GetRouterPositions();
        std::vector<Position> GetEnbPositions();
        void AddRouterPosition(Position position);
        void AddEnbPosition(Position position);

    	FormModel(float alpha, float beta, float gamma, int omega, float routerRange, float enbRange, float sizeX, float sizeY, float minDemand, float maxDemand, int userCounter);
};

#endif /* FORMMODEL_H_ */
