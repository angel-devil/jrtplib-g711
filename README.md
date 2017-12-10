# jrtplib-g711

# 概述   
利用JRTPLIB库实现编码方式为G.711的音频RTP推流服务，并通过VLC播放音频。
# 准备工作
首先完成JTHREAD和JRTPLIB库的编译安装，安装后的链接库位于/usr/lib/，头文件位于/usr/include/jthread和/usr/include/jrtplib。
# 说明
* G.711标准下主要有两种压缩算法：u-law,a-law；
* G.711音频编码采样率为8000每秒；
* G.711音频编码速率为64kbps，即每秒发送64000比特；
* pcmu的负载类型为0，pcma的负载类型为8；
* timestamp增量=时钟频率/帧率；
* 40ms发送一个RTP包，1s可以发送25个RTP包，即帧率为25；
* timestamp增量=8000/25=320；
