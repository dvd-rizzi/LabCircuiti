import numpy as np
import matplotlib.pyplot as plt

Rv = 50           
R = 220          
L = 0.01          
C = 2e-6          
RL = 0.5          


omega = np.logspace(1, 5, 1000) 

Z_L = RL + 1j * omega * L
Z_C = 1 / (1j * omega * C)
Zp = (Z_L * Z_C) / (Z_L + Z_C)

H = R / (Rv + Zp + R)

H_dB = 20 * np.log10(np.abs(H))

plt.figure(figsize=(10, 6))
plt.semilogx(omega, H_dB, label='Modulo |H(ω)|', color='blue', linewidth=2)

omega_0 = 1 / np.sqrt(L * C)
plt.axvline(omega_0, color='red', linestyle='--', alpha=0.7, 
            label=f'Risonanza ω₀ ≈ {omega_0:.1f} rad/s')

plt.title('Risposta in frequenza del Notch Filter')
plt.xlabel('Frequenza angolare ω [rad/s] (Scala Logaritmica)')
plt.ylabel('Guadagno [dB]')
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.legend()
plt.show()