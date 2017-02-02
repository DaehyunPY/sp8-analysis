FROM fedora:latest
MAINTAINER Daehyun You <daehyun@mail.tagen.tohoku.ac.jp>

RUN dnf update -y && dnf install -y \
        @'C Development Tools and Libraries' \
        cmake gdb-gdbserver \
        boost-devel rapidjson-devel \
        root root-roofit \
    && dnf clean all

WORKDIR /root/
ADD ./ /opt/sp8-analysis/
RUN mkdir build && cd build && cmake /opt/sp8-analysis/ -DCMAKE_BUILD_TYPE=Release && make all && cd .. \
    && ln -s build/SortExe sort.exe && ln -s build/AnalysisExe ana.exe
RUN mkdir build-debug && cd build-debug && cmake /opt/sp8-analysis/ -DCMAKE_BUILD_TYPE=Debug && make all && cd .. \
    && ln -s build-debug/SortExe sort-debug.exe && ln -s build-debug/AnalysisExe ana-debug.exe

EXPOSE 2222
CMD bash

