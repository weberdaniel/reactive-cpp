# note ubuntu:18.04 is currntly ubuntu-minimal
# according to https://canonical.com/blog/minimal-ubuntu-released
FROM ubuntu:18.04 AS copystage
WORKDIR /project
RUN apt-get update
RUN apt-get install -y openssl libssl-dev curl linux-headers-generic
RUN apt-get install -y cmake g++ gcc make git
RUN git clone https://github.com/actor-framework/actor-framework.git
RUN cd actor-framework && ./configure --dev-mode && cd build && make
RUN pwd
RUN ls

##################################################
# create the actual build artifact
##################################################
FROM copystage as buildstage
WORKDIR /project
COPY . .
RUN mkdir -p /project/build
RUN cd /project/build && cmake .. && make && chmod 777 /project/build/app/svdemo
ENTRYPOINT ["run.sh"]

#########################################################
# OLD Alpine Linux Image
#########################################################

##################################################
# create environment for build (including CAF)
##################################################
#FROM alpine:latest AS copystage 
#WORKDIR /project
#EXPOSE 80
#EXPOSE 3128
#COPY . .
#RUN apk add curl openssl openssl-dev linux-headers
#RUN apk add cmake g++ gcc make libexecinfo-dev libexecinfo libunwind
#RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk
#RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-bin-2.29-r0.apk
#RUN apk add --no-cache --allow-untrusted glibc-2.29-r0.apk 
#RUN apk add --no-cache --allow-untrusted glibc-bin-2.29-r0.apk
#RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
#RUN cd /project && \
#    mkdir build && \
#    cd build  &&  \
#    cmake .. && \
#    make
#RUN cd /project; ls -la

##################################################
# create the actual build artifact
##################################################
#FROM copystage as buildstage
#WORKDIR /project
#RUN cd /project/build && make && chmod 777 /project/build/app/svdemo
#ENTRYPOINT ["run.sh"]

#########################################################
# OLD Alpine Linux Image
#########################################################
