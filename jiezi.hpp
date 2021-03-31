#ifndef _JIEZI_H
#define _JIEZI_H

class Jiezi
{
private:
	unsigned short op;	//操作码
	unsigned char memory[4096];	//内存4KB
	unsigned char reg[16];	//寄存器16个
	unsigned short ip;	//指令指针寄存器
	unsigned short pc;	//程序计数器 
	unsigned char timer[2];	//计时器
	unsigned short stack[16];	//堆栈
	unsigned short sp; 	//堆栈指针 
public:
	bool drawflag;
	unsigned char video_memory[2048];	//显存 
	unsigned char Keyboard[16];
	Jiezi();
	virtual ~Jiezi();
	void initialize();	//初始化CPU
	void fetch();	//取指令
	void execute();	//执行指令 
	void cpu_cycle();	//CPU周期
	void clock();
	void write(int ins, int addr);	//写内存
	void read(int addr, int lenth);
	bool load_programe(const char* filename);	//加载程序 
	
	//微操作
	void cls();
	void rts();
	void jmp();
	void jsr();
	void skeqxr();
	void sknexr();
	void skeqxy();
	void movxr();
	void addxr();
	void movxy();
	void orxy();
	void andxy();
	void xorxy();
	void addxy();
	void subxy();
	void shrx();
	void rsbxy();
	void shlx();
	void sknexy();
	void mvi();
	void jmi();
	void randxk();
	void spritexyn();
	void skprk();
	void skupk();
	void gdelayr();
	void keyr();
	void sdelayr();
	void ssoundr();
	void adir();
	void fontr();
	void bcdr();
	void str();
	void ldr();
	
	void (Jiezi::* mircoop_table[17])();
	void (Jiezi::* mircoop_0[17])();
	void (Jiezi::* mircoop_8[17])();
	void (Jiezi::* mircoop_e[17])();
	void (Jiezi::* mircoop_f[17])();
	void (Jiezi::* mircoop_f5[7])();
	
	void cpu0();
	void cpu8();
	void cpue();
	void cpuf();
	void cpuf5();
	void cpuNull();
};

#endif
