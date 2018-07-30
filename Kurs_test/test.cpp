#include "Initialization.h"
#include "Obfuskation.h"
#include "Write.h"

/****************************************************************************
Основная функция
****************************************************************************/
int main(void)
{
	setlocale(LC_ALL, "Russian");						//Устанавливаем локали
	srand((unsigned int)time(NULL));
	CHAR *NameFile = new CHAR[MAX_PATH];				//выделяем массив под имя файла, который надо обфусцировать
	CHAR* NameFile_out = new CHAR[MAX_PATH];
	std::cout << "Введите имя файла, который хотите обфусцировать: ";
	//std::cin.getline(NameFile, MAX_PATH);				//Получаем имя файла
	std::cout << "Введите имя файла, в который запишется обфусцированный код: ";
	//std::cin.getline(NameFile_out, MAX_PATH);
	NameFile = "sc1";									//задаём имя файла статически для отладки
	NameFile_out = "sc1_new";
	BYTE* Source_code=InitializationSourseCode(NameFile);
	/****************************************************************************
	Для отладки
	****************************************************************************/
	DWORD tmp;
	/*VirtualProtect(Source_code, _msize(Source_code) / sizeof(Source_code[0]), PAGE_EXECUTE_READWRITE, &tmp);
	((void(*)(void)) Source_code)();*/
	/****************************************************************************/
	Commands* command = InitializationAllCommands(Source_code);		//Заполняем массив структур команд
	if (command == 0){ return -1; }									//Если не заполнился
	CorrectionJMP(command);
	InsertTrashInPieces(command);
	DWORD32 Number_Pieces = GetNumberPieces(command);
	Pieces* buf = InitializationAllPieces(command, Number_Pieces);	//Заполняем структуры линейных кусочков
	MixPieces(buf);												
	CountTheSize(buf);												//Пересчитываем размер кусочков, после вставки мусорных команд
	BYTE* code=RestorationCode(buf);								//Получаем обфусцированный массив в виде байтового массива
	/****************************************************************************
	Для отладки
	****************************************************************************/
	std::cout << std::endl;
	int N = 100;
	int *mas = new int[N];
	for (int i = 0; i < N; i++)
	{
		mas[i] = rand()%100;
	}
	for (int i = 0; i < N; i++)
	{
		printf("%d\n", mas[i]);
	}
	//DWORD tmp;
	VirtualProtect(code, _msize(code) / sizeof(code[0]), PAGE_EXECUTE_READWRITE, &tmp);
	((void(*)(int, int*, void*, void*)) code)(N - 1, mas, printf, "hello\n");//, &N, mas); //первая N

	/****************************************************************************/
	printf("%d\n", 0);
	for (int i = 1; i < N; i++)
	{
		printf("%d\n", mas[i]);
	}
	WriteCode(code, NameFile_out);
	std::cout << "Программа завершила свою работу\n\tСпасибо за использование моей программы!!!\n\t\tУдачи!\n";
	system("pause");
	return 0;
}
