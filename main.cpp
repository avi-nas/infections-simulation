#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "Population.h"
#include <cstdlib>

int main() {
    std::unordered_map<std::string, double> parameters;
    std::ifstream paramFile("parameters.txt");
    std::string line;

    while (std::getline(paramFile, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        if (std::getline(iss, key, '=') && (iss >> value)) {
            parameters[key] = value;
        }
    }
    paramFile.close();

    // Assign parameters to variables
    int populationSize = static_cast<int>(parameters["populationSize"]);
    double interactionRate = parameters["interactionRate"];
    double infectionRate = parameters["infectionRate"];
    double initialInfectionRate = parameters["initialInfectionRate"];
    double recoveryRate = parameters["recoveryRate"];
    double vaccinationRate = parameters["vaccinationRate"];
    double mutationRate = parameters["mutationRate"];
    int totalSteps = static_cast<int>(parameters["totalSteps"]);

    Population population(populationSize, interactionRate, initialInfectionRate, recoveryRate, mutationRate);
    population.vaccinateHighRisk(vaccinationRate);  // Or use vaccinateSociallyActive

    std::ofstream outFile("simulation_output.csv");
    outFile << "TimeStep,Susceptible,Infected,Recovered,Vaccinated\n";

    for (int step = 0; step < totalSteps; ++step) {
        population.updatePopulationState(infectionRate);

        int numSusceptible = population.getSusceptibleCount();
        int numInfected = population.getInfectedCount();
        int numRecovered = population.getRecoveredCount();
        int numVaccinated = population.getVaccinatedCount();

        outFile << step << "," 
                << numSusceptible << ","
                << numInfected << ","
                << numRecovered << ","
                << numVaccinated << "\n";

        std::cout << "Time Step " << step << std::endl;
        population.printPopulationStates();
        std::cout << std::endl;

        if (population.noInfections()) {
            std::cout << "No more infections, ending simulation at step " << step << std::endl;
            break;
        }
    }
    outFile.close();

    const char* pythonScript = "show.py";
    int result = system(("python " + std::string(pythonScript)).c_str());

    // Check the result of the system call
    if (result == 0) {
        // Execution was successful
        std::cout << "Python script executed successfully." << std::endl;
    } else {
        // There was an error in executing the Python script
        std::cerr << "Error executing Python script." << std::endl;
        return 1; // Return a non-zero value to indicate an error
    }

    return 0;
}

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


class Person {
public:
    enum State { Susceptible, Infected, Recovered, Vaccinated };

    Person();
    std::string status_string() const;
    void update();
    void infect(int n);
    bool is_stable() const;

private:
    State state;
    int daysInfected;
    int infectionDuration;
};

