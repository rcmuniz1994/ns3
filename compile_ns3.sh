#!/bin/bash

# Entrar na pasta

 cd ns-3.27/

# Configurar

CXXFLAGS="-Wall -g -O0" ./waf --build-profile=debug --enable-examples --enable-tests configure

# Testar

./waf --run hello-simulator
