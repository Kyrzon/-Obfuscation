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
Структура команд
****************************************************************************/
struct Commands{
	DWORD32 length;			//Длина команды
	DWORD32 offset;			//Смещение в кусочке
	struct Commands *jmp;	//Команда, на которую указывает jmp
	struct Commands *next;	//Указатель на следующую команду
	struct Commands *prev;	//Указатель на предыдущую команду
	BYTE* command;			//сама команда
	struct Pieces * P;		//кусочек, которому принадлежит команда
	DWORD32 bonus_offset;
};
/****************************************************************************
Структура линейных кусочков
****************************************************************************/
struct Pieces{
	struct Commands *first;	//Указатель на первую команду
	struct Commands *jmp;	//Указатель на последнюю команду
	DWORD32 size;			//Размер блока
	DWORD32 Number_Instruction;	//Количество инструкций
	struct Pieces *P;		//Кусочек, на который указывает jmp
	struct Commands *com;	//Команда в этом кусочке, на который указывает jmp
};
#endif

//сделать call
//Сложный пример(с->asm)
//Усложнить обфускацию