#include "All.h"


/****************************************************************************
Получаем размер не обфусцированного кода
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
Собираем кусочки в код и записываем его в массив
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
		{		//получили указатель кусочка, в ктором команда, на который ссылается джамп

			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ПОЧЕМУ?????????????????????? Как это работает?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//сохраняем первую команду в том кусочке, на который указывает джамп
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//считаем смещение до этого кусочка, не учитывая смещение джампа в своём кусочке
					buf++;
				}
				//offset_jmp -= LENGTH_JMP;				//Учли смещение джампа в своём кусочке
				while (command->jmp != command_buf)			//Учитываем смещение той команды, на которую указывает джамп
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
			//записываем Условный джамп
			for (DWORD32 j = 0; j < command->length; j++)
			{
				code[j + count] = command->command[j];
			}
			count += command->length;

			//записываем джамп, который срабатывает, если не работает условный
			command = command->next;
			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ПОЧЕМУ?????????????????????? Как это работает?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//сохраняем первую команду в том кусочке, на который указывает джамп
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//считаем смещение до этого кусочка, не учитывая смещение джампа в своём кусочке
					buf++;
				}
				offset_jmp += LENGTH_JMP;				//Учли смещение джампа из-за следующего джампа
				while (command->jmp != command_buf)			//Учитываем смещение той команды, на которую указывает джамп
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
				offset_jmp += LENGTH_JMP;	//Учли смещение джампа из-за следующего джампа
				while (command->jmp != command_buf)
				{
					offset_jmp += command_buf->length;
					command_buf = command_buf->next;
				}
			}
			code[0 + count] = command->command[0];
			*((LONG32*)(code + count + 1)) = offset_jmp;
			count += command->length;
			//Записываем джамп, который удлинняет условный джамп
			command = command->next;
			p = command->jmp->P;

			buf = (p - &pieces[i]); /// sizeof(pieces[0]);    ПОЧЕМУ?????????????????????? Как это работает?????
			offset_jmp = 0;
			if (buf <= 0)
			{
				command_buf = pieces[i + buf].first;		//сохраняем первую команду в том кусочке, на который указывает джамп
				while (buf <= 0)
				{
					offset_jmp -= pieces[i + buf].size;		//считаем смещение до этого кусочка, не учитывая смещение джампа в своём кусочке
					buf++;
				}
				//offset_jmp -= LENGTH_JMP;				//Учли смещение джампа в своём кусочке
				while (command->jmp != command_buf)			//Учитываем смещение той команды, на которую указывает джамп
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
Получаем обфусцированный код в виде байтового массива
****************************************************************************/
BYTE* RestorationCode(Pieces *pieces)
{
	DWORD32 size = GetSizeCode(pieces);
	BYTE* code = WriteToCode(size, pieces);
	return code;
}
/****************************************************************************
Запись в файл обфусцированного кода
****************************************************************************/
VOID WriteCode(BYTE* Code, CHAR *NameFile)
{
	DWORD32 sizeCode;									//Размер кода
	HANDLE hFile = CreateFileA(NameFile,				//Имя файла
		GENERIC_WRITE,			//Режим доступа
		FILE_SHARE_WRITE,		//Совместный доступ
		NULL,					//SD(дескриптор защиты)
		CREATE_ALWAYS,			//Как действовать
		FILE_ATTRIBUTE_NORMAL,	//Атрибуты файла
		NULL);					//дескриптор шаблога файла
	if (hFile == INVALID_HANDLE_VALUE){ std::cout << "Не могу открыть файл/n"; }	//Проверка: Открылся ли файл?
	sizeCode = _msize(Code)/sizeof(Code[0]);					//Получаем размер кода
	if (sizeCode == 0){ std::cout << "Массив пуст/n"; }	//Проверка: не пуст ли массив?
	DWORD Musor;										//Переменная для мусора. Просто не знаю как без неё
	if (!WriteFile(hFile, Code, sizeCode, &Musor, NULL))	//Читаем из файла
	{
		std::cout << "Проблема...\n";
		GetLastError();
	}
	CloseHandle(hFile);
	return;
}