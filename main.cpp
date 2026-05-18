#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati_modificati/100ohm/sweep4_freq", 100, 470e-6, 0.9e-6, 2.5, 50};
    parameters p2{"Dati_nuovi/330O/sweep2_330O_freq.txt", 330, 470e-6, 0.9e-6, 2.5, 50};
    parameters p3{"Dati_modificati/560ohm/sweep1_freq_560O", 560, 470e-6, 0.9e-6, 2.5, 50};
    
    voltage_function(p2);
    response_function(p2);
    v_fit(p2);

    gaussian_error("Dati_modificati/Misure_preliminari/Fixed_voltage_1");
}