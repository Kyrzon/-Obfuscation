#include "All.h"


/****************************************************************************
�������� ������ �� ���������������� ����
****************************************************************************/
DWORD32 GetSizeCode(Pieces* pieces)
{
	DWORD32 length = 0;
	DWORD32 Number_Pieces = _msize(pieces) / sizeof(pieces[0]);
	for (DWORD32 i = 0; i < Number_Pieces; i++){
		length += pieces[i].size;
	}
	return length;
}

/****************************************************************************
�������� ������� � ��� � ���������� ��� � ������
****************************************************************************/
BYTE* WriteToCode(DWORD32 size, Pieces* pieces)
{
	Commands* command = 0;
	Commands* command_buf = 0;
	//
	Pieces* p;
	//
	DWORD32 count = 0;
	LONG32 buf;
	BYTE* code = new BYTE[size];
	LONG32 offset_jmp;
	DWORD32 Number_Pieces = _msize(pieces) / sizeof(pieces[0]);
	for (DWORD32 i = 0; i < Number_Pieces; i++)
	{
		command = pieces[i].first;
		while (command->command[0] != OPCODE_JMP && command->command[0] != OPCODE_RET && command->command[0] != OPCODE_CALL &&
			(command->command[0] <= OPCODE_DOWN_CONDITIONAL_JMP || command->command[0] >= OPCODE_UP_CONDITIONAL_JMP))
		{
			for (DWORD32 j = 0; j < command->length; j++)
			{
				code[j + count] = command->command[j];
			}
			count += command->length;
			command = command->next;
		}
		if (command->command[0] == OPCODE_JMP || command->command[0] == OPCODE_CALL)
		{		//�������� ��������� �������, � ������ �������, �� ������� ��������� �����

			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ������?????????????????????? ��� ��� ��������?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//��������� ������ ������� � ��� �������, �� ������� ��������� �����
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//������� �������� �� ����� �������, �� �������� �������� ������ � ���� �������
					buf++;
				}
				//offset_jmp -= LENGTH_JMP;				//���� �������� ������ � ���� �������
				while (command->jmp != command_buf)			//��������� �������� ��� �������, �� ������� ��������� �����
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			else
			{
				command_buf = pieces[i + buf].first;
				buf--;
				while (buf != 0)
				{
					offset_jmp += pieces[i + buf].size;
					buf--;
				}
				//offset_jmp += command->offset;
				while (command->jmp != command_buf)
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			code[0 + count] = command->command[0];
			*((LONG32*)(code + count + 1)) = offset_jmp;
		}
		if (command->command[0] >= OPCODE_DOWN_CONDITIONAL_JMP && command->command[0] <= OPCODE_UP_CONDITIONAL_JMP)
		{
			//���������� �������� �����
			for (DWORD32 j = 0; j < command->length; j++)
			{
				code[j + count] = command->command[j];
			}
			count += command->length;

			//���������� �����, ������� �����������, ���� �� �������� ��������
			command = command->next;
			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ������?????????????????????? ��� ��� ��������?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//��������� ������ ������� � ��� �������, �� ������� ��������� �����
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//������� �������� �� ����� �������, �� �������� �������� ������ � ���� �������
					buf++;
				}
				offset_jmp += LENGTH_JMP;				//���� �������� ������ ��-�� ���������� ������
				while (command->jmp != command_buf)			//��������� �������� ��� �������, �� ������� ��������� �����
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			else
			{
				command_buf = pieces[i + buf].first;
				buf--;
				while (buf != 0)
				{
					offset_jmp += pieces[i + buf].size;
					buf--;
				}
				offset_jmp += LENGTH_JMP;	//���� �������� ������ ��-�� ���������� ������
				while (command->jmp != command_buf)
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			code[0 + count] = command->command[0];
			*((LONG32*)(code + count + 1)) = offset_jmp;
			count += command->length;
			//���������� �����, ������� ��������� �������� �����
			command = command->next;
			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ������?????????????????????? ��� ��� ��������?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//��������� ������ ������� � ��� �������, �� ������� ��������� �����
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//������� �������� �� ����� �������, �� �������� �������� ������ � ���� �������
					buf++;
				}
				//offset_jmp -= LENGTH_JMP;				//���� �������� ������ � ���� �������
				while (command->jmp != command_buf)			//��������� �������� ��� �������, �� ������� ��������� �����
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			else
			{
				command_buf = pieces[i + buf].first;
				buf--;
				while (buf != 0)
				{
					offset_jmp += pieces[i + buf].size;
					buf--;
				}
				//offset_jmp += command->offset;
				while (command->jmp != command_buf)
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			code[0 + count] = command->command[0];
			*((LONG32*)(code + count + 1)) = offset_jmp;
		}
		if (command->command[0] == OPCODE_RET)
		{
			for (DWORD32 j = 0; j < command->length; j++)
			{
				code[j + count] = command->command[j];
			}
		}
		count += command->length;
	}
	return code;
}

/****************************************************************************
�������� ��������������� ��� � ���� ��������� �������
****************************************************************************/
BYTE* RestorationCode(Pieces *pieces)
{
	DWORD32 size = GetSizeCode(pieces);
	BYTE* code = WriteToCode(size, pieces);
	return code;
}
/****************************************************************************
������ � ���� ���������������� ����
****************************************************************************/
VOID WriteCode(BYTE* Code, CHAR *NameFile)
{
	DWORD32 sizeCode;									//������ ����
	HANDLE hFile = CreateFileA(NameFile,				//��� �����
		GENERIC_WRITE,			//����� �������
		FILE_SHARE_WRITE,		//���������� ������
		NULL,					//SD(���������� ������)
		CREATE_ALWAYS,			//��� �����������
		FILE_ATTRIBUTE_NORMAL,	//�������� �����
		NULL);					//���������� ������� �����
	if (hFile == INVALID_HANDLE_VALUE){ std::cout << "�� ���� ������� ����/n"; }	//��������: �������� �� ����?
	sizeCode = _msize(Code)/sizeof(Code[0]);					//�������� ������ ����
	if (sizeCode == 0){ std::cout << "������ ����/n"; }	//��������: �� ���� �� ������?
	DWORD Musor;										//���������� ��� ������. ������ �� ���� ��� ��� ��
	if (!WriteFile(hFile, Code, sizeCode, &Musor, NULL))	//������ �� �����
	{
		std::cout << "��������...\n";
		GetLastError();
	}
	CloseHandle(hFile);
	return;
}