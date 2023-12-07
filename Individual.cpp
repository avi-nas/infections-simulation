#include "Individual.h"

Individual::Individual() : state(Susceptible), daysInfected(0), 
                           socialActivityLevel(rand() % 10), isHighRiskFlag(rand() % 2 == 0), 
                           isInfectedWithMutatedStrain(false) {}

void Individual::updateState(double infectionRate, double recoveryRate, double mutationRate) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    if (state == Infected) {
        daysInfected++;
        if (dis(gen) < recoveryRate) {
            state = Recovered;
            isInfectedWithMutatedStrain = false; // Reset mutation flag upon recovery
        }
    } else if (state == Susceptible) {
        if (dis(gen) < infectionRate) {
            state = Infected;
            isInfectedWithMutatedStrain = dis(gen) < mutationRate; // Check for mutation
        }
    }
    // Recovered individuals remain immune
}

void Individual::vaccinate() {
    if (state == Susceptible) {
        state = Vaccinated;
    }
}

bool Individual::isHighRisk() const {
    return isHighRiskFlag;
}

int Individual::getSocialActivityLevel() const {
    return socialActivityLevel;
}

Individual::State Individual::getState() const {
    return state;
}

bool Individual::isVaccinated() const {
    return state == Vaccinated;
}
