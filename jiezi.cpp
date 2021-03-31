#include "jiezi.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>

unsigned char font[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Jiezi::Jiezi()
{
	mircoop_table[0] = &Jiezi::cpu0;
	mircoop_table[1] = &Jiezi::jmp;
	mircoop_table[2] = &Jiezi::jsr;
	mircoop_table[3] = &Jiezi::skeqxr;
	mircoop_table[4] = &Jiezi::sknexr;
	mircoop_table[5] = &Jiezi::skeqxy;
	mircoop_table[6] = &Jiezi::movxr;
	mircoop_table[7] = &Jiezi::addxr;
	mircoop_table[8] = &Jiezi::cpu8;
	mircoop_table[9] = &Jiezi::sknexy;
	mircoop_table[10] = &Jiezi::mvi;
	mircoop_table[11] = &Jiezi::jmi;
	mircoop_table[12] = &Jiezi::randxk;
	mircoop_table[13] = &Jiezi::spritexyn;
	mircoop_table[14] = &Jiezi::cpue;
	mircoop_table[15] = &Jiezi::cpuf;
	
	for (int i = 0; i < 16; i++) mircoop_0[i] = &Jiezi::cpuNull;
	mircoop_0[0] = &Jiezi::cls;
	mircoop_0[14] = &Jiezi::rts;

	for (int i = 0; i < 16; i++) mircoop_8[i] = &Jiezi::cpuNull;
	mircoop_8[0] = &Jiezi::movxy;
	mircoop_8[1] = &Jiezi::orxy;
	mircoop_8[2] = &Jiezi::andxy;
	mircoop_8[3] = &Jiezi::xorxy;
	mircoop_8[4] = &Jiezi::addxy;
	mircoop_8[5] = &Jiezi::subxy;
	mircoop_8[6] = &Jiezi::shrx;
	mircoop_8[7] = &Jiezi::rsbxy;
	mircoop_8[14] = &Jiezi::shlx;

	for (int i = 0; i < 16; i++) mircoop_e[i] = &Jiezi::cpuNull;
	mircoop_e[1] = &Jiezi::skupk;
	mircoop_e[14] = &Jiezi::skprk;

	for (int i = 0; i < 16; i++) mircoop_f[i] = &Jiezi::cpuNull;
	mircoop_f[3] = &Jiezi::bcdr;
	mircoop_f[5] = &Jiezi::cpuf5;
	mircoop_f[7] = &Jiezi::gdelayr;
	mircoop_f[8] = &Jiezi::ssoundr;
	mircoop_f[9] = &Jiezi::fontr;
	mircoop_f[10] = &Jiezi::keyr;
	mircoop_f[14] = &Jiezi::adir;

	for (int i = 0; i < 7; i++) mircoop_f5[i] = &Jiezi::cpuNull;
	mircoop_f5[1] = &Jiezi::sdelayr;
	mircoop_f5[5] = &Jiezi::str;
	mircoop_f5[6] = &Jiezi::ldr;
}

Jiezi::~Jiezi()
{
	
}

void Jiezi::initialize()
{
	pc = 0x200;	//rom位置 
	op = 0;	//初始化操作码 
	ip = 0;	//初始化指令指针寄存器 
	sp = 0;//初始化栈顶指针

	for (int i = 0; i < 16; i++)
	{
		reg[i] = 0;
	}
	
	for(int i=0;i<2048;i++)
	{
		video_memory[i] = 0;
	}
	
	for(int i=0;i<16;i++)
	{
		stack[i] = 0;
	}
	
	for(int i=0;i<4096;i++)
	{
		memory[i] = 0;
	}
	
	timer[0] = 0;
	timer[1] = 0;
	
	for(int i=0;i<80;i++)
	{
		memory[i] = font[i];
	}
}

void Jiezi::cls()
{
	for(int i=0;i<2048;i++)
	{
		video_memory[i] = 0;
	}
	drawflag = true;
	pc += 2;
}

void Jiezi::rts()
{
	sp--;
	pc = stack[sp];
	pc += 2;
}

void Jiezi::jmp()
{
	pc = op & 0x0fff;
}

void Jiezi::jsr()
{
	stack[sp] = pc;
	sp++;
	pc = op & 0xfff;
}

void Jiezi::skeqxr()
{
	if(reg[(op&0x0f00)>>8] == (op&0x00ff)) pc += 4;
	else pc += 2; 
}

void Jiezi::sknexr()
{
	if(reg[(op&0x0f00)>>8] != (op&0x00ff)) pc += 4;
	else pc += 2; 
}

void Jiezi::skeqxy()
{
	if(reg[(op&0x0f00)>>8] == reg[(op&0x00f0)>>4]) pc += 4;
	else pc += 2;
}

void Jiezi::movxr()
{
	reg[(op&0x0f00)>>8] = op&0x00ff;
	pc += 2;
}

void Jiezi::addxr()
{
	reg[(op&0x0f00)>>8] += op&0x00ff;
	pc += 2;
}

void Jiezi::movxy()
{
	reg[(op&0x0f00)>>8] = reg[(op&0x00f0)>>4];
	pc += 2;
}

void Jiezi::orxy()
{
	reg[(op&0x0f00)>>8] |= reg[(op&0x00f0)>>4];
	pc += 2;
}

void Jiezi::andxy()
{
	reg[(op&0x0f00)>>8] &= reg[(op&0x00f0)>>4];
	pc += 2;
}

void Jiezi::xorxy()
{
	reg[(op&0x0f00)>>8] ^= reg[(op&0x00f0)>>4];
	pc += 2;
}

void Jiezi::addxy()
{
	reg[(op&0x0f00)>>8] += reg[(op&0x00f0)>>4];
	if(reg[(op&0x0f00)>>8] + reg[(op&0x00f0)>>4] > 255) reg[15] = 1;
	else reg[15] = 0;
	pc += 2;
}

void Jiezi::subxy()
{
	reg[(op&0x0f00)>>8] -= reg[(op&0x00f0)>>4];
	if(reg[(op&0x0f00)>>8] < reg[(op&0x00f0)>>4]) reg[15] = 0;
	else reg[15] = 1;
	pc += 2;
}

void Jiezi::shrx()
{
	reg[15] = reg[(op&0x0f00)>>8]&0x0001;
	reg[(op&0x0f00)>>8] >>= 1;
	pc += 2;
}

void Jiezi::rsbxy()
{
	reg[(op&0x0f00)>>8] = reg[(op&0x00f0)>>4] - reg[(op&0x0f00)>>8];
	if(reg[(op&0x0f00)>>8] < reg[(op&0x00f0)>>4]) reg[15] = 0;
	else reg[15] = 1;
	pc += 2;
}

void Jiezi::shlx()
{
	reg[15] = reg[(op&0x0f00)>>8]&0x1000;
	reg[(op&0x0f00)>>8] <<= 1;
	pc += 2;
}

void Jiezi::sknexy()
{
	if(reg[(op&0x0f00)>>8] != reg[(op&0x00f0)>>4]) pc += 4;
	else pc += 2;
}

void Jiezi::mvi()
{
	ip = op&0x0fff;
	pc += 2;
}

void Jiezi::jmi()
{
	pc = reg[0] + (op&0x0fff);
}

void Jiezi::randxk()
{
	reg[(op&0x0f00)>>8] = (rand()%0xff)&(op&0x00ff);
	pc += 2;
}

void Jiezi::spritexyn()
{
	unsigned short x = reg[(op & 0x0F00) >> 8];
    unsigned short y = reg[(op & 0x00F0) >> 4];
    unsigned short height = op & 0x000F;
    unsigned short pixel;
    unsigned int yline, xline;

    reg[15] = 0;
    for (yline = 0; yline < height; yline++)
    {
        pixel = memory[ip + yline];
        for(xline = 0; xline < 8; xline++)
        {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(video_memory[(x + xline + ((y + yline) * 64))] == 1)
                {
                    reg[15] = 1;                                    
                }
                video_memory[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }

    drawflag = 1;
    pc += 2;
}

void Jiezi::skprk()
{
	if(font[reg[(op&0x0f00)>>8]] != 0) pc += 4;
    else pc += 2;
}

void Jiezi::skupk()
{
	if(font[reg[(op&0x0f00)>>8]] == 0) pc += 4;
    else pc += 2;
}

void Jiezi::gdelayr()
{
	reg[(op&0x0f00)>>8] = timer[0];
	pc += 2;
}

void Jiezi::keyr()
{
	unsigned char keyPress = 0;
    for(int i = 0; i < 16; i++)
    {
        if(font[i] != 0)
        {
            reg[(op&0x0f00)>>8] = i;
            keyPress = 1;
        }
    }
    if(!keyPress)                       
        return;
    pc += 2;
}

void Jiezi::sdelayr()
{
	timer[0] = reg[(op&0x0f00)>>8];
	pc += 2;
}

void Jiezi::ssoundr()
{
	timer[1] = reg[(op&0x0f00)>>8];
	pc += 2;
}

void Jiezi::adir()
{
	if(ip + reg[(op&0x0F00)>>8] > 0xfff) reg[15] = 1;
    else reg[15] = 0;
    ip += reg[(op&0x0F00)>>8];
    pc += 2;
}

void Jiezi::fontr()
{
	ip = reg[(op&0x0F00)>>8] * 0x5;
	pc += 2;
}

void Jiezi::bcdr()
{
	memory[ip] = reg[(op&0x0F00)>>8]/100;
	memory[ip+1] = reg[(op&0x0F00)>>8]/10%10;
	memory[ip+2] = reg[(op&0x0F00)>>8]%100%10;
	pc += 2;
}

void Jiezi::str()
{
	for(int i=0;i<(op&0x0f00)>>8;i++)
	{
		memory[ip+i] = reg[i];
	}
	ip += (op&0x0f00)>>8+1;
	pc += 2;
}

void Jiezi::ldr()
{
	for(int i=0;i<(op&0x0f00)>>8;i++)
	{
		reg[i] = memory[ip+i];
	}
	ip += (op&0x0f00)>>8+1;
	pc += 2;
}

void Jiezi::clock()
{
	if(timer[0]>0) timer[0]--;
	if(timer[1]>0) timer[1]--;
}

void Jiezi::cpu0()
{
	(this->*mircoop_0[op&0x000f])();
}

void Jiezi::cpu8()
{
	(this->*mircoop_8[op&0x000f])();
}

void Jiezi::cpue()
{
	(this->*mircoop_e[op&0x000f])();
}

void Jiezi::cpuf()
{
	(this->*mircoop_f[op&0x000f])();
}

void Jiezi::cpuf5()
{
	(this->*mircoop_f5[(op&0x00f0)>>4])();
}

void Jiezi::fetch()
{
	op = (memory[pc]<<8) + memory[pc+1];
}

void Jiezi::execute()
{
	(this->*mircoop_table[(op & 0xf000) >> 12])();
}

void Jiezi::cpu_cycle()
{
	fetch();
	execute();
}

void Jiezi::cpuNull()
{
	
}

void Jiezi::write(int ins, int addr)
{
	memory[addr] = (ins&0xff00)>>8;
	memory[addr + 1] = (ins & 0x00ff);
}

void Jiezi::read(int addr, int lenth)
{
	for (int i = addr; i < addr + lenth; i++)
	{
		std::cout << "0x" << std::hex << int(memory[i]) << " ";
	}
	std::cout << std::endl;
}

bool Jiezi::load_programe(const char* filename)
{
	long lSize;
	char* buffer = NULL;
	FILE* pFile = NULL;
	size_t result;

	fopen_s(&pFile, filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
	}

	fseek(pFile, 0, SEEK_END);//移动指针到文件末尾
	lSize = ftell(pFile);//获得文件大小
	rewind(pFile);//移动指针到文件开头
	std::cout << "File size:" << (int)lSize << std::endl;

	buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return 0;
	}

	// Copy the file into the buffer
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return 0;
	}

	// Copy buffer to Chip8 memory
	if ((4096 - 512) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		std::cout << "Error: ROM too big for memory" << std::endl;

	fclose(pFile);
	free(buffer);

	return 1;
}
