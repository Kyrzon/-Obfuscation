#include "All.h"


/****************************************************************************
��������� ������� �����
****************************************************************************/
DWORD32 MyGetFileSize(HANDLE hFile)
{
	DWORD dwSizeHigh = 0, dwSizeLow = 0;
	DWORD32 nFileLen = 0;
	dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
	nFileLen = (dwSizeHigh * (MAXDWORD + 1)) + dwSizeLow;
	return nFileLen;
}

/****************************************************************************
������������� ��������� ������� �������� �����
****************************************************************************/
BYTE* InitializationSourseCode(CHAR *NameFile)
{
	DWORD32 sizeFile;									//������ �����
	HANDLE hFile = CreateFileA(NameFile,				//��� �����
		GENERIC_READ,			//����� �������
		FILE_SHARE_READ,		//���������� ������
		NULL,					//SD(���������� ������)
		OPEN_EXISTING,			//��� �����������
		FILE_ATTRIBUTE_NORMAL,	//�������� �����
		NULL);					//���������� ������� �����
	if (hFile == INVALID_HANDLE_VALUE){ std::cout << "�� ���� ������� ����/n"; }	//��������: �������� �� ����?
	sizeFile = MyGetFileSize(hFile);					//�������� ������ �����	
	if (sizeFile == 0){ std::cout << "���� ����/n"; }	//��������: �� ���� �� ����?
	BYTE* code = new BYTE[sizeFile];					//��������� ��������� ������� ��� �������� ���
	DWORD Musor;										//���������� ��� ������. ������ �� ���� ��� ��� ��
	if (!ReadFile(hFile, code, sizeFile, &Musor, NULL))	//������ �� �����
	{
		std::cout << "��������...\n";
		GetLastError();
	}
	CloseHandle(hFile);
	return code;
}






/****************************************************************************
��������� ���������� ���������� � �������
****************************************************************************/
DWORD32 GetNumberInstruction(BYTE*code)
{
	DWORD32 count = 0;
	DWORD32 length;
	DWORD32 count_Instruction = 0;
	hde32s hs;
	DWORD32 size_File = _msize(code) / sizeof(code[0]);
	while (count < size_File) {
		length = hde32_disasm(code + count, &hs);
		count += length;
		if (hs.flags & F_ERROR)
		{
			printf("������ ����������!\n");
			system("pause");
			return -1;
		}
		if ((code + count)[0] <= OPCODE_UP_CONDITIONAL_JMP && (code + count)[0] >= OPCODE_DOWN_CONDITIONAL_JMP)
		{
			count_Instruction+=2;
		}
		count_Instruction++;
	}
	return count_Instruction;
}
/****************************************************************************
������������� ������� �������� ������
****************************************************************************/
Commands* InitializationAllCommands(BYTE* code)
{
	DWORD32 count = 0;
	DWORD32 length;
	DWORD32 Number_Instruction = GetNumberInstruction(code);
	hde32s hs;
	Commands* command = new Commands[Number_Instruction];
	DWORD32 count_Instruction = 0;
	DWORD32 count_Real_Instruction = 0;
	while (1)
	{
		length = hde32_disasm(code + count, &hs);
		if (hs.flags & F_ERROR)
		{
			printf("������ ����������!\n");
			system("pause");
			return 0;
		}
		else
		{
			if (hs.opcode == OPCODE_SHORT_JMP)	//���� ��������� �������� �����, �� �������� ��� �� �����
			{

				count_Real_Instruction++;
				command[count_Instruction].length = LENGTH_JMP;
				command[count_Instruction].command = new BYTE[LENGTH_JMP];
				LONG32 jmp = (CHAR)(code + count)[1];
				command[count_Instruction].command[0] = OPCODE_JMP;
				*(LONG32*)(command[count_Instruction].command + 1) = jmp;
				command[count_Instruction].bonus_offset = 3;
				if (count_Instruction == 0)
				{
					command[count_Instruction].prev = 0;
				}
				else{
					command[count_Instruction].prev = &command[count_Instruction - 1];
				}
				if (count_Real_Instruction == Number_Instruction)
				{
					command[count_Instruction].next = 0;
					return command;
				}
				command[count_Instruction].next = &command[count_Instruction + 1];
				count_Instruction++;
				count += LENGTH_JMP_SH;
			}
			else if (hs.opcode <= OPCODE_UP_CONDITIONAL_JMP && hs.opcode >= OPCODE_DOWN_CONDITIONAL_JMP)
			{
				count_Real_Instruction++;
				//������������ �������� jmp
				LONG32 jmp = (CHAR)(code + count)[1];
				command[count_Instruction].length = length;
				command[count_Instruction].command = new BYTE[length];
				command[count_Instruction].command[0] = hs.opcode;
				command[count_Instruction].command[1] = LENGTH_JMP;
				if (count_Instruction == 0)
				{
					command[count_Instruction].prev = 0;
				}
				else{
					command[count_Instruction].prev = &command[count_Instruction - 1];
				}
				command[count_Instruction].next = &command[count_Instruction+1];
				command[count_Instruction].bonus_offset = 0;
				count += LENGTH_JMP_SH;
				count_Instruction++;
				//��������� �����, ������� ����� �����������, ���� �������� �� �����������
				count_Real_Instruction++;
				command[count_Instruction].length = LENGTH_JMP;
				command[count_Instruction].command = new BYTE[LENGTH_JMP];
				command[count_Instruction].command[0] = OPCODE_JMP;
				*(LONG32*)(command[count_Instruction].command + 1) = LENGTH_JMP;
				command[count_Instruction].next = &command[count_Instruction+1];
				command[count_Instruction].prev = &command[count_Instruction - 1];
				command[count_Instruction].bonus_offset = 5;
				count_Instruction++;
				//��������� �����, �� ������� ����� ���������� �������� �����, � �������������� ���
				count_Real_Instruction++;
				command[count_Instruction].length = LENGTH_JMP;
				command[count_Instruction].command = new BYTE[LENGTH_JMP];
				command[count_Instruction].command[0] = OPCODE_JMP;
				if (jmp < 0)	//���������� ��������, �����������, ������ ������������ ��������
				{
					*(LONG32*)(command[count_Instruction].command + 1) = jmp;// - LENGTH_JMP_SH;// - LENGTH_JMP;
				}
				else
				{
					*(LONG32*)(command[count_Instruction].command + 1) = jmp;
				}
				command[count_Instruction].prev = &command[count_Instruction - 1];
				command[count_Instruction].bonus_offset = 5;
				if (count_Real_Instruction == Number_Instruction)
				{
					command[count_Instruction].next = 0;
					return command;
				}
				command[count_Instruction].next = &command[count_Instruction + 1];
				count_Instruction++;
			}
			else{
				count_Real_Instruction++;
				command[count_Instruction].length = length;
				command[count_Instruction].command = new BYTE[length];
				for (DWORD32 i = 0; i < length; i++){
					command[count_Instruction].command[i] = (code + count)[i];
				}
				if (count_Instruction == 0)
				{ 
					command[count_Instruction].prev = 0; 
				}
				else{
					command[count_Instruction].prev = &command[count_Instruction - 1];
				}
				if (count_Real_Instruction == Number_Instruction)
				{
					command[count_Instruction].next = 0;
					return command;
				}
				command[count_Instruction].next = &command[count_Instruction + 1];
				command[count_Instruction].bonus_offset = 0;
				count_Instruction++;
				count += length;
			}
		}
	}
}

DWORD32 GetNumberPieces(Commands* command)
{
	DWORD32 NumberJMP = 0;
	while (command->next != 0)
	{
		if (command->command[0] == OPCODE_JMP)
		{
			NumberJMP++;
		}
		if (command->command[0] == OPCODE_CALL)
		{
			NumberJMP++;
		}
		if (command->command[0] <= OPCODE_UP_CONDITIONAL_JMP && command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP)
		{
			NumberJMP--;
		}
		if (command->command[0] == OPCODE_RET)
		{
			NumberJMP++;
		}
		command = command->next;
	}
	NumberJMP++;
	return NumberJMP;
}





/****************************************************************************
����������  jmp-�
****************************************************************************/
VOID CorrectionJMP(Commands* command)
{
	Commands* buf = 0;
	Commands* next;
	while (command->next != 0)
	{
		command->jmp = 0;
		if (command->command[0] == OPCODE_JMP || command->command[0] == OPCODE_CALL)
		{
			LONG32 jmp = (CHAR)command->command[1];
			if (jmp < 0){
				LONG32 length = 0;
				buf = command;
				while (length > jmp)
				{
					jmp -= buf->bonus_offset;	//��������� �������������� �������� �������
					length -= buf->length;	//������� �� ����� ����� �������
					buf=buf->prev;
				}
				buf=buf->next;
				command->jmp = buf;
			}
			else
			{
				LONG32 length = 0;
				buf = command;
				buf = buf->next;
				while (length < jmp)
				{
					length += buf->length - buf->bonus_offset;
					buf = buf->next;
				}
				command->jmp = buf;
			}
		}
		if (command->command[0] <= OPCODE_UP_CONDITIONAL_JMP && command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP)
		{
			//���������� ��������� � ��������� ������ 
			command->jmp = command->next->next;
			command = command->next;
			command->jmp = command->next->next;
			command = command->next;
			LONG32 jmp = (CHAR)command->command[1];
			if (jmp < 0)
			{
				LONG32 length = 0;
				buf = command;
				while (length > jmp)
				{
					jmp -= buf->bonus_offset;	//��������� �������������� �������� �������
					length -= buf->length;		//������� �� ����� ����� �������
					buf = buf->prev;
				}
				buf = buf->next;
				command->jmp = buf;
			}
			else
			{
				LONG32 length = 0;
				buf = command;
				buf = buf->next;
				while (length < jmp)
				{
					length += buf->length - buf->bonus_offset;
					buf = buf->next;
				}
				command->jmp = buf;
			}
		}
		command=command->next;
	}
	return ;
}


/****************************************************************************
�������������� �������
****************************************************************************/
Pieces* InitializationAllPieces(Commands* com, DWORD32 Number_Pieces)
{
	Pieces *pieces = new Pieces[Number_Pieces];
	DWORD32 Count_Commands = 0;
	DWORD32 Number_Instruction;
	DWORD32 size_Pieces;
	Commands *command = com;
	for (DWORD32 j = 0; j < Number_Pieces; j++){
		size_Pieces = 0;
		Number_Instruction = 0;
		pieces[j].first = command;
		while (command->command[0] != OPCODE_JMP && command->command[0] != OPCODE_RET && command->command[0] != OPCODE_CALL &&
			(command->command[0] <= OPCODE_DOWN_CONDITIONAL_JMP || command->command[0] >= OPCODE_UP_CONDITIONAL_JMP))
		{
			command->offset = size_Pieces;
			size_Pieces += command->length;
			command->P = &pieces[j];
			command = command->next;
			Number_Instruction++;
		}
		if (command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && command->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
		{
			//�������� ����� ������������ ��� ������� �������
			command->offset = size_Pieces;
			size_Pieces += command->length;
			command->P = &pieces[j];
			command = command->next;
			Number_Instruction++;
			//�����, ������� ��������� �� ��������� �����
			command->offset = size_Pieces;
			command->P = &pieces[j];
			pieces[j].jmp = command;
			size_Pieces += command->length;
			command = command->next;
			Number_Instruction++;
			//�����, ������� ��������� ��������
			command->offset = size_Pieces;
			command->P = &pieces[j];
			size_Pieces += command->length;
			pieces[j].size = size_Pieces;
			command = command->next;
			Number_Instruction++;
		}
		else
		{
			command->offset = size_Pieces;
			command->P = &pieces[j];
			pieces[j].jmp = command;
			size_Pieces += command->length;
			pieces[j].size = size_Pieces;
			command = command->next;
			Number_Instruction++;
		}
		pieces[j].Number_Instruction = Number_Instruction;
	}
	return pieces;
}