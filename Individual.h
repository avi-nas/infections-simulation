#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <random>

class Individual {
public:
    enum State { Susceptible, Infected, Recovered, Vaccinated };

    Individual();
    void updateState(double infectionRate, double recoveryRate, double mutationRate);
    void vaccinate();
    bool isHighRisk() const;
    int getSocialActivityLevel() const;
    State getState() const;
    bool isVaccinated() const;

private:
    State state;
    int daysInfected;
    int socialActivityLevel;
    bool isHighRiskFlag;
    bool isInfectedWithMutatedStrain;
};

#endif // INDIVIDUAL_H
