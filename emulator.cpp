#include "Jiezi.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	Jiezi *cpu = new Jiezi;
	int ins = 0;
	int addrw = 0x200;
	int addrr, lenth;
	char cmd;
	cpu->initialize();
	while (1)
	{
		cout << "input instruction:";
		cin >> hex >> ins;
		cpu->write(ins, addrw);
		addrw += 2;
		cpu->cpu_cycle();
		cout << "view data:y/n  ";
		cin >> cmd;
		if (cmd == 'y')
		{
			cout << "input addr and lenth:";
			cin >> hex >> addrr >> lenth;
			cpu->read(addrr, lenth);
		}
	}
}
