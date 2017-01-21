#!/bin/sh

### parameters 
img=david9107/sp8-analysis
user=daehyun
container=mydesktop-$user
cperiod=100000
cquota=200000
memory=16g
huser=$user
hhome=/home/$huser
guser=$huser
ghome=/home/$guser
userp=10000
sshp=22
vncp=5900
dp=1
stime=30s
uid=$(id -u $huser)
vncd="vncserver-$guser@:$dp.service"

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
        useradd -u $uid -G wheel $guser && passwd $guser \
        && groupadd -g 2000 work && usermod -a -G work $guser" \
    && echo 'Setting up VNC password... ' \
    && docker exec -ti $container su $guser -c " \
        vncpasswd \
        && echo $'#!/bin/sh\nvncconfig -nowin &\nstartkde &' > ~/.vnc/xstartup \
        && chmod u+x ~/.vnc/xstartup" \
    && echo 'Running daemons... ' \
    && docker exec $container sh -c " \
        cp /usr/lib/systemd/system/vncserver@.service /etc/systemd/system/$vncd \
        && sed -i 's/<USER>/'$guser'/g' /etc/systemd/system/$vncd \
        && sleep $stime \
        && systemctl daemon-reload \
        && systemctl enable sshd && systemctl start sshd \
        && systemctl enable $vncd && systemctl start $vncd \
        && firewall-cmd --permanent --zone=public --add-port=$(expr $vncp + $dp)/tcp \
        && firewall-cmd --reload") \
|| docker start $container
