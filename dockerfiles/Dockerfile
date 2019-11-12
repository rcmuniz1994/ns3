FROM ubuntu:14.04

RUN sudo apt-get update
RUN sudo apt-get install software-properties-common -y


RUN mkdir ns3
COPY ./ns-allinone-3.27/ ns3
COPY ./compile_ns3.sh ns3
COPY ./dependencias.sh ns3

WORKDIR ns3

RUN . dependencias.sh

RUN . compile_ns3.sh

