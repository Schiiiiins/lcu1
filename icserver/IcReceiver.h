#pragma once

class IcReceiver
{
public:
	IcReceiver(void);
	~IcReceiver(void);
private:
	int _socket;
};
