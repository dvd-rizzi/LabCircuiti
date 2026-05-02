#include "F_Voltage.hpp"

int main() {
    parameters p1{100, 470*10e-6, 10e-6, 2.5, 50};
    parameters p2{100, 4.7*10e-3, 2*10e-6, 170, 50};
    voltage_function(p1);
}