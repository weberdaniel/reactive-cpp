##################################################
# create environment for build (including CAF)
##################################################
FROM ubuntu:latest AS copystage 
WORKDIR /project
COPY . .
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get update 
RUN apt-get -y install apt-utils
RUN apt-get -y install dialog 
RUN apt-get -y install git curl openssl libssl-dev
RUN apt-get update && apt-get -y install cmake protobuf-compiler g++ gcc
RUN cd /project && \
    mkdir build && \
    cd build  &&  \
    cmake .. && \
    make
RUN cd /project; ls -la

##################################################
# create the actual build artifact
##################################################
FROM copystage as buildstage
WORKDIR /project
RUN cd /project/build && make 

##################################################
#  Do a cpplint on everything and check result
##################################################
#FROM copystage as lintstage
#WORKDIR /project
#RUN cd /project/build 
