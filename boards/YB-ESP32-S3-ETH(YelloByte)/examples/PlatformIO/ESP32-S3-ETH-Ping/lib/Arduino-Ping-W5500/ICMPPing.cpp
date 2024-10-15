/*
 * Copyright (c) 2010 by Blake Foster <blfoster@vassar.edu>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "ICMPPing.h"
#include <util.h>

#ifdef ICMPPING_INSERT_YIELDS
#define ICMPPING_DOYIELD()		delay(2)
#else
#define ICMPPING_DOYIELD()
#endif


inline uint16_t _makeUint16(const uint8_t& highOrder, const uint8_t& lowOrder)
{
    // make a 16-bit unsigned integer given the low order and high order bytes.
    // lowOrder first because the Arduino is little endian.
    uint8_t value [] = {lowOrder, highOrder};
    return *(uint16_t *)&value;
}

uint16_t _checksum(const ICMPEcho& echo)
{
    // calculate the checksum of an ICMPEcho with all fields but icmpHeader.checksum populated
    unsigned long sum = 0;

    // add the header, bytes reversed since we're using little-endian arithmetic.
    sum += _makeUint16(echo.icmpHeader.type, echo.icmpHeader.code);

    // add id and sequence
    sum += echo.id + echo.seq;

    // add time, one half at a time.
    uint16_t const * time = (uint16_t const *)&echo.time;
    sum += *time + *(time + 1);
    
    // add the payload
    for (uint8_t const * b = echo.payload; b < echo.payload + sizeof(echo.payload); b+=2)
    {
        sum += _makeUint16(*b, *(b + 1));
    }

    // ones complement of ones complement sum
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

ICMPEcho::ICMPEcho(uint8_t type, uint16_t _id, uint16_t _seq, uint8_t * _payload)
: seq(_seq), id(_id), time(millis())
{
    memcpy(payload, _payload, REQ_DATASIZE);
    icmpHeader.type = type;
    icmpHeader.code = 0;
    icmpHeader.checksum = _checksum(*this);
}

ICMPEcho::ICMPEcho()
: seq(0), id(0), time(0)
{
    memset(payload, 0, sizeof(payload));
    icmpHeader.code = 0;
    icmpHeader.type = 0;
    icmpHeader.checksum = 0;
}

void ICMPEcho::serialize(uint8_t * binData) const
{
    *(binData++) = icmpHeader.type;
    *(binData++) = icmpHeader.code;

    *(uint16_t *)binData = htons(icmpHeader.checksum); binData += 2;
    *(uint16_t *)binData = htons(id);                  binData += 2;
    *(uint16_t *)binData = htons(seq);                 binData += 2;
    *(icmp_time_t *)  binData = htonl(time);                binData += 4;

    memcpy(binData, payload, sizeof(payload));
}

void ICMPEcho::deserialize(uint8_t const * binData)
{
    icmpHeader.type = *(binData++);
    icmpHeader.code = *(binData++);

    icmpHeader.checksum = ntohs(*(uint16_t *)binData); binData += 2;
    id                  = ntohs(*(uint16_t *)binData); binData += 2;
    seq                 = ntohs(*(uint16_t *)binData); binData += 2;

    if (icmpHeader.type != TIME_EXCEEDED)
    {
        time = ntohl(*(icmp_time_t *)binData);   binData += 4;
    }

    memcpy(payload, binData, sizeof(payload));
}


uint16_t ICMPPing::ping_timeout = PING_TIMEOUT;

ICMPPing::ICMPPing(SOCKET socket, uint8_t id) :
#ifdef ICMPPING_ASYNCH_ENABLE
  _curSeq(0), _numRetries(0), _asyncstart(0), _asyncstatus(BAD_RESPONSE),
#endif
  _id(id), _nextSeq(0), _socket(socket),  _attempt(0)
{
    memset(_payload, 0x1A, REQ_DATASIZE);
}


void ICMPPing::setPayload(uint8_t * payload)
{
	memcpy(_payload, payload, REQ_DATASIZE);
}

void ICMPPing::openSocket()
{

	w5500.execCmdSn(_socket, Sock_CLOSE);
    w5500.writeSnIR(_socket, 0xFF);
    w5500.writeSnMR(_socket, SnMR::IPRAW);
    w5500.writeSnPROTO(_socket, IPPROTO::ICMP);
    w5500.writeSnPORT(_socket, 0);
    w5500.execCmdSn(_socket, Sock_OPEN);
}



void ICMPPing::operator()(const IPAddress& addr, int nRetries, ICMPEchoReply& result)
{
	openSocket();

    ICMPEcho echoReq(ICMP_ECHOREQ, _id, _nextSeq++, _payload);

    for (_attempt=0; _attempt<nRetries; ++_attempt)
    {

    	ICMPPING_DOYIELD();

        result.status = sendEchoRequest(addr, echoReq);
        if (result.status == SUCCESS)
        {
            byte replyAddr [4];
        	ICMPPING_DOYIELD();
            receiveEchoReply(echoReq, addr, result);
        }
        if (result.status == SUCCESS)
        {
            break;
        }
    }
   
    w5500.execCmdSn(_socket, Sock_CLOSE);
    w5500.writeSnIR(_socket, 0xFF);
}

ICMPEchoReply ICMPPing::operator()(const IPAddress& addr, int nRetries)
{
    ICMPEchoReply reply;
    operator()(addr, nRetries, reply);
    return reply;
}

Status ICMPPing::sendEchoRequest(const IPAddress& addr, const ICMPEcho& echoReq)
{
    // I wish there were a better way of doing this, but if we use the uint32_t
    // cast operator, we're forced to (1) cast away the constness, and (2) deal
    // with an endianness nightmare.
    uint8_t addri [] = {addr[0], addr[1], addr[2], addr[3]};
    w5500.writeSnDIPR(_socket, addri);
    w5500.writeSnTTL(_socket, 128);
    // The port isn't used, becuause ICMP is a network-layer protocol. So we
    // write zero. This probably isn't actually necessary.
    w5500.writeSnDPORT(_socket, 0);

    uint8_t serialized [sizeof(ICMPEcho)];
    echoReq.serialize(serialized);

    w5500.send_data_processing(_socket, serialized, sizeof(ICMPEcho));
    w5500.execCmdSn(_socket, Sock_SEND);

    while ((w5500.readSnIR(_socket) & SnIR::SEND_OK) != SnIR::SEND_OK) 
    {
        if (w5500.readSnIR(_socket) & SnIR::TIMEOUT)
        {
            w5500.writeSnIR(_socket, (SnIR::SEND_OK | SnIR::TIMEOUT));
            return SEND_TIMEOUT;
        }

        ICMPPING_DOYIELD();
    }
    w5500.writeSnIR(_socket, SnIR::SEND_OK);
    return SUCCESS;
}

void ICMPPing::receiveEchoReply(const ICMPEcho& echoReq, const IPAddress& addr, ICMPEchoReply& echoReply)
{
    icmp_time_t start = millis();
    while (millis() - start < ping_timeout)
    {

        if (w5500.getRXReceivedSize(_socket) < 1)
        {
        	// take a break, maybe let platform do
        	// some background work (like on ESP8266)
        	ICMPPING_DOYIELD();
        	continue;
        }

        // ah! we did receive something... check it out.

        uint8_t ipHeader[6];
		uint8_t buffer = w5500.readSnRX_RD(_socket);
		w5500.read_data(_socket, (uint16_t) buffer, ipHeader, sizeof(ipHeader));
		buffer += sizeof(ipHeader);
		for (int i = 0; i < 4; ++i)
			echoReply.addr[i] = ipHeader[i];
		uint8_t dataLen = ipHeader[4];
		dataLen = (dataLen << 8) + ipHeader[5];

		uint8_t serialized[sizeof(ICMPEcho)];
		if (dataLen > sizeof(ICMPEcho))
			dataLen = sizeof(ICMPEcho);
		w5500.read_data(_socket, (uint16_t) buffer, serialized, dataLen);
		echoReply.data.deserialize(serialized);

		buffer += dataLen;
		w5500.writeSnRX_RD(_socket, buffer);
		w5500.execCmdSn(_socket, Sock_RECV);

		echoReply.ttl = w5500.readSnTTL(_socket);

		// Since there aren't any ports in ICMP, we need to manually inspect the response
		// to see if it originated from the request we sent out.
		switch (echoReply.data.icmpHeader.type) {
		case ICMP_ECHOREP: {
			if (echoReply.data.id == echoReq.id
					&& echoReply.data.seq == echoReq.seq) {
				echoReply.status = SUCCESS;
				return;
			}
			break;
		}
		case TIME_EXCEEDED: {
			uint8_t * sourceIpHeader = echoReply.data.payload;
			unsigned int ipHeaderSize = (sourceIpHeader[0] & 0x0F) * 4u;
			uint8_t * sourceIcmpHeader = echoReply.data.payload + ipHeaderSize;

			// The destination ip address in the originating packet's IP header.
			IPAddress sourceDestAddress(sourceIpHeader + ipHeaderSize - 4);

			if (!(sourceDestAddress == addr))
				continue;

			uint16_t sourceId = ntohs(*(uint16_t * )(sourceIcmpHeader + 4));
			uint16_t sourceSeq = ntohs(*(uint16_t * )(sourceIcmpHeader + 6));

			if (sourceId == echoReq.id && sourceSeq == echoReq.seq) {
				echoReply.status = BAD_RESPONSE;
				return;
			}
			break;
		}
		}


    }
    echoReply.status = NO_RESPONSE;
}



#ifdef ICMPPING_ASYNCH_ENABLE
/*
 * When ICMPPING_ASYNCH_ENABLE is defined, we have access to the
 * asyncStart()/asyncComplete() methods from the API.
 */
bool ICMPPing::asyncSend(ICMPEchoReply& result)
{
    ICMPEcho echoReq(ICMP_ECHOREQ, _id, _curSeq, _payload);

    Status sendOpResult(NO_RESPONSE);
    bool sendSuccess = false;
    for (uint8_t i=_attempt; i<_numRetries; ++i)
    {
    	_attempt++;

    	ICMPPING_DOYIELD();
    	sendOpResult = sendEchoRequest(_addr, echoReq);
    	if (sendOpResult == SUCCESS)
    	{
    		sendSuccess = true; // it worked
    		sendOpResult = ASYNC_SENT; // we're doing this async-style, force the status
    		_asyncstart = millis(); // not the start time, for timeouts
    		break; // break out of this loop, 'cause we're done.

    	}
    }
    _asyncstatus = sendOpResult; // keep track of this, in case the ICMPEchoReply isn't re-used
    result.status = _asyncstatus; // set the result, in case the ICMPEchoReply is checked
    return sendSuccess; // return success of send op
}
bool ICMPPing::asyncStart(const IPAddress& addr, int nRetries, ICMPEchoReply& result)
{
	openSocket();

	// stash our state, so we can access
	// in asynchSend()/asyncComplete()
	_numRetries = nRetries;
	_attempt = 0;
	_curSeq = _nextSeq++;
	_addr = addr;

	return asyncSend(result);

}

bool ICMPPing::asyncComplete(ICMPEchoReply& result)
{

	if (_asyncstatus != ASYNC_SENT)
	{
		// we either:
		//  - didn't start an async request;
		//	- failed to send; or
		//	- are no longer waiting on this packet.
		// either way, we're done
		return true;
	}


	if (w5500.getRXReceivedSize(_socket))
	{
		// ooooh, we've got a pending reply
	    ICMPEcho echoReq(ICMP_ECHOREQ, _id, _curSeq, _payload);
		receiveEchoReply(echoReq, _addr, result);
		_asyncstatus = result.status; // make note of this status, whatever it is.
		return true; // whatever the result of the receiveEchoReply(), the async op is done.
	}

	// nothing yet... check if we've timed out
	if ( (millis() - _asyncstart) > ping_timeout)
	{

		// yep, we've timed out...
		if (_attempt < _numRetries)
		{
			// still, this wasn't our last attempt, let's try again
			if (asyncSend(result))
			{
				// another send has succeeded
				// we'll wait for that now...
				return false;
			}

			// this send has failed. too bad,
			// we are done.
			return true;
		}

		// we timed out and have no more attempts left...
		// hello?  is anybody out there?
		// guess not:
	    result.status = NO_RESPONSE;
	    return true;
	}

	// have yet to time out, will wait some more:
	return false; // results still not in

}

#endif	/* ICMPPING_ASYNCH_ENABLE */



