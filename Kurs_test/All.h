#ifndef	_ALL_
#define _ALL_

#include <windows.h>
#include <iostream>
#include "hde32.h"
#include <stdio.h>
#include <fstream>
#include <time.h> 
#include <math.h>

#define LENGTH_JMP_SH 2
#define LENGTH_JMP 5
#define QUALITY 5
#define OPCODE_CALL 0xE8
#define OPCODE_JMP 0xE9
#define OPCODE_SHORT_JMP 0xEB
#define OPCODE_RET 0xC3
#define OPCODE_UP_CONDITIONAL_JMP	0x7F
#define OPCODE_DOWN_CONDITIONAL_JMP	0x70
#define OPCODE_XOR 0x35
#define OPCODE_OR 0x09
#define OPCODE_AND 0x21
#define OPCODE_MOV 0x89

/****************************************************************************
��������� ������
****************************************************************************/
struct Commands{
	DWORD32 length;			//����� �������
	DWORD32 offset;			//�������� � �������
	struct Commands *jmp;	//�������, �� ������� ��������� jmp
	struct Commands *next;	//��������� �� ��������� �������
	struct Commands *prev;	//��������� �� ���������� �������
	BYTE* command;			//���� �������
	struct Pieces * P;		//�������, �������� ����������� �������
	DWORD32 bonus_offset;
};
/****************************************************************************
��������� �������� ��������
****************************************************************************/
struct Pieces{
	struct Commands *first;	//��������� �� ������ �������
	struct Commands *jmp;	//��������� �� ��������� �������
	DWORD32 size;			//������ �����
	DWORD32 Number_Instruction;	//���������� ����������
	struct Pieces *P;		//�������, �� ������� ��������� jmp
	struct Commands *com;	//������� � ���� �������, �� ������� ��������� jmp
};
#endif

//������� call
//������� ������(�->asm)
//��������� ����������