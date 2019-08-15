FROM ubuntu:19.10

WORKDIR /usr/src/tiny-os

RUN apt-get -y update
RUN apt-get -y install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu build-essential qemu-system-aarch64 cmake --no-install-recommends
RUN apt-get clean

ADD ./ ./
