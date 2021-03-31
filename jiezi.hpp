#ifndef _JIEZI_H
#define _JIEZI_H

class Jiezi
{
private:
	unsigned short op;	//������
	unsigned char memory[4096];	//�ڴ�4KB
	unsigned char reg[16];	//�Ĵ���16��
	unsigned short ip;	//ָ��ָ��Ĵ���
	unsigned short pc;	//��������� 
	unsigned char timer[2];	//��ʱ��
	unsigned short stack[16];	//��ջ
	unsigned short sp; 	//��ջָ�� 
public:
	bool drawflag;
	unsigned char video_memory[2048];	//�Դ� 
	unsigned char Keyboard[16];
	Jiezi();
	virtual ~Jiezi();
	void initialize();	//��ʼ��CPU
	void fetch();	//ȡָ��
	void execute();	//ִ��ָ�� 
	void cpu_cycle();	//CPU����
	void clock();
	void write(int ins, int addr);	//д�ڴ�
	void read(int addr, int lenth);
	bool load_programe(const char* filename);	//���س��� 
	
	//΢����
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
