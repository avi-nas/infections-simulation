#include "Population.h"
#include <iostream>

Population::Population(int size, double interactionRate, double initialInfectionRate, 
                       double recoveryRate, double mutationRate)
: interactionRate(interactionRate), recoveryRate(recoveryRate), mutationRate(mutationRate) {
    std::random_device rd;
    gen = std::mt19937(rd());  // Initialize the random number generator here

    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < size; ++i) {
        Individual newIndividual;
        if (dis(gen) < initialInfectionRate) {
            newIndividual.updateState(1.0, recoveryRate, mutationRate); // Initial infection
        }
        individuals.push_back(newIndividual);
    }
}

int Population::getSusceptibleCount() const {
    int count = 0;
    for (const Individual& individual : individuals) {
        if (individual.getState() == Individual::Susceptible) {
            ++count;
        }
    }
    return count;
}

int Population::getInfectedCount() const {
    int count = 0;
    for (const Individual& individual : individuals) {
        if (individual.getState() == Individual::Infected) {
            ++count;
        }
    }
    return count;
}

int Population::getRecoveredCount() const {
    int count = 0;
    for (const Individual& individual : individuals) {
        if (individual.getState() == Individual::Recovered) {
            ++count;
        }
    }
    return count;
}

int Population::getVaccinatedCount() const {
    int count = 0;
    for (const Individual& individual : individuals) {
        if (individual.getState() == Individual::Vaccinated) {
            ++count;
        }
    }
    return count;
}

void Population::vaccinateHighRisk(double vaccinationRate) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (Individual& individual : individuals) {
        if (!individual.isVaccinated() && individual.isHighRisk() && dis(gen) < vaccinationRate) {
            individual.vaccinate();
        }
    }
}

void Population::vaccinateSociallyActive(double vaccinationRate, int threshold) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (Individual& individual : individuals) {
        if (individual.getSocialActivityLevel() > threshold && dis(gen) < vaccinationRate) {
            individual.vaccinate();
        }
    }
}

void Population::updatePopulationState(double infectionRate) {
    vaccinateHighRisk(0.1);  // Example vaccination rate

    for (Individual& individual : individuals) {
        individual.updateState(infectionRate, recoveryRate, mutationRate);
    }
}

bool Population::noInfections() const {
    for (const Individual& individual : individuals) {
        if (individual.getState() == Individual::Infected) {
            return false; // There are still infected individuals
        }
    }
    return true; // No infected individuals left
}

void Population::printPopulationStates() const {
    int susceptible = 0, infected = 0, recovered = 0, vaccinated = 0;
    for (const Individual& individual : individuals) {
        switch (individual.getState()) {
            case Individual::Susceptible: ++susceptible; break;
            case Individual::Infected: ++infected; break;
            case Individual::Recovered: ++recovered; break;
            case Individual::Vaccinated: ++vaccinated; break;
        }
    }
    std::cout << "Susceptible: " << susceptible
              << ", Infected: " << infected
              << ", Recovered: " << recovered
              << ", Vaccinated: " << vaccinated << std::endl;
}
