#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati_modificati/sweep2", 100, 470e-6, 00.9e-6, 2.5, 50};
    
    voltage_function(p1);
    response_function(p1);
    gaussian_error("Dati_modificati/Fixed_voltage");
    v_fit(p1);
}