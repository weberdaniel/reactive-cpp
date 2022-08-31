#########################################################
# OLD Alpine Linux Image
#########################################################

##################################################
# create environment for build (including CAF)
##################################################
FROM alpine:latest AS buildstage-actorframework
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
    make -j

##################################################
# create the actual build artifact
##################################################
FROM buildstage-actorframework as buildstage-svdemo
WORKDIR /project
RUN mkdir /project/svdemo
COPY . /project/svdemo
RUN ls /project
RUN cd /project/actor-framework/build; find -name *.so
RUN mkdir /project/svdemo/build && cd /project/svdemo/build && cmake -DDOCKER=ON .. && make -j && chmod 777 /project/svdemo/build/app/svdemo/svdemo && chmod 777 /project/svdemo/run.sh
ENTRYPOINT ["/project/svdemo/run.sh"]

#########################################################
# Create a deploy stage
#########################################################
FROM alpine:latest as caf-supervisor
WORKDIR /project
RUN apk add libstdc++ libunwind
COPY --from=buildstage-svdemo /project/svdemo/build/app/svdemo/svdemo .
COPY --from=buildstage-svdemo /project/svdemo/run.sh .
COPY --from=buildstage-svdemo /project/svdemo/app/svdemo/caf-application.conf .
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_core/libcaf_core.so* /project/
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_io/libcaf_io.so* /project/
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_net/libcaf_net.so* /project/
RUN ls
ENTRYPOINT ["/project/run.sh"]
