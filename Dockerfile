FROM ubuntu:latest AS copystage 
WORKDIR /project
COPY . .
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get update 
RUN apt-get -y install apt-utils
RUN apt-get -y install dialog 
RUN apt-get -y install git curl
RUN apt-get update && apt-get -y install cmake protobuf-compiler g++ gcc
RUN cd /project && \
    mkdir build && \
    cd build  &&  \
    cmake .. && \
    make

# Code file to execute when the docker container starts up (`entrypoint.sh`)
RUN cd /project; ls -la
