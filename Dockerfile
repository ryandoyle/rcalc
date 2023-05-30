FROM ubuntu:22.04
RUN apt update && apt install -y mingw-w64 build-essential
WORKDIR /work