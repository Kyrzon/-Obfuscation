#include "All.h"


Commands* StupidTrash(Commands* com)
{
	DWORD32 Number_Command_Trash = rand() % QUALITY;
	if (Number_Command_Trash == 0){ return com; }
	Pieces* P = com->P;
	Commands* command = new Commands[Number_Command_Trash];
	BYTE op1;
	BYTE op2;
	BYTE reg1;
	BYTE reg2;
	DWORD32 for_xor;
	DWORD32 j;
	DWORD32 i = 0;
	while (i < Number_Command_Trash)
	{
		if (Number_Command_Trash - i >= 2){ j = 4; }
		else{ j = 3; }
		switch (rand() % j)
		{
		case 0:										//Вставляем or
			op1 = OPCODE_OR;
			break;
		case 1:										//Вставляем and
			op1 = OPCODE_AND;
			break;
		case 2:										//Вставляем mov
			op1 = OPCODE_MOV;
			break;
		default:									//Вставляем 2 xor
			op1 = OPCODE_XOR;
			op2 = OPCODE_XOR;
			for_xor = rand();
			command[i].length = 5;
			command[i].bonus_offset = 5;
			command[i].command = new BYTE[5];
			command[i].command[0] = op1;
			*(long*)(command[i].command + 1) = for_xor;
			command[i].next = &command[i + 1];
			command[i].P = P;
			i++;
			if (i == 0)
			{
				command[i].prev = com;
			}
			else{
				command[i].prev = &command[i - 1];
			}
			command[i].length = 5;
			command[i].bonus_offset = 5;
			command[i].command = new BYTE[5];
			command[i].command[0] = op2;
			*(long*)(command[i].command + 1) = for_xor;
			if (i < Number_Command_Trash - 1){
				command[i].next = &command[i + 1];
			}
			else{ command[i].next = 0; }
			command[i].prev = &command[i - 1];
			command[i].P = P;
			i++;
			break;
		}
		if (op1 != OPCODE_XOR)
		{
			switch (rand() % 3)
			{
			case 0:										//Вставляем or
				op2 = OPCODE_OR;
				break;
			case 1:										//Вставляем and
				op2 = OPCODE_AND;
				break;
			default:									//Вставляем mov
				op2 = OPCODE_MOV;
				break;
			}
		}
		if (op1 != 0 && op1 != OPCODE_XOR){
			switch (rand() % 6)
			{
			case 0:										//eax
				reg1 = 0xC0;
				break;
			case 1:										//ebx
				reg1 = 0xDB;
				break;
			case 2:										//ecx
				reg1 = 0xC9;
				break;
			case 3:										//edx
				reg1 = 0xD2;
				break;
			case 4:										//esp
				reg1 = 0xE4;
				break;
			case 5:										//ebp
				reg1 = 0xED;
				break;
			default:
				reg1 = 0;
				break;
			}
		}
		if (op1 != OPCODE_XOR && op2 != 0)
		{
			switch (rand() % 6)
			{
			case 0:										//eax
				reg2 = 0xC0;
				break;
			case 1:										//ebx
				reg2 = 0xDB;
				break;
			case 2:										//ecx
				reg2 = 0xC9;
				break;
			case 3:										//edx
				reg2 = 0xD2;
				break;
			case 4:										//esp
				reg2 = 0xE4;
				break;
			case 5:										//ebp
				reg2 = 0xED;
				break;
			default:
				break;
			}
		}
		if (op1 != 0 && op1 != OPCODE_XOR)	//Записываем код
		{
			command[i].length = 2;
			command[i].bonus_offset = 2;
			command[i].command = new BYTE[2];
			command[i].command[0] = op1;
			command[i].command[1] = reg1;
			if (i < Number_Command_Trash - 1){
				command[i].next = &command[i + 1];
			}
			else{ command[i].next = 0; }
			if (i == 0)
			{
				command[i].prev = com;
			}
			else{
				command[i].prev = &command[i - 1];
			}
			command[i].P = P;
			i++;
		}
		if (i < Number_Command_Trash)
		{
			if (op2 != 0 && op2 != OPCODE_XOR){//Записываем код
				command[i].length = 2;
				command[i].bonus_offset = 2;
				command[i].command = new BYTE[2];
				command[i].command[0] = op2;
				command[i].command[1] = reg2;
				if (i < Number_Command_Trash - 1){
					command[i].next = &command[i + 1];
				}
				else{ command[i].next = 0; }
				if (i == 0)
				{
					command[i].prev = com;
				}
				else{
					command[i].prev = &command[i - 1];
				}
				command[i].P = P;
				i++;
			}
		}
	}
	if (Number_Command_Trash>0){
		command[i - 1].next = com->next;
		command[0].prev = com;
		com->next = &command[0];
		if (command[i - 1].next != 0){
			command[i - 1].next->prev = &command[i - 1];
		}
	}
	return &command[i - 1]; //Чтобы присвоить это команде и присоединять мусор уже следующей
}
/****************************************************************************
Вставка мусорных команд
****************************************************************************/
Commands* ReplaceMov(Commands* com)
{
	Commands* command = new Commands[3];
	/*; mov ebp, esp
		jmp  _over1
	_mov :
	mov ebp, esp
		jmp _quit1
	_over1 :
	jmp _mov
	_quit1 :*/
	command[0].length = LENGTH_JMP;
	command[0].bonus_offset = LENGTH_JMP;
	command[0].command = new BYTE[LENGTH_JMP];
	command[0].command[0] = OPCODE_JMP;
	command[0].jmp = &command[2];
	command[0].prev = com->prev;
	command[0].next = com;

	command[1].length = LENGTH_JMP;
	command[1].bonus_offset = LENGTH_JMP;
	command[1].command = new BYTE[LENGTH_JMP];
	command[1].command[0] = OPCODE_JMP;
	command[1].jmp = com->next;
	command[1].prev = com;
	command[1].next = &command[2];

	command[2].length = LENGTH_JMP;
	command[2].bonus_offset = LENGTH_JMP;
	command[2].command = new BYTE[LENGTH_JMP];
	command[2].command[0] = OPCODE_JMP;
	command[2].jmp = com;
	command[2].prev = &command[1];
	command[2].next = com->next;

	if (com->next!=0)
	com->next->prev = &command[2];
	if (com->prev!=0)
	com->prev->next = &command[0];
	com->prev = &command[0];
	com->next = &command[1];
	return &command[2];
}


/****************************************************************************
Вставляем мусор в кусочки
****************************************************************************/
VOID InsertTrashInPieces(Commands* com)
{
	int i = 0;
		while (com->command[0] != OPCODE_RET)
		{
			if (com->command[0] == OPCODE_MOV)
			{
				com=ReplaceMov(com);
			}
			if (com->command[0] == 0x83 && com->command[1] >= 0xC0 && com->command[1]<=0xC5)	//add???
			{
				com = ReplaceMov(com);
			}
			if (com->command[0] == 0x83 && com->command[1] >= 0xE8 && com->command[1] <= 0xED)	//sub???
			{
				com = ReplaceMov(com);
			}
			/*if (com->command[0] == OPCODE_CMP)
			{

			}*/
			//switch (rand() % 1)
			//{
			//case 0:
			if (com->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && com->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
			{
				com = com->next->next;
			}
			if (com->command[0] != 0x3B && com->command[0] != 0x39)	//не cmp
			{
				com = StupidTrash(com);
			}
			com = com->next;
		}
	return ;
}

/****************************************************************************
Исправляем размер кусочков после обфусцирования
****************************************************************************/
VOID CountTheSize(Pieces* pieces)
{
	DWORD32 Number_Pieces = _msize(pieces) / sizeof(pieces[0]);
	Commands* command;
	DWORD32 size = 0;
	DWORD32 Number_Instruction;
	for (DWORD32 i = 0; i < Number_Pieces; i++)
	{
		command = pieces[i].first;
		Number_Instruction = 0;
		size = 0;
		while (1){
			if (command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && command->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
			{
				size += command->length;
				command = command->next;
				Number_Instruction++;
				size += command->length;
				command = command->next;
				Number_Instruction++;
				size += command->length;
				Number_Instruction++;
				break;
			}
			else if (command->command[0] == OPCODE_JMP || command->command[0] == OPCODE_RET || command->command[0] == OPCODE_CALL)
			{
				size += command->length;
				Number_Instruction++;
				break;
			}
			else
			{
				size += command->length;
				command = command->next;
				Number_Instruction++;
			}
		}
		pieces[i].Number_Instruction = Number_Instruction;
		pieces[i].size = size;

	}
	return;
}

VOID exchange(Pieces* pieces, Pieces* pieces1)
{
	Commands* com=pieces->com;
	Commands* first = pieces->first;
	Commands* jmp = pieces->jmp;
	DWORD32 Number_Instruction = pieces->Number_Instruction;
	Pieces* P = pieces->P;
	DWORD32 size = pieces->size;

	pieces->com = pieces1->com;
	pieces->first = pieces1->first;
	pieces->jmp = pieces1->jmp;
	pieces->Number_Instruction = pieces1->Number_Instruction;
	pieces->P = pieces1->P;
	pieces->size = pieces1->size;

	Commands* command = pieces->first;
	while (command->command[0] != OPCODE_JMP && command->command[0] != OPCODE_RET && command->command[0] != OPCODE_CALL &&
		(command->command[0] <= OPCODE_DOWN_CONDITIONAL_JMP || command->command[0] >= OPCODE_UP_CONDITIONAL_JMP))
	{
		command->P = pieces;
		command = command->next;
	}
	if (command->command[0] == OPCODE_JMP)
	{		
		command->P = pieces;
	}
	if (command->command[0] == OPCODE_CALL)
	{
		command->P = pieces;
	}
	if (command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && command->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
	{
		command->P = pieces;
		command->next->P = pieces;
		command->next->next->P = pieces;
	}
	if (command->command[0] == OPCODE_RET)
	{
		command->P = pieces;
	}

	pieces1->com = com;
	pieces1->first = first;
	pieces1->jmp = jmp;
	pieces1->Number_Instruction = Number_Instruction;
	pieces1->P = P;
	pieces1->size = size;
	
	command = pieces1->first;
	while (command->command[0] != OPCODE_JMP && command->command[0] != OPCODE_RET && command->command[0] != OPCODE_CALL &&
		(command->command[0] <= OPCODE_DOWN_CONDITIONAL_JMP || command->command[0] >= OPCODE_UP_CONDITIONAL_JMP))
	{
		command->P = pieces1;
		command = command->next;
	}
	if (command->command[0] == OPCODE_JMP)
	{
		command->P = pieces1;
	}
	if (command->command[0] == OPCODE_CALL)
	{
		command->P = pieces1;
	}
	if (command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && command->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
	{
		command->P = pieces1;
		command->next->P = pieces1;
		command->next->next->P = pieces1;
	}
	if (command->command[0] == OPCODE_RET)
	{
		command->P = pieces1;
	}
	return;
}

VOID MixPieces(Pieces* pieces)
{
	DWORD32 k, k1;
	DWORD32 count = 10; //= rand() % QUALITY;
	DWORD32 Number_Pieces = _msize(pieces) / sizeof(pieces[0]);
	for (DWORD32 i = 0; i < Number_Pieces; i++)
	{
		k = rand() % Number_Pieces;
		k1 = rand() % Number_Pieces;
		if ( k!=k1 && k!=0 && k1!=0)
			exchange(&pieces[k], &pieces[k1]);
	}
	return;
}
