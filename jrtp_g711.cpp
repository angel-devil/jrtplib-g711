#include <rtpsession.h>
#include <rtpudpv4transmitter.h>
#include <rtpipv4address.h>
#include <rtpsessionparams.h>
#include <rtperrors.h>
#include <rtplibraryversion.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace jrtplib;


void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}


void rtpPrintf(uint8_t *buf, uint16_t len)
{
	uint16_t i=0;
		
	printf("RTP len=%d : \n", len);
	
	for(i=0; i<len; i++)
	{
		printf(" %02X", buf[i]);
		if(i%32 == 31)
			printf("\n");
	}
	
	printf("\n");
}


// 1s 8000个采样
// 40ms发送一个RTP包   1s可以发送25个RTP包   即帧率为25
// timestamp增量 = 时钟频率 / 帧率 = 8000 / 25 = 320
// pcmu的负载类型为0 pcma的负载类型为8

int main(void)
{	
	int status;

	RTPSession sess;
	uint16_t portbase = 6666;  
	uint16_t destport = 6664;
	uint8_t destip[]={192, 168, 2, 105};

	FILE *fd;
	uint16_t read_len = 0;
	uint8_t buf[1024] = {0};


	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	/* set g711a param */
	sessparams.SetUsePredefinedSSRC(true);         // 设置使用预先定义的SSRC    
	sessparams.SetOwnTimestampUnit(1.0/8000.0);    // 设置采样间隔 
	sessparams.SetAcceptOwnPackets(true);          // 接收自己发送的数据包  

	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams,&transparams);
	checkerror(status);

	RTPIPv4Address addr(destip,destport);
	status = sess.AddDestination(addr);
	checkerror(status);

	sess.SetDefaultTimestampIncrement(320);        // 设置默认时间戳增加间隔  320
	sess.SetDefaultPayloadType(8);                 // 设置默认负载类型 pcma的负载类型为8
	sess.SetDefaultMark(true);                     // 设置默认标识


	fd = fopen("./test.g711", "rb");               // g711a pcma
	if(!fd)
	{
		printf("error: can not open file !\n");

		return 0;
	}
	
	while( !feof(fd) )    
	{	
		read_len = fread(buf, 1, 320, fd);         
		//rtpPrintf(buf, read_len);

		status = sess.SendPacket(buf, read_len, 8, true, 320);  
		checkerror(status);

		RTPTime::Wait(0.03);                     
	} 	
	fclose(fd);

	return 0;
}
