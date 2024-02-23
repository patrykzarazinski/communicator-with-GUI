FROM ubuntu:22.04

LABEL maintainer="Patryk Zarazinski"

ENV TZ=Europe/Warsaw

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -yqq update  && \
    apt-get -yqq install wget && \
    apt-get -yqq install build-essential

#CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.29.0-rc1/cmake-3.29.0-rc1-linux-x86_64.tar.gz && \
    tar -xzf cmake-3.29.0-rc1-linux-x86_64.tar.gz -C /opt && \
    rm -f cmake-3.29.0-rc1-linux-x86_64.tar.gz && \
    mv /opt/cmake-3.29.0-rc1-linux-x86_64 /opt/cmake

ENV PATH="/opt/cmake/bin:$PATH"

#Boost
RUN apt-get -yq install libboost-all-dev

COPY . /usr/src/app

WORKDIR /usr/src/app

CMD ["bash"]