# Tema 4
### Suponiendo que en el LED azul caen aprox 2v, y quereis que pasen 2mA, que resistencia deberíais poner?
R = ?
V (esp32) = 3.3v
V (caída led) = 2v
I = 2mA

R = V/I -> (3.3 - 2)/0.002 = 650 Ohm ~ 680 Ohm

_Checking for 5v and 10mA_
R = ?
V (arduino) = 5v
V (caída led) = 2
I = 20mA

R = V/I -> (5 - 2)/0.02 = 150 Ohm ~ 220 Ohm