#########################################################
# OLD Alpine Linux Image
#########################################################

##################################################
# create environment for build (including CAF)
##################################################
FROM alpine:3.16 AS buildstage-actorframework
ARG number_of_build_jobs
ARG http_proxy
ARG https_proxy
ENV http_proxy ${http_proxy}
ENV https_proxy ${https_proxy}
RUN echo "Proxy: $http_proxy $https_proxy"
WORKDIR /project
EXPOSE 80
EXPOSE 3128
#note according to https://gitlab.alpinelinux.org/alpine/aports/-/issues/10446 you need to explicitly
#install wget, because the native busybox version of wget does not support proxys
RUN apk add curl openssl openssl-dev linux-headers wget
RUN apk add cmake g++ gcc make libexecinfo-dev libexecinfo libunwind libunwind-dev compiler-rt git
RUN echo 
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk 
RUN wget --no-check-certificate https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-bin-2.29-r0.apk
#RUN apk add --no-cache --allow-untrusted glibc-2.29-r0.apk 
#RUN apk add --no-cache --allow-untrusted glibc-bin-2.29-r0.apk
RUN cd /project && git clone https://github.com/weberdaniel/actor-framework && cd actor-framework && git checkout 1-incorporate-libunwind
RUN cd /project/actor-framework && \
    ./configure --build-type=Debug --log-level=TRACE --disable-examples --disable-tools --enable-utility-targets --enable-runtime-checks --enable-shared-libs && \
    cd build  &&  \
    make -j $number_of_build_jobs

##################################################
# create the actual build artifact
##################################################
FROM buildstage-actorframework as buildstage-svdemo
ARG number_of_build_jobs
ARG http_proxy
ARG https_proxy
ENV http_proxy ${http_proxy}
ENV https_proxy ${https_proxy}
RUN echo "Proxy: $http_proxy $https_proxy"
WORKDIR /project
RUN mkdir /project/svdemo
COPY . /project/svdemo
RUN ls /project
RUN cd /project/actor-framework/build; find -name *.so
RUN mkdir /project/svdemo/build && cd /project/svdemo/build && cmake -DDOCKER=ON .. && make -j $number_of_build_jobs && chmod 777 /project/svdemo/build/app/svdemo/svdemo && chmod 777 /project/svdemo/run.sh
ENTRYPOINT ["/project/svdemo/run.sh"]

#########################################################
# Create a deploy stage
#########################################################
FROM alpine:3.16 as caf-supervisor
ARG http_proxy
ARG https_proxy
ENV http_proxy ${http_proxy}
ENV https_proxy ${https_proxy}
RUN echo "Proxy: $http_proxy $https_proxy"
WORKDIR /project
RUN apk add wget libstdc++ libunwind
COPY --from=buildstage-svdemo /project/svdemo/build/app/svdemo/svdemo .
COPY --from=buildstage-svdemo /project/svdemo/run.sh .
COPY --from=buildstage-svdemo /project/svdemo/app/svdemo/caf-application.conf .
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_core/libcaf_core.so* /project/
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_io/libcaf_io.so* /project/
COPY --from=buildstage-svdemo /project/actor-framework/build/libcaf_net/libcaf_net.so* /project/
RUN ls
ENTRYPOINT ["/project/run.sh"]
