#pragma once

//#include <>

// Mayer's Singleton Template
class CClientTest
{
private:
	CClientTest();
	~CClientTest();
	CClientTest(const CClientTest&);
	CClientTest& operator=(const CClientTest&);
public:
	static CClientTest& getInstance();

	void startSim();
	void pauseSim();

};