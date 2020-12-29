FROM ubuntu:20.04
ENV TZ=Europe/Minsk
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y cmake git clang-10 python3-pip gcc-10 g++-10 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
ENV CC=/usr/bin/gcc-10
ENV CXX=/usr/bin/g++-10
RUN python3 -m pip install --upgrade pip setuptools \
    && python3 -m pip install conan
RUN apt-get update \
    && apt-get install -y unzip wget \
    && apt-get clean && rm -rf /var/lib/apt/lists/* \
    && wget https://github.com/bazelbuild/bazel/releases/download/3.5.1/bazel-3.5.1-installer-linux-x86_64.sh \
    && chmod +x bazel-3.5.1-installer-linux-x86_64.sh && ./bazel-3.5.1-installer-linux-x86_64.sh \
    && rm bazel-3.5.1-installer-linux-x86_64.sh