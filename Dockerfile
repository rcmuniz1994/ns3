FROM ubuntu:14.04


RUN mkdir ns3
COPY ./ns3_28_installation.sh ns3
COPY ./dependencias.sh ns3

WORKDIR ns3

RUN./dependencias.sh
RUN ./ns3_28_installation.sh

