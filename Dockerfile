FROM david9107/mydesktop:develop
MAINTAINER Daehyun You <daehyun@mail.tagen.tohoku.ac.jp>

WORKDIR /root

### analysis
ADD ./ /opt/analysis/
RUN cd /opt/analysis && mkdir build && cd build && cmake .. && make all \
    && cd .. && cp -fr macros/* /usr/share/root/macros/

CMD /sbin/init
EXPOSE 22 5901 8888
