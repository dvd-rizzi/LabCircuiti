#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati/100O/sweep1_100O_freq", 100, 486e-6, 0.9e-6, 2.5, 50};
    parameters p2{"Dati/330O/sweep2_330O_freq", 330, 470e-6, 0.9e-6, 2.5, 50};
    parameters p3{"Dati_modificati/560ohm/sweep1_freq_560O", 560, 470e-6, 0.9e-6, 2.5, 50};
    
    voltage_function(p1);
    response_function(p1);
    v_fit(p1);

    //gaussian_error("Dati_modificati/Misure_preliminari/Fixed_voltage_1");
}