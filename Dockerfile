FROM ubuntu:latest

WORKDIR /usr/src/tiny-os

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo

RUN apt-get -y update
RUN apt-get -y install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu build-essential qemu-system-aarch64 cmake telnet --no-install-recommends
RUN apt-get clean

ADD ./ ./
