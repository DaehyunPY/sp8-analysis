FROM fedora:latest
MAINTAINER Daehyun You <daehyun@mail.tagen.tohoku.ac.jp>

RUN dnf update -y && dnf install -y \
        @'C Development Tools and Libraries' \
        cmake gdb-gdbserver \
        boost-devel rapidjson-devel \
        root root-roofit \
    && dnf clean all

WORKDIR /root/
RUN mkdir bin
ENV PATH "/root/bin:$PATH"

ADD ./ /opt/sp8-analysis/
RUN mkdir build && cd build && cmake /opt/sp8-analysis/ -DCMAKE_BUILD_TYPE=Release && make all && cd .. \
    && cd bin && ln -s /root/build/SortExe sort.exe && ln -s /root/build/AnalysisExe ana.exe

CMD bash
