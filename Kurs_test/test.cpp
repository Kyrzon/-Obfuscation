#include "Initialization.h"
#include "Obfuskation.h"
#include "Write.h"

/****************************************************************************
�������� �������
****************************************************************************/
int main(void)
{
	setlocale(LC_ALL, "Russian");						//������������� ������
	srand((unsigned int)time(NULL));
	CHAR *NameFile = new CHAR[MAX_PATH];				//�������� ������ ��� ��� �����, ������� ���� �������������
	CHAR* NameFile_out = new CHAR[MAX_PATH];
	std::cout << "������� ��� �����, ������� ������ �������������: ";
	//std::cin.getline(NameFile, MAX_PATH);				//�������� ��� �����
	std::cout << "������� ��� �����, � ������� ��������� ��������������� ���: ";
	//std::cin.getline(NameFile_out, MAX_PATH);
	NameFile = "sc1";									//����� ��� ����� ���������� ��� �������
	NameFile_out = "sc1_new";
	BYTE* Source_code=InitializationSourseCode(NameFile);
	/****************************************************************************
	��� �������
	****************************************************************************/
	DWORD tmp;
	/*VirtualProtect(Source_code, _msize(Source_code) / sizeof(Source_code[0]), PAGE_EXECUTE_READWRITE, &tmp);
	((void(*)(void)) Source_code)();*/
	/****************************************************************************/
	Commands* command = InitializationAllCommands(Source_code);		//��������� ������ �������� ������
	if (command == 0){ return -1; }									//���� �� ����������
	CorrectionJMP(command);
	InsertTrashInPieces(command);
	DWORD32 Number_Pieces = GetNumberPieces(command);
	Pieces* buf = InitializationAllPieces(command, Number_Pieces);	//��������� ��������� �������� ��������
	MixPieces(buf);												
	CountTheSize(buf);												//������������� ������ ��������, ����� ������� �������� ������
	BYTE* code=RestorationCode(buf);								//�������� ��������������� ������ � ���� ��������� �������
	/****************************************************************************
	��� �������
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
	((void(*)(int, int*, void*, void*)) code)(N - 1, mas, printf, "hello\n");//, &N, mas); //������ N

	/****************************************************************************/
	printf("%d\n", 0);
	for (int i = 1; i < N; i++)
	{
		printf("%d\n", mas[i]);
	}
	WriteCode(code, NameFile_out);
	std::cout << "��������� ��������� ���� ������\n\t������� �� ������������� ���� ���������!!!\n\t\t�����!\n";
	system("pause");
	return 0;
}
