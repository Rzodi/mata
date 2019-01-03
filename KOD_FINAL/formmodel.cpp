#include "formmodel.h"

std::vector<Position> FormModel::GetRouterPositions()
{
    return routerPositions;
}
std::vector<Position> FormModel::GetEnbPositions()
{
    return enbPositions;
}
void FormModel::AddRouterPosition(Position position)
{
    routerPositions.push_back(position);
}
void FormModel::AddEnbPosition(Position position)
{
    enbPositions.push_back(position);
}

FormModel::FormModel(float alpha, float beta, float gamma, int omega, float routerRange, float enbRange, float sizeX, float sizeY, float minDemand, float maxDemand, int userCounter){
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
    UserCounter = userCounter;
}
