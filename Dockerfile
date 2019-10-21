FROM ubuntu:14.04

RUN sudo apt-get update
RUN sudo apt-get install software-properties-common

COPY ./dependencias.sh .
RUN ./dependencias.sh

RUN mkdir ns3
COPY ./ns-allinone-3.27 ns3
COPY ./compile_ns3.sh ns3

WORKDIR ns3

RUN ./compile_ns3.sh

