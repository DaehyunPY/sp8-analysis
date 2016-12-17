FROM ubuntu:16.04
MAINTAINER Daehyun You <daehyun9176@icloud.com>

RUN apt-get update && apt-get install -y \
    gcc-5 g++-5 libc-dev dpkg-dev make \
    git cmake ssh wget less vim unzip \
    nautilus gedit terminator firefox
WORKDIR /root
RUN mkdir bin && echo 'export PATH=/root/bin:$PATH' >> .bashrc
RUN dbus-uuidgen > /etc/machine-id

### Sublime Text
RUN wget 'https://download.sublimetext.com/sublime_text_3_build_3126_x64.tar.bz2' && \
    bash -c 'tar -xf *.tar.bz2 && rm *.tar.bz2'
RUN ln -s /root/sublime_text_3/sublime_text bin/

### CLion
RUN wget 'https://download.jetbrains.com/cpp/CLion-2016.3.tar.gz' && \
    bash -c 'tar -xf *.tar.gz && rm *.tar.gz && ln -s clion* clion'
RUN echo 'export PATH=/root/clion/bin:$PATH' >> .bashrc

### PyCharm
RUN wget 'https://download.jetbrains.com/python/pycharm-community-2016.3.1.tar.gz' && \
    bash -c 'tar -xf *.tar.gz && rm *.tar.gz && ln -s pycharm* pycharm'
RUN echo 'export PATH=/root/pycharm/bin:$PATH' >> .bashrc

### CERN ROOT
RUN wget 'https://root.cern.ch/download/root_v6.08.00.Linux-ubuntu16-x86_64-gcc5.4.tar.gz' && \
    bash -c 'tar -xf *.tar.gz && rm *.tar.gz'
RUN echo '. root/bin/thisroot.sh' >> .bashrc
ENV ROOTSYS /root/root

### Anaconda
RUN wget 'https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh' && \
    bash Anaconda3*.sh -b && rm Anaconda3*.sh
RUN echo 'export PATH=/root/anaconda3/bin:$PATH' >> .bashrc
ENV PATH /root/anaconda3/bin:$PATH
RUN conda update -y --all

### opencv
RUN wget 'https://github.com/Itseez/opencv/archive/3.1.0.zip' && \
    unzip *.zip && rm *.zip && ln -s opencv* opencv
RUN cd opencv && mkdir build && cd build && cmake .. \
    -DPYTHON3_EXECUTABLE=$(which python) \
    -DBUILD_opencv_python3=TRUE \
    -DPYTHON_DEFAULT_EXECUTABLE=$(which python) && \
    make -j4 all && cd
RUN echo 'export PYTHONPATH=/root/opencv/build/lib/python3:$PYTHONPATH' >> .bashrc

### analysis
ADD ./ analysis/
RUN cd analysis && mkdir build && cd build && cmake .. && make all && cd
RUN mv root/macros/* analysis/macros/ && rmdir root/macros && ln -s /root/analysis/macros root/macros && \
    ln -s /root/analysis/build/*Exe bin/

RUN mkdir /var/run/sshd && \
    sed -i 's/PermitRootLogin/#PermitRootLogin/' /etc/ssh/sshd_config && echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config && \
    echo 'root:root' | chpasswd
EXPOSE 22 8888
ENTRYPOINT `which sshd` -D
