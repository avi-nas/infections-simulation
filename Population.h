    #ifndef POPULATION_H
    #define POPULATION_H

    #include "Individual.h"
    #include <vector>
    #include <random>

    class Population {
    private:
        std::vector<Individual> individuals;
        std::mt19937 gen;  // Random number generator as a private member
        double interactionRate;
        double recoveryRate;
        double mutationRate;

    public:
        Population(int size, double interactionRate, double initialInfectionRate, 
                double recoveryRate, double mutationRate);
        int getSusceptibleCount() const;
        int getInfectedCount() const;
        int getRecoveredCount() const;
        int getVaccinatedCount() const;
        void vaccinateHighRisk(double vaccinationRate);
        void vaccinateSociallyActive(double vaccinationRate, int threshold);
        void updatePopulationState(double infectionRate);
        bool noInfections() const;
        void printPopulationStates() const;
    };

    #endif // POPULATION_H
