#ifndef PERSON_H
#define PERSON_H

#include <string>

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

#endif // PERSON_H
