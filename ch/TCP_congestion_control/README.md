# TCP_congestion_control
[![HitCount](http://hits.dwyl.com/ksanu1998/https://githubcom/ksanu1998/TCP_congestion_control.svg)](http://hits.dwyl.com/ksanu1998/https://githubcom/ksanu1998/TCP_congestion_control)<br>
<b> Lab Statement </b> <br>
Implement the congestion control algorithm of TCP over a UDP socket. The server requests a file of data. The client then packetizes the file and sends these packets over the network. The server acknowledges these packets. These acknowledgments control the congestion window at the client (which is the transmitter of these packets). 


<b> Note </b> <br>
1. This repository contains two code files, <a href="https://github.com/ksanu1998/TCP_congestion_control/blob/master/server.c">server.c</a> and <a href="https://github.com/ksanu1998/TCP_congestion_control/blob/master/client.c">client.c</a>, which have to be compiled and run.<br> 
2. Please refer to <a href="https://github.com/ksanu1998/TCP_congestion_control/blob/master/readme.txt">readme.txt</a> for compiling guidelines. <br>
3. Please refer to <a href="https://github.com/ksanu1998/TCP_congestion_control/blob/master/report.pdf">report.pdf</a> for plots of Congestion Window vs Time, ACK Arrival Instant vs Time, Packet Loss Instant vs Time and Packet Arrival Instant vs Time, for a sample file.<br>

