FROM ubuntu:16.10
MAINTAINER Daehyun You daehyun9176@icloud.com
RUN apt-get update
RUN apt-get install -y gcc-5 g++-5 libc-dev dpkg-dev make
RUN apt-get install -y git cmake binutils libx11-dev libxpm-dev libxft-dev libxext-dev
RUN apt-get install -y ssh wget less vim 
RUN apt-get install -y nautilus gedit terminator
WORKDIR /root

### CERN ROOT
RUN wget 'https://root.cern.ch/download/root_v6.08.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz'
RUN bash -c 'tar -xf root*.tar.gz'
RUN echo '. root/bin/thisroot.sh' >> .bashrc

### CLion
RUN wget 'https://download.jetbrains.com/cpp/CLion-2016.3.tar.gz'
RUN bash -c 'tar -xf CLion*.tar.gz && ln -s clion* clion'
RUN echo 'export PATH=/root/clion/bin:$PATH' >> .bashrc

### Anaconda
RUN wget 'https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh'
RUN bash Anaconda3*.sh -b
RUN echo 'export PATH=/root/anaconda3/bin:$PATH' >> .bashrc

### analysis
ADD ./ analysis/
ENV ROOTSYS /root/root
RUN mkdir build && cd build && cmake ../analysis && cmake --build . && cd ..
RUN cp -fr analysis/macros/* root/macros/
RUN echo 'export PATH=/root/build:$PATH' >> .bashrc

RUN mkdir /var/run/sshd
RUN sed -i 's/PermitRootLogin/#PermitRootLogin/' /etc/ssh/sshd_config
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
RUN echo 'root:root' | chpasswd
EXPOSE 22
ENTRYPOINT `which sshd` -D
