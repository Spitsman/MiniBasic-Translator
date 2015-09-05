// mini_basic_translator.cpp : Defines the entry point for the console application.
//�������� ����� � �����, �������, ��������� ������
// upd : �������� ��� ��������� �������� (�� ������ <=), ������� ��������� ������� ����� � ������� ���������, � �� ��� �������

#include "stdafx.h"
#include <iostream>
#include "lexer.h"
#include "syntax_analyser.h"
#include "generator.h"


int _tmain(int argc, _TCHAR* argv[])
{
	/*lexer automat;
	automat.run("input.txt");
	automat.writeToFile();*/
	setlocale(LC_ALL, "Russian");
	/*synt automat;
	automat.start("input.txt");
	automat.writeToFile();
	automat.showAtoms();*/
	generator g;
	g.go("input.txt");
	g.writeToFile();
	g.showAtoms();
	return 0;
}

