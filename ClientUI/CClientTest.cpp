#include "pch.h"
#include "CClientTest.h"

CClientTest::CClientTest()
{
	OutputDebugString(TEXT("A client is created!"));
}

CClientTest& CClientTest::operator=(const CClientTest&)
{
	return CClientTest::getInstance();
}

CClientTest& CClientTest::getInstance()
{
	static CClientTest instance;
	return instance;
}

void CClientTest::startSim()
{
	OutputDebugString(TEXT("Start simulation\r\n"));
}

void CClientTest::pauseSim()
{
	OutputDebugString(TEXT("Pause simulation"));
}

CClientTest::~CClientTest()
{
	OutputDebugString(TEXT("A client is deleted!"));
}
