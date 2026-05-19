#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati/100O/sweep2_100O_freq", 100, 486e-6, 0.9e-6, 2.144, 50};
    parameters p2{"Dati/330O/sweep1_330O_freq", 330, 486e-6, 0.9e-6, 2.144, 50};
    parameters p3{"Dati/560O/sweep1_560O_freq", 560, 486e-6, 0.9e-6, 2.144, 50};
    
    //voltage_function(p3);
    //response_function(p1);
    //v_fit(p3);

    multifit(p1, p2, p3);

    //gaussian_error("Dati_modificati/Misure_preliminari/Fixed_voltage_1");
}