#include "Person.h"

Person::Person() : state(Susceptible), daysInfected(0), infectionDuration(0) {}

std::string Person::status_string() const {
    switch (state) {
        case Susceptible: return "Susceptible";
        case Infected: return "Infected";
        case Recovered: return "Recovered";
        case Vaccinated: return "Vaccinated";
        default: return "Unknown";
    }
}

void Person::update() {
    if (state == Infected && daysInfected++ >= infectionDuration) {
        state = Recovered;
    }
}

void Person::infect(int n) {
    if (state == Susceptible) {
        state = Infected;
        infectionDuration = n;
        daysInfected = 0;
    }
}

bool Person::is_stable() const {
    return state == Recovered;
}
