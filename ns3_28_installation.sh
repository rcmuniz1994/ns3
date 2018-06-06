#!/bin/bash

# ns-3 installation and compilation

# Criar pasta
#mkdir ~/ns-3
cd ~/ns-3
# Baixar:
sudo wget https://www.nsnam.org/release/ns-allinone-3.28.tar.bz2 --no-check-certificate
#Descompactar:
tar xvjf ns-allinone-3.28.tar.bz2
#Entrar na pasta:
cd ns-allinone-3.28/ns-3.28/
# Configurar:
CXXFLAGS="-Wall -g -O0" ./waf --build-profile=debug --enable-examples --enable-tests configure
# Compilar:
./waf

# Testar 
./waf --run hello-simulator



echo For Eclipse configuration, see presentation: Tutorial_instalação_NS-3_e_integração_com_eclipsev06.pptx

