FROM ubuntu:20.04
ENV TZ=Europe/Minsk
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y cmake git clang-10 python3-pip gcc-10 g++-10 unzip wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
ENV CC=/usr/bin/gcc-10
ENV CXX=/usr/bin/g++-10
RUN python3 -m pip install --upgrade pip setuptools \
    && python3 -m pip install conan \
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.7.5/bazelisk-linux-amd64 \
    && chmod +x bazelisk-linux-amd64 && mv bazelisk-linux-amd64 /usr/bin/bazel