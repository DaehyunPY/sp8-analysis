### Version: 0.0.1
FROM ubuntu:16.10
MAINTAINER Daehyun You daehyun9176@icloud.com
RUN apt-get update
RUN apt-get install -y nautilus less vim wget gcc g++ cmake
WORKDIR /root

### CERN ROOT
RUN wget 'https://root.cern.ch/download/root_v6.08.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz'
RUN bash -c 'tar xf root*.tar.gz'
RUN echo '. root/bin/thisroot.sh' >> .bashrc

### analysis
ADD ./ analysis/
ENV ROOTSYS /root/root
RUN mkdir build && cd build && cmake ../analysis && cmake --build . && cd ..
RUN cp -fr analysis/macros/* root/macros/
ENV PATH /root/build:$PATH

ENTRYPOINT bash
