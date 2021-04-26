#!/bin/bash

./var/www/html/CPE190/jetson-inference/build/aarch64/bin/detectnet /dev/video0 rtp://@:5000 --headless &

cd ~/Desktop
vlc -vvv hi.sdp --sout '#transcode{vcodec=theo,acodec=vorb,vb=800,ab=128}:standard{access=http,mux=ogg,dst=:5000}' &
