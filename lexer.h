#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "hash.h"
#include <stack>


using namespace std;

class lexer;
typedef void(lexer::*func)();
enum symbols{ s_letter, s_digit, s_operation, s_ratio, s_left_bracket, s_right_bracket, s_point, s_space, s_endline, s_endfile, s_error };
enum lexemes{ l_label, l_operand, l_ratio, l_operation, l_next, l_let, l_for, l_goto, l_gosub, l_left_bracket, l_right_bracket, l_if, l_return, l_end, l_to, l_step, l_rem, l_error, l_endfile };

struct lexeme
{
	int lexeme_type;
	double lexeme_value;
	lexeme()
	{
		lexeme_type = -1;
		lexeme_value = -1;
	}
	lexeme(int a, int b)
	{
		lexeme_type = a;
		lexeme_value = b;
	}
};

struct token
{
	int token_type;
	int token_value;
};

struct ident_table_row
{
	int number;
	char letter;
	int alt;
	func fn;
	ident_table_row(int n, char c, func f, int a)
	{
		number = n;
		letter = c;
		alt = a;
		fn = f;
	}
};

class error_in_string
{
	int number;
	int class_lex;
public:
	error_in_string(int n, int m)
	{
		number = n;
		class_lex = m;
	}
	void write(list<pair<int,int>>&table)
	{
		table.push_back(make_pair(number, class_lex));
		//ofstream file_out("errors.txt");
		//ile_out << "Error in string " << number+1 << endl;
		//file_out.close();
	}
};

class lexer
{
protected:
	func state;
	vector<ident_table_row> ident_table;
	vector<char> start_vector;

	token t;
	void Transliterator(char c)
	{
		token res;
		if ('0' <= c && c <= '9')
		{
			res.token_type = s_digit;
			res.token_value = c - '0';
		}
		else if ('A' <= c && c <= 'Z')
		{
			res.token_type = s_letter;
			res.token_value = c - 'A';
		}
		else if ('a' <= c && c <= 'z')
		{
			res.token_type = s_letter;
			res.token_value = c - 'a';
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
		{
			res.token_type = s_operation;
			if (c == '+') res.token_value = 1;
			else if (c == '-') res.token_value = 2;
			else if (c == '*') res.token_value = 3;
			else if (c == '/') res.token_value = 4;
			else if (c == '^') res.token_value = 5;
		}
		else if (c == '<' || c == '>' || c == '=')
		{
			res.token_type = s_ratio;
			if (c == '=') res.token_value = 1;
			else if (c == '<') res.token_value = 2;
			else if (c == '>') res.token_value = 3;
		}
		else if (c == ' ')
		{
			res.token_type = s_space;
			res.token_value = 0;
		}
		else if (c == '\n' || c == '\0')
		{
			res.token_type = s_endline;
			res.token_value = 0;
		}
		else if (c == '(')
		{
			res.token_type = s_left_bracket;
			res.token_value = 0;
		}
		else if (c == ')')
		{
			res.token_type = s_right_bracket;
			res.token_value = 0;
		}
		else if (c == '.')
		{
			res.token_type = s_point;
			res.token_value = 0;
		}
		else if (c == ' ')
		{
			res.token_type = s_space;
			res.token_value = 0;
		}
		else
		{
			res.token_type = s_error;
			res.token_value = 0;
		}
		RZN = res.token_value;
		RK = res.token_type;
	}

	vector <lexeme> table_lexemes;
	hash_table table_strings;
	double table_operands[350];
	list <pair<int,int>> table_errors;
	//-----------------------------------Регистры-------------

	int RKL;//регистр класса лексемы
	double RCH;//регистр числа
	int RSTR;//регистр строки
	int RZN;//регистр значения символа
	int RK;//регистр значения класса символа
	int ROB;//регистр обнаружения
	int RP;//регистр порядка
	int RZ;//регистр знака порядка
	int RS;//регистр счетчика
	string reg_var;//регистр переменной
	int ROT;//регистр отношения
	int NTO;//указатель на первый свободный элемент table_operands. Начальное значение = 287
	//------------------------------Функции таблицы обнаружения---------------------

	void B1d()
	{
		ROB++;
		state = &lexer::B1;
	}
	void F1a()
	{
		RKL = l_let;
		state = &lexer::F1;
	}
	void F1b()
	{
		RKL = l_for;
		state = &lexer::F1;
	}
	void A2q()
	{
		RKL = l_end;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::A1;
	}
	void E1a()
	{
		RKL = l_goto;
		state = &lexer::E1;
	}
	void E1b()
	{
		RKL = l_gosub;
		state = &lexer::E1;
	}
	void A2r()
	{
		RKL = l_if;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::A2;
	}
	void C1a()
	{
		RKL = l_next;
		state = &lexer::C1;
	}
	void A2s()
	{
		RKL = l_return;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::A1;
	}
	void G1a()
	{
		RKL = l_rem;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::G1;
	}
	void A2t()
	{
		RKL = l_step;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::A2;
	}
	void A2u()
	{
		RKL = l_to;
		lexeme l;
		l.lexeme_type = RKL;
		table_lexemes.push_back(l);
		state = &lexer::A2;
	}

	//-------------------------------------Состояния--------------------------------

	void error()
	{
		if (RK == s_endline)
		{
			lexeme l;
			l.lexeme_type = l_error;
			table_lexemes.push_back(l);
			state = &lexer::A1;
			throw error_in_string(RSTR, RKL);
		}
		else state = &lexer::error;
	}

	void A1()//complete
	{
		switch (RK)
		{
		case s_digit:
			RKL = l_label;
			RSTR = RZN;
			state = &lexer::E2;
			break;
		case s_space:
			state = &lexer::A1;
			break;
		case s_endline:
			state = &lexer::A1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void A2()
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			RKL = l_operand;
			RCH = RZN;
			state = &lexer::D1;
			break;
		case s_letter:
			//C2a
			RKL = l_operand;
			reg_var = RZN + 'a';
			state = &lexer::C2;
			break;
		case s_ratio:
			//H1a
			ROT = RZN;
			RKL = l_ratio;
			state = &lexer::H1;
			break;
		case s_left_bracket:
			//A2h
			RKL = l_left_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_right_bracket:
			//A3b
			RKL = l_right_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A3;
			break;
		case s_point:
			RKL = l_operand;
			state = &lexer::D6;
			break;
		case s_operation:
			//A2a
			//l.lexeme_type = RKL;
			break;
		case s_endline:
			state = &lexer::A1;
			break;
		case s_space:
			state = &lexer::A2;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void A3()
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			RKL = l_operand;
			RCH = RZN;
			state = &lexer::D1;
			break;
		case s_letter:
			//B1a
			ROB = start_vector[RZN];//maybe fix
			if (ROB == 0) state = &lexer::error;
			state = &lexer::B1;
			break;
		case s_ratio:
			//H1a
			ROT = RZN;
			RKL = l_ratio;
			state = &lexer::H1;
			break;
		case s_operation:
			//A2a
			RKL = l_operation;
			l.lexeme_type = RKL;
			l.lexeme_value = RZN;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_left_bracket:
			//A2h
			RKL = l_left_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_right_bracket:
			//A3b
			RKL = l_right_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A3;
			break;
		case s_space:
			state = &lexer::A3;
			break;
		case s_endline:
			state = &lexer::A1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void B1()//complete
	{
		switch (RK)
		{
		case s_letter:
			//M1();
			if (RZN + 'A' == ident_table[ROB].letter) (this->*ident_table[ROB].fn)();
			else
			{
				ROB = ident_table[ROB].alt;
				if (ROB == 0) state = &lexer::error;
				else (this->*ident_table[ROB].fn)();
			}
			break;
		case s_space:
			state = &lexer::B1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void C1()//complete
	{
		switch (RK)
		{
		case s_letter:
			reg_var = RZN + 'a';
			state = &lexer::C2;
			break;
		case s_space:
			state = &lexer::C1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void C2()//complete
	{
		lexeme l;
		switch (RK)
		{
		case s_ratio:
			//H1b;
			l.lexeme_type = RKL;//operand
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);
			table_operands[tolower(reg_var[0]) - 'a' + 1] = 1;
			ROT = RZN;
			RKL = l_ratio;
			state = &lexer::H1;
			break;
		case s_digit:
			//A3a
			reg_var += RZN + '0';
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1 + 26 * (reg_var[1] - '0' + 1);
			table_lexemes.push_back(l);
			reg_var = "";
			state = &lexer::A3;//может быть А3
			break;
		case s_letter:
			//B1b
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);

			//прочитали переменную, читаем ключевое слово
			ROB = start_vector[RZN];//maybe fix
			if (ROB == 0) state = &lexer::error;
			else state = &lexer::B1;
			break;
		case s_operation:
			//A2g
			table_operands[tolower(reg_var[0]) - 'a' + 1] = 1;
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);
			reg_var = "";

			RKL = l_operation;
			l.lexeme_type = RKL;
			l.lexeme_value = RZN;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_left_bracket:
			//A2k
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);
			reg_var = "";

			RKL = l_left_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_right_bracket:
			//A3c
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);
			reg_var = "";

			RKL = l_right_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A3;
			break;
		case s_space:
			state = &lexer::C2;
			break;
		case s_endline:
			l.lexeme_type = RKL;
			l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
			table_lexemes.push_back(l);
			state = &lexer::A1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D1()
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			RCH = RCH * 10 + RZN;
			state = &lexer::D1;
			break;
		case s_letter:
			//M2
			if (RZN == 5)
			{
				//DA1d
				//B1b
			}
			else
			{
				//D3a
				l.lexeme_type = RKL;
				l.lexeme_value = NTO;
				table_lexemes.push_back(l);
				table_operands[NTO] = RCH;
				NTO++;

				ROB = start_vector[RZN];
				if (ROB == 0) state = &lexer::error;
				else state = &lexer::B1;
			}
			break;
		case s_operation:
			//A2c
			l.lexeme_type = RKL;
			//l.lexeme_value = RCH;
			table_operands[NTO] = RCH;
			l.lexeme_value = NTO;
			NTO++;
			table_lexemes.push_back(l);
			RKL = l_operation;
			l.lexeme_type = RKL;
			l.lexeme_value = RZN;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_endline:
			//DA1A();
			//create lexeme number
			//
			l.lexeme_type = RKL;
			l.lexeme_value = NTO;
			table_lexemes.push_back(l);
			table_operands[NTO] = RCH;
			NTO++;
			state = &lexer::A1;
			break;
		case s_left_bracket:
			//DA1d
			l.lexeme_type = RKL;
			l.lexeme_value = NTO;
			table_lexemes.push_back(l);
			table_operands[NTO] = RCH;
			NTO++;
			RKL = l_left_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A2;
			break;
		case s_right_bracket:
			//DA1d
			l.lexeme_type = RKL;
			l.lexeme_value = NTO;
			table_lexemes.push_back(l);
			table_operands[NTO] = RCH;
			NTO++;
			RKL = l_right_bracket;
			l.lexeme_type = RKL;
			table_lexemes.push_back(l);
			state = &lexer::A3;
			break;
		case s_point:
			//D2c
			RS = 0;
			state = &lexer::D2;
			break;
		case s_space:
			state = &lexer::D1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D2()
	{
		lexeme l;
		switch (RK)
		{
		case s_letter:
			//M3;
			if (RZN != 4)//E - четверная буква в алфавите, если считать с нуля
			{
				//DA2d
				//B1b
			}
			else state = &lexer::D3;
			break;
		case s_digit:
			//D2a
			RS++;
			RCH *= 10;
			RCH += RZN;
			state = &lexer::D2;
			break;
		case s_operation:
			//A2d: DA2d, A2g
			RP = -RS;
			//calculate constant
			RP -= RS;
			RCH = RCH * pow(10, RP);
			break;
		case s_endline:
			//A1c
			RP -= RS;
			RCH = RCH * pow(10, RP);
			l.lexeme_type = RKL;
			l.lexeme_value = NTO;
			table_lexemes.push_back(l);
			table_operands[NTO] = RCH;
			NTO++;
			state = &lexer::A1;
			break;
		case s_space:
			state = &lexer::D2;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D3()
	{
		switch (RK)
		{
		case s_digit:
			//D5a
			RZ = 1;
			RP = RZN;
			state = &lexer::D5;
			break;
		case s_operation:
			//D4a
			if (RZN == 1) RZ = 1;
			if (RZN == 2) RZ = -1;
			if (RZN > 2) state = &lexer::error;
			else state = &lexer::D4;
			break;
		case s_space:
			state = &lexer::D3;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D4()
	{
		switch (RK)
		{
		case s_digit:
			//D5b
			RP = RZN;
			state = &lexer::D5;
			break;
		case s_space:
			state = &lexer::D4;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D5()
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			//D5c
			RP *= 10;
			RP += RZN;
			state = &lexer::D5;
			break;
		case s_endline:
			//A1d : DA3d, A1a
			if (RZ < 0) RP *= -1;
			RP -= RS;
			RCH = RCH * pow(10, RP);
			//calculate constant
			//create lexeme
			l.lexeme_type = RKL;
			l.lexeme_value = NTO;
			table_lexemes.push_back(l);
			table_operands[NTO] = RCH;
			NTO++;
			state = &lexer::A1;
			break;
		case s_space:
			state = &lexer::D5;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void D6()//complete
	{
		switch (RK)
		{
		case s_digit:
			//D2b
			RS = 1;
			RCH = RZN;
			state = &lexer::D2;
			break;
		case s_space:
			state = &lexer::D6;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void E1()//complete
	{
		switch (RK)
		{
		case s_digit:
			//E2b
			RSTR = RZN;
			state = &lexer::E2;
			break;
		case s_space:
			state = &lexer::E1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void E2()
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			RSTR = RSTR * 10 + RZN;
			state = &lexer::E2;
			break;
		case s_letter:
			//DA1E() : Используется  метод расстановки.  Индекс  расстановки
			//вычисляется путем деления номера строки на  константу Р.
			//	На начальных стадиях  построения  мы полагаем  Р = 100. По
			//	индексу из таблицы указателей списков выбирается
			//	указатель на таблицу номеров строк, имеющих этот индекс,
			//	организованную в виде связного списка элементов.В этих
			//	элементах  нас интересуют поля : НОМЕР СТРОКИ и СЛЕД
			//	ЭЛЕМЕНТ, содержащий указатель на следующий элемент
			//	списка.Если в списке, соответствующем индексу номера
			//	строки, нет элемента, содержащего номер, который мы ищем
			//	(возможно, из - за того, что список пуст), создается
			//	новый элемент, он заполняется и помещается в начало
			//	этого списка.)
			//	Вычислить остаток от деления РЕГИСТРА СТРОКИ на Р
			//	Загрузить в РЕГИСТР УКАЗАТЕЛЯ содержимое элемента
			//	таблицы  указателей списков, по индексу, равному только
			//	что вычисленному остатку
			//B1b(); - create lexeme
			l.lexeme_type = RKL;
			table_strings.insert(RSTR);
			l.lexeme_value = table_strings.find(RSTR);
			table_lexemes.push_back(l);
			//B1a();
			ROB = start_vector[RZN];//maybe fix
			if (ROB == 0) state = &lexer::error;
			state = &lexer::B1;
			break;
		case s_endline:
			//DA1e
			//A1a
			l.lexeme_type = RKL;
			//l.lexeme_value = RSTR;
			//table_lexemes.push_back(l);
			table_strings.insert(RSTR);
			l.lexeme_value = table_strings.find(RSTR);
			table_lexemes.push_back(l);
			state = &lexer::A1;
			break;
		case s_space:
			state = &lexer::E2;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void F1()//complete
	{
		switch (RK)
		{
		case s_letter:
			//F2a();//wtf
			reg_var = RZN + 'a';
			state = &lexer::F2;
			break;
		case s_space:
			state = &lexer::F1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void F2()//complete
	{
		lexeme l;
		switch (RK)
		{
		case s_digit:
			//F3a();
			//RZN++;
			//RZN *= 26;
			//wtf
			reg_var += RZN + '0';
			state = &lexer::F2;
			break;
		case s_ratio:
			//A2o();
			if (RZN != 1) state = &lexer::error;
			else
			{
				lexeme l;
				l.lexeme_type = RKL;

				if (reg_var.length() == 1)
				{
					table_operands[tolower(reg_var[0]) - 'a'+ 1] = 1;
					l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
				}
				else if (reg_var.length() == 2)
				{
					table_operands[tolower(reg_var[0]) - 'a' + 1 + 26 * (reg_var[1] - '0' + 1)] = 1;
					l.lexeme_value = tolower(reg_var[0]) - 'a' + 1 + 26 * (reg_var[1] - '0' + 1);
				}
				table_lexemes.push_back(l);
				state = &lexer::A2;
			}
			break;
		case s_space:
			state = &lexer::F2;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void F3()//complete
	{
		switch (RK)
		{
		case s_ratio:
			//A2o
			if (RZN != 1) state = &lexer::error;
			else
			{
				lexeme l;
				l.lexeme_type = RKL;

				if (reg_var.length() == 1)
				{
					table_operands[tolower(reg_var[0]) - 'a' + 1] = 1;
					l.lexeme_value = tolower(reg_var[0]) - 'a' + 1;
				}
				else if (reg_var.length() == 2)
				{
					table_operands[tolower(reg_var[0]) - 'a'+ 1 + 26 * (reg_var[1] - '0' + 1)] = 1;
					l.lexeme_value = tolower(reg_var[0]) - 'a'+ 1 + 26 * (reg_var[1] - '0' + 1);
				}
				table_lexemes.push_back(l);
				state = &lexer::A2;
			}
			break;
		case s_space:
			state = &lexer::F3;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

	void G1()//complete
	{
		if (RK == s_endline) state = &lexer::A1;
		else state = &lexer::G1;
	}

	void H1()
	{
		lexeme l;
		switch (RK)
		{
		case s_letter:
			//C2b
			l.lexeme_type = RKL;//ratio
			l.lexeme_value = ROT;
			table_lexemes.push_back(l);
			RKL = l_operand;
			reg_var = RZN + 'a';//fix
			state = &lexer::C2;
			break;
		case s_digit:
			//D1c
			l.lexeme_type = RKL;
			l.lexeme_value = ROT;
			table_lexemes.push_back(l);
			RKL = l_operand;
			RCH = RZN;
			state = &lexer::D1;
			break;
		case s_ratio:
			//A2p
			if (ROT == 2)//<
			{
				if (RZN == 1)ROT = 4;//<=
				else if (RZN == 3) ROT = 6;//<>
				else state = &lexer::error;
			}
			else if (ROT == 3)//>
			{
				if (RZN == 1) ROT = 5; //>=
				else state = &lexer::error;
			}
			else state = &lexer::error;
			break;
		case s_space:
			state = &lexer::H1;
			break;
		default:
			state = &lexer::error;
			break;
		}
	}

public:
	lexer()
	{
		NTO = 286;
		start_vector = { 0, 0, 0, 0, 1, 3, 5, 0, 11, 0, 0, 12, 0, 14, 0, 0, 0, 17, 23, 26, 0, 0, 0, 0, 0, 0 };//без сдвига
		ident_table.push_back(ident_table_row(0, 'z', 0, 0));
		ident_table.push_back(ident_table_row(1, 'N', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(2, 'D', &lexer::A2q, 0));//END
		ident_table.push_back(ident_table_row(3, 'O', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(4, 'R', &lexer::F1b, 0));//FOR
		ident_table.push_back(ident_table_row(5, 'O', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(6, 'T', &lexer::B1d, 8));
		ident_table.push_back(ident_table_row(7, 'O', &lexer::E1a, 0));//GOTO
		ident_table.push_back(ident_table_row(8, 'S', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(9, 'U', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(10, 'B', &lexer::E1b, 0));//GOSUB
		ident_table.push_back(ident_table_row(11, 'F', &lexer::A2r, 0));//IF
		ident_table.push_back(ident_table_row(12, 'E', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(13, 'T', &lexer::F1a, 0));//LET
		ident_table.push_back(ident_table_row(14, 'E', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(15, 'X', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(16, 'T', &lexer::C1a, 0));//NEXT
		ident_table.push_back(ident_table_row(17, 'E', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(18, 'T', &lexer::B1d, 22));
		ident_table.push_back(ident_table_row(19, 'U', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(20, 'R', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(21, 'N', &lexer::A2s, 0));//RETURN
		ident_table.push_back(ident_table_row(22, 'M', &lexer::G1a, 0));//REM
		ident_table.push_back(ident_table_row(23, 'T', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(24, 'E', &lexer::B1d, 0));
		ident_table.push_back(ident_table_row(25, 'P', &lexer::A2t, 0));//STEP
		ident_table.push_back(ident_table_row(26, 'O', &lexer::A2u, 0));//TO		
	}
	void run(char* filename)
	{
		fstream file(filename);
		char symbol;
		string str;
		state = &lexer::A1;
		while (getline(file, str))
		{
			//cout << str << endl;
			try
			{
				for (int i = 0; i <= str.length(); i++)
				{
					Transliterator(str[i]);
					(this->*state)();
				}
			}
			catch (error_in_string error)
			{
				error.write(table_errors);
			}
		}
	}

	void showLexemes()
	{
		cout << "Lexemes: " << endl << endl;
		string lexemesString[]{"label", "operand", "ratio", "operation", "next", "let", "for", "goto", "gosub", "left_bracket", "right_bracket", "if", "return", "end", "to", "step", "rem", "error", "endfile"};
		for (vector<lexeme>::iterator it = table_lexemes.begin(); it != table_lexemes.end(); it++)
		{
			cout << lexemesString[it->lexeme_type] << " ";
			if (it->lexeme_value != -1) cout << it->lexeme_value;
			cout << endl;
		}
	}

	void writeToFile()
	{
		ofstream out_file("out_file.doc");
		if (!out_file)
		{
			cout << "Can't open file" << endl;
			return;
		}

		out_file << "Lexemes: " << endl;
		string lexemesString[]{"label", "operand", "ratio", "operation", "next", "let", "for", "goto", "gosub", "left_bracket", "right_bracket", "if", "return", "end", "to", "step", "rem", "error", "endfile"};
		
		for (vector<lexeme>::iterator it = table_lexemes.begin(); it != table_lexemes.end(); it++)
		{
			out_file.width(13);
			out_file.fill(' ');
			out_file << left << lexemesString[it->lexeme_type] << " " ;
			if (it->lexeme_value != -1) out_file << it->lexeme_value;
			out_file << endl;
		}

		out_file << endl << "Operands: " << endl;
		for (int i = 0; i < 350; i++)
		{
			if (table_operands[i] != 0)
			{
				if (i == 286) out_file << "Constants: " << endl;
				out_file << i << ". " << table_operands[i] << endl;
			}
		}

		out_file << endl << "Strings:" << endl;
		table_strings.printToFile(out_file);

		out_file.close();

		ofstream errors("errors.txt");
		for (list<pair<int,int>>::iterator it = table_errors.begin(); it != table_errors.end(); it++)
			errors << "Error in string " << it->first << " or near, after lexeme " << it->second << endl;
		errors.close();
	}
};
