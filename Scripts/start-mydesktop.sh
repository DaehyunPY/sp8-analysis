#!/bin/sh

### parameters 
img=sp8-analysis
cperiod=100000
cquota=200000
memory=16g
user=daehyun
userp=10000
###
container=mydesktop-$user
sshp=22
vncp=5900
dp=1
stime=30s
uid=$(id -u $user)
hhome=/home/$user
ghome=/home/$user
vncd="vncserver-$user@:$dp.service"

### run img, or start the container 
(docker run -td --privileged \
        --cpu-period $cperiod \
        --cpu-quota $cquota \
        --memory $memory \
        -p $(expr $userp + $sshp):$sshp \
        -p $(expr $userp + $vncp + $dp):$(expr $vncp + $dp) \
        -v $hhome:$ghome \
        -v /helium:/mnt/helium \
        -v /work:/mnt/work \
        -v /opt:/mnt/opt \
        --name $container $img /sbin/init \
    && echo 'Adding user... ' \
    && docker exec -ti $container sh -c " \
        useradd -u $uid -G wheel $user && passwd $user \
        && groupadd -g 2000 work && usermod -a -G work $user" \
    && echo 'Setting up VNC password... ' \
    && docker exec -ti $container su $user -c " \
        vncpasswd \
        && echo $'#!/bin/sh\nvncconfig -nowin &\nstartkde &' > ~/.vnc/xstartup \
        && chmod u+x ~/.vnc/xstartup" \
    && echo 'Running daemons... ' \
    && docker exec $container sh -c " \
        cp /usr/lib/systemd/system/vncserver@.service /etc/systemd/system/$vncd \
        && sed -i 's/<USER>/'$user'/g' /etc/systemd/system/$vncd \
        && sleep $stime \
        && systemctl daemon-reload \
        && systemctl enable $vncd \
        && systemctl start sshd \
        && systemctl start $vncd \
        && firewall-cmd --permanent --zone=public --add-port=$(expr $vncp + $dp)/tcp \
        && firewall-cmd --reload") \
|| docker start $container
