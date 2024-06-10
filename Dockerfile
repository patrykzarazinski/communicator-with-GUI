FROM ubuntu:22.04

LABEL maintainer="Patryk Zarazinski"

ENV TZ=Europe/Warsaw

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -yqq update  && \
    apt-get -yqq install wget && \
    apt-get -yqq install build-essential && \
    apt-get -yqq install git

#CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.29.0-rc1/cmake-3.29.0-rc1-linux-x86_64.tar.gz && \
    tar -xzf cmake-3.29.0-rc1-linux-x86_64.tar.gz -C /opt && \
    rm -f cmake-3.29.0-rc1-linux-x86_64.tar.gz && \
    mv /opt/cmake-3.29.0-rc1-linux-x86_64 /opt/cmake

ENV PATH="/opt/cmake/bin:$PATH"

#Boost
RUN apt-get -yq install libboost-all-dev

#Bazel
RUN apt-get -yqq install apt-transport-https curl gnupg -y && \
    curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg && \
    mv bazel-archive-keyring.gpg /usr/share/keyrings && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | \
        tee /etc/apt/sources.list.d/bazel.list && \
    apt-get update && apt -yqq install bazel

WORKDIR /opt

#Abseil
RUN git clone https://github.com/abseil/abseil-cpp.git && \
    mkdir abseil-cpp/build && cd abseil-cpp/build && \
    cmake .. && \
    make && \
    make install

#Protobuf
RUN wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v25.3.tar.gz && \
    tar -xzf v25.3.tar.gz && \
    cd protobuf-25.3 && mv /opt/abseil-cpp third_party && \
    mkdir build && cd build && \
    cmake -Dprotobuf_BUILD_TESTS=OFF .. && \
    make && \
    make install

COPY . /usr/src/app

WORKDIR /usr/src/app

#spdlog
RUN git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && mkdir build && cd build && \
    git checkout v1.14.1 && \
    cmake .. && make -j
#temporary_change
CMD ["bash"]