FROM ubuntu:16.04
MAINTAINER Daehyun You <daehyun9176@icloud.com>

### packages
RUN apt-get update
RUN apt-get install -y gcc-5 g++-5 libc-dev dpkg-dev make
RUN apt-get install -y git cmake ssh wget less vim 
RUN apt-get install -y nautilus gedit terminator
WORKDIR /root
RUN mkdir bin && echo 'export PATH=/root/bin:$PATH' >> .bashrc

### Sublime Text
RUN wget 'https://download.sublimetext.com/sublime_text_3_build_3126_x64.tar.bz2'
RUN bash -c 'tar -xf sublime_text_3*.tar.bz2 && rm sublime_text_3*.tar.bz2'
RUN ln -s /root/sublime_text_3/sublime_text bin/

### CERN ROOT
RUN wget 'https://root.cern.ch/download/root_v6.08.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz'
RUN bash -c 'tar -xf root*.tar.gz && rm root*.tar.gz'
RUN echo '. root/bin/thisroot.sh' >> .bashrc
ENV ROOTSYS /root/root

### CLion
RUN wget 'https://download.jetbrains.com/cpp/CLion-2016.3.tar.gz'
RUN bash -c 'tar -xf CLion*.tar.gz && ln -s clion* clion && rm CLion*.tar.gz'
RUN echo 'export PATH=/root/clion/bin:$PATH' >> .bashrc

### Anaconda
RUN wget 'https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh'
RUN bash Anaconda3*.sh -b && rm Anaconda3*.sh
RUN echo 'export PATH=/root/anaconda3/bin:$PATH' >> .bashrc

### analysis
ADD ./ analysis/
RUN mkdir build && cd build && cmake ../analysis && cmake --build . && cd ..
RUN mv root/macros/* analysis/macros/ && rmdir root/macros && ln -s /root/analysis/macros root/macros
RUN ln -s /root/build/*Exe bin/

RUN mkdir /var/run/sshd
RUN sed -i 's/PermitRootLogin/#PermitRootLogin/' /etc/ssh/sshd_config
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
RUN echo 'root:root' | chpasswd
EXPOSE 22
ENTRYPOINT `which sshd` -D
