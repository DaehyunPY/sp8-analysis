FROM david9107/mydesktop
MAINTAINER Daehyun You <daehyun@mail.tagen.tohoku.ac.jp>

WORKDIR /root

### install packages
RUN dnf update -y && dnf install -y \
        boost-devel rapidjson-devel \
    && dnf clean all

### analysis
ADD ./ /opt/analysis/
RUN cd /opt/analysis && mkdir build && cd build && cmake .. && make all \
    && cd .. && cp -fr macros/* /usr/share/root/macros/

CMD /sbin/init
EXPOSE 22 5901 8888
