#include "F_Voltage.hpp"

int main() {
    parameters p1{"Dati/100O/sweep2_100O_phase", 100, 486e-6, 0.9e-6, 2.144, 50};
    parameters p2{"Dati/330O/sweep2_330O_freq", 330, 486e-6, 0.9e-6, 2.144, 50};
    parameters p3{"Dati/560O/sweep1_560O_freq", 560, 486e-6, 0.9e-6, 2.144, 50};
    
    //voltage_function(p2);
    //response_function(p1);
    //v_fit(p2);
    //r_fit(p1);
    //time_graph("Dati/100O/sweep1_100O");
    //multifit(p1, p2, p3);
    //gaussian_error("Trash?/Dati_modificati/Misure_preliminari/Fixed_voltage_1");
    fitFase(p1);
    disegnaModello(p1);

}