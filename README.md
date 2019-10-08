# ns3
This repository was created with the intention to safe my progress on ns3.

To get this repository's content. Run this on your terminal:

git clone https://github.com/rcmuniz1994/ns3.git

To install a new NS-3.28:

1) To install the NS-3 dependencies run: ./depencias.sh. 
2) To install the NS-3 run: ./ns3_28_installation.sh.

If you like to run the NS3 on a container, you must run this on your terminal to build the Docker image:

docker image build -t ns3:docker

Now you can run a container. Run this:

docker run -it --rm --name ns3:docker

To run this command you must have Docker installed. To install Docker, access this: https://github.com/gomex/docker-para-desenvolvedores/blob/master/manuscript/instalacao.md
