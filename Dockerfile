#########################################################
# OLD Alpine Linux Image
#########################################################

##################################################
# create environment for build (including CAF)
##################################################
FROM alpine:latest AS svdemocafstage
WORKDIR /project
EXPOSE 80
EXPOSE 3128
RUN apk add curl openssl openssl-dev linux-headers
RUN apk add cmake g++ gcc make libexecinfo-dev libexecinfo libunwind libunwind-dev compiler-rt git
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-bin-2.29-r0.apk
RUN apk add --no-cache --allow-untrusted glibc-2.29-r0.apk 
RUN apk add --no-cache --allow-untrusted glibc-bin-2.29-r0.apk
RUN cd /project && git clone https://github.com/weberdaniel/actor-framework && cd actor-framework && git checkout 1-incorporate-libunwind
RUN cd /project/actor-framework && \
    ./configure --build-type=Debug --log-level=TRACE --disable-examples --disable-tools --enable-utility-targets --enable-runtime-checks --enable-shared-libs && \
    cd build  &&  \
    make

##################################################
# create the actual build artifact
##################################################
FROM svdemocafstage as svdemostage
WORKDIR /project
RUN mkdir /project/svdemo
COPY . /project/svdemo
RUN ls /project
RUN cd /project/actor-framework/build; find -name *.so
RUN mkdir /project/svdemo/build && cd /project/svdemo/build && cmake -DDOCKER=ON .. && make && chmod 777 /project/svdemo/build/app/svdemo/svdemo && chmod 777 /project/svdemo/run.sh
ENTRYPOINT ["/project/svdemo/run.sh"]

#########################################################
# OLD Alpine Linux Image
#########################################################
