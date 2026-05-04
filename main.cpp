#include "F_Voltage.hpp"

int main() {
    parameters p1{"", 100, 470e-6, 1e-6, 2.5, 50};
    
    voltage_function(p1);
    response_function(p1);
}