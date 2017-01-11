FROM david9107/mydesktop
MAINTAINER Daehyun You <daehyun@mail.tagen.tohoku.ac.jp>

WORKDIR /root

### analysis
ADD ./ /opt/analysis/
RUN cd /opt/analysis && mkdir build && cd build && cmake .. && make all \
    && cd .. && cp macros/* /usr/share/root/macros/

CMD /sbin/init
EXPOSE 22 5901 8888
