### example 
### =======
### docker run -p 2222:22 IMAGE
### ssh root@localhost -p 2222 -Y  # put 'root' for password
### 
FROM ubuntu:16.10
MAINTAINER Daehyun You daehyun9176@icloud.com
RUN apt-get update
RUN apt-get install -y less vim wget gcc g++ cmake ssh nautilus gedit terminator
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

RUN mkdir /var/run/sshd
RUN sed -i 's/PermitRootLogin/#PermitRootLogin/' /etc/ssh/sshd_config
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
RUN echo 'root:root' | chpasswd
EXPOSE 22
ENTRYPOINT `which sshd` -D
