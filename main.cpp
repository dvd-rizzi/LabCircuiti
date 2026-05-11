#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati_modificati/100ohm/sweep4_freq", 100, 470e-6, 0.9e-6, 2.5, 50};
    parameters p2{"Dati_modificati/330ohm/sweep2_freq_330O", 330, 470e-6, 0.9e-6, 2.5, 50};
    
    voltage_function(p2);
    response_function(p1);
    v_fit(p2);

    gaussian_error("Dati_modificati/Misure_preliminari/Fixed_voltage_1");
}