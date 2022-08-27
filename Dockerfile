#########################################################
# OLD Alpine Linux Image
#########################################################

##################################################
# create environment for build (including CAF)
##################################################
FROM alpine:latest AS copystage 
WORKDIR /project
EXPOSE 80
EXPOSE 3128
COPY . .
RUN apk add curl openssl openssl-dev linux-headers
RUN apk add cmake g++ gcc make libexecinfo-dev libexecinfo libunwind
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-bin-2.29-r0.apk
RUN apk add --no-cache --allow-untrusted glibc-2.29-r0.apk 
RUN apk add --no-cache --allow-untrusted glibc-bin-2.29-r0.apk
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
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
RUN cd /project/build && make && chmod 777 /project/build/app/svdemo
ENTRYPOINT ["run.sh"]

#########################################################
# OLD Alpine Linux Image
#########################################################
