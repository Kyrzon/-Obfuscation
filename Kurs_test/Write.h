#ifndef	_WRITE_
#define _WRITE_
#include "All.h"
/****************************************************************************
�������� ��������������� ��� � ���� ��������� �������
****************************************************************************/
BYTE* RestorationCode(Pieces *pieces);

/****************************************************************************
�������� ������� � ��� � ���������� ��� � ������
****************************************************************************/
BYTE* WriteToCode(DWORD32 size, Pieces* pieces);

/****************************************************************************
�������� ������ �� ���������������� ����
****************************************************************************/
DWORD32 GetSizeCode(Pieces* pieces);
/****************************************************************************
������ � ���� ���������������� ����
****************************************************************************/
VOID WriteCode(BYTE* Code, CHAR *NameFile);
#endif