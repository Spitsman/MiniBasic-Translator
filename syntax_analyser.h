#include <iostream>
#include <iomanip>

enum syntLexemes{S_LABEL, S_OPERAND,S_RATIO,S_LOOPEND,S_LET,S_FOR,S_GOTO,S_GOSUB,S_LEFTBR,S_RIGHTBR,S_IF,S_RETURN,S_END,S_TO, S_STEP,S_REM,S_PLUS,S_MINUS,S_MULT,S_DIV,S_POW,S_ERR,S_ENDM};
enum nonterminals{ APROGRAMM = 1, APRBODY, ASTEP, AOTHERSTR, AEXPRESS, ATERM, AMULTIPLIER, APRIMARY, AE_NODE, AT_NODE, AF_NODE, ARATIO, ALOOPEND, ATRANSIT, ARIGHTBR, AEND, ATO, ADOG, ALET, AIFGO, AKEEP, ALABEL, ACHECK, ACONTROL, AINCREASE, AGO, APLUS, AMINUS, AMULT, ADIV, AEXP, ASINGPLUS, ASINGMINUS };
enum atoms{ AT_END, AT_NS, AT_LET, AT_GO, AT_KEEPGO, AT_RETGO, AT_IFGO, AT_KEEP, AT_LABEL, AT_CHECK, AT_INCREASE, AT_PLUS, AT_MINUS, AT_MULT, AT_DIV, AT_EXP, AT_SINGPLUS,AT_SINGMINUS };

class synt :public lexer
{
protected:
	struct atom
	{
		atoms type;
		int atribute1;
		int atribute2;
		int atribute3;
		int atribute4;
		friend std::ostream& operator<<(std::ostream& out, atom& a)
		{
			out << a.type << " " << a.atribute1 << " " << a.atribute2 << " " << a.atribute3 << " " << a.atribute4 << std::endl;
			return out;
		}
	};
	stack <int>st;
	syntLexemes cur;
	int strNum;
	int poz;
	vector<atom>TA;
	static const int nsize = 34;
	static const int msize = 23;
	int HOBTX;
	int HOBT;
	int HOBTAM;
	typedef void(synt::*func)();
	func synt_table[nsize][msize];
public:
	synt()
	{
		st.push(ADOG);
		st.push(APROGRAMM);
		poz = 0;
		HOBTX =  HOBTAM = 1;
		for (int i = 0; i < 23; i++)
		{
			synt_table[APROGRAMM][i] = &synt::A;
			synt_table[APRBODY][i] = &synt::B;
			synt_table[ASTEP][i] = &synt::G1;
			synt_table[AOTHERSTR][i] = &synt::D;
			synt_table[AEXPRESS][i] =synt_table[ATERM][i] = synt_table[AMULTIPLIER][i] = synt_table[APRIMARY][i] = &synt::K4;
			synt_table[AE_NODE][i] = synt_table[AT_NODE][i] = synt_table[ALOOPEND][i] =synt_table[AEND][i] = &synt::O;
			synt_table[AF_NODE][i] = &synt::f26;
			synt_table[ARATIO][i] = &synt::E1;
			synt_table[ATRANSIT][i] = &synt::M1;
			synt_table[ARIGHTBR][i] = &synt::H1;			
			synt_table[ATO][i] = &synt::P1;
			synt_table[ADOG][i] = &synt::R;
			synt_table[ALET][i] = &synt::d;
			synt_table[AIFGO][i] = &synt::e;
			synt_table[AKEEP][i] = &synt::f;
			synt_table[ALABEL][i] = &synt::g;
			synt_table[ACHECK][i] = &synt::h;
			synt_table[ACONTROL][i] = &synt::i;
			synt_table[AINCREASE][i] = &synt::j;
			synt_table[AGO][i] = &synt::k;
			synt_table[APLUS][i] = &synt::l;
			synt_table[AMINUS][i] = &synt::m;
			synt_table[AMULT][i] = &synt::n;
			synt_table[ADIV][i] = &synt::o;
			synt_table[AEXP][i] = &synt::p;
			synt_table[ASINGPLUS][i] = &synt::q;
			synt_table[ASINGMINUS][i] = &synt::r;
		}
		synt_table[APROGRAMM][S_LABEL] = &synt::f1;	
		synt_table[APROGRAMM][S_ERR] = &synt::C;
		synt_table[APROGRAMM][S_ENDM] = &synt::T1;			
		synt_table[APRBODY][S_LABEL] = &synt::f3;
		synt_table[APRBODY][S_LOOPEND] = synt_table[APRBODY][S_END] = &synt::f2;
		synt_table[APRBODY][S_LET] = &synt::f4;
		synt_table[APRBODY][S_FOR] = &synt::f9;
		synt_table[APRBODY][S_GOTO] = &synt::f5; 
		synt_table[APRBODY][S_GOSUB] = &synt::f7;
		synt_table[APRBODY][S_IF] = &synt::f6;
		synt_table[APRBODY][S_RETURN] = &synt::f8;
		synt_table[APRBODY][S_REM] = &synt::f12;
		synt_table[APRBODY][S_ERR] = &synt::C;
		synt_table[APRBODY][S_ENDM] = &synt::T2;
		synt_table[ASTEP][S_LABEL] = &synt::f11;
		synt_table[ASTEP][S_RIGHTBR] = &synt::G2;
		synt_table[ASTEP][S_STEP] = &synt::f10;
		synt_table[ASTEP][S_ERR] = &synt::C;
		synt_table[ASTEP][S_ENDM] = &synt::T2;			
		synt_table[AOTHERSTR][S_LABEL] = &synt::f13;
		synt_table[AOTHERSTR][S_ERR] = &synt::C;
		synt_table[AOTHERSTR][S_ENDM] = &synt::T2;			
		synt_table[AEXPRESS][S_LABEL] = &synt::K2;
		synt_table[AEXPRESS][S_OPERAND] = synt_table[AEXPRESS][S_LEFTBR] = &synt::f14;
		synt_table[AEXPRESS][S_RATIO] = synt_table[AEXPRESS][S_GOTO]= synt_table[AEXPRESS][S_TO] = synt_table[AEXPRESS][S_STEP] = &synt::K1;
		synt_table[AEXPRESS][S_RIGHTBR] = &synt::K3;
		synt_table[AEXPRESS][S_PLUS] = &synt::f15;
		synt_table[AEXPRESS][S_MINUS] = &synt::f16;
		synt_table[AEXPRESS][S_MULT] = synt_table[AEXPRESS][S_DIV] = synt_table[AEXPRESS][S_POW] = &synt::K5;
		synt_table[AEXPRESS][S_ERR] = &synt::C;
		synt_table[AEXPRESS][S_ENDM] = &synt::T3;			
		synt_table[ATERM][S_LABEL] = &synt::K2;
		synt_table[ATERM][S_OPERAND] = synt_table[ATERM][S_LEFTBR] = &synt::f20;
		synt_table[ATERM][S_RATIO] = synt_table[ATERM][S_GOTO] = synt_table[ATERM][S_TO] = synt_table[ATERM][S_STEP] = &synt::K1;
		synt_table[ATERM][S_RIGHTBR] = &synt::K3;
		synt_table[ATERM][S_PLUS] = synt_table[ATERM][S_MINUS] = synt_table[ATERM][S_MULT] = synt_table[ATERM][S_DIV] = synt_table[ATERM][S_POW] = &synt::K6;
		synt_table[ATERM][S_ERR] = &synt::C;
		synt_table[ATERM][S_ENDM] = &synt::T3;			
		synt_table[AMULTIPLIER][S_LABEL] = &synt::K2;
		synt_table[AMULTIPLIER][S_OPERAND] = synt_table[AMULTIPLIER][S_LEFTBR] = &synt::f24;
		synt_table[AMULTIPLIER][S_RATIO] = synt_table[AMULTIPLIER][S_GOTO] = synt_table[AMULTIPLIER][S_TO] = synt_table[AMULTIPLIER][S_STEP] = &synt::K1;
		synt_table[AMULTIPLIER][S_RIGHTBR] = &synt::K3;
		synt_table[AMULTIPLIER][S_PLUS] = synt_table[AMULTIPLIER][S_MINUS] = synt_table[AMULTIPLIER][S_MULT] = synt_table[AMULTIPLIER][S_DIV] = synt_table[AMULTIPLIER][S_POW] = &synt::K6;
		synt_table[AMULTIPLIER][S_ERR] = &synt::C;
		synt_table[AMULTIPLIER][S_ENDM] = &synt::T3;			
		synt_table[APRIMARY][S_LABEL] = &synt::K2;
		synt_table[APRIMARY][S_OPERAND] = &synt::f28;
		synt_table[APRIMARY][S_LEFTBR] = &synt::f27;
		synt_table[APRIMARY][S_RATIO] = synt_table[APRIMARY][S_GOTO] = synt_table[APRIMARY][S_TO] = synt_table[APRIMARY][S_STEP] = &synt::K1;
		synt_table[APRIMARY][S_RIGHTBR] = &synt::K3;
		synt_table[APRIMARY][S_PLUS] = synt_table[APRIMARY][S_MINUS] = synt_table[APRIMARY][S_MULT] = synt_table[APRIMARY][S_DIV] = synt_table[APRIMARY][S_POW] = &synt::K6;
		synt_table[APRIMARY][S_ERR] = &synt::C;
		synt_table[APRIMARY][S_ENDM] = &synt::T3;			
		synt_table[AE_NODE][S_OPERAND] = synt_table[AE_NODE][S_LEFTBR] = &synt::K7;
		synt_table[AE_NODE][S_LABEL] = synt_table[AE_NODE][S_RATIO] = synt_table[AE_NODE][S_GOTO] = synt_table[AE_NODE][S_RIGHTBR] = synt_table[AE_NODE][S_TO] = synt_table[AE_NODE][S_STEP] = &synt::f19;
		synt_table[AE_NODE][S_PLUS] = &synt::f17;
		synt_table[AE_NODE][S_MINUS] = &synt::f18;		
		synt_table[AT_NODE][S_OPERAND] = synt_table[AE_NODE][S_LEFTBR] = &synt::K7;
		synt_table[AT_NODE][S_LABEL] = synt_table[AT_NODE][S_RATIO] = synt_table[AT_NODE][S_GOTO] = synt_table[AT_NODE][S_RIGHTBR] = synt_table[AT_NODE][S_TO] = synt_table[AT_NODE][S_STEP] = synt_table[AT_NODE][S_PLUS] = synt_table[AT_NODE][S_MINUS] = &synt::f23;
		synt_table[AT_NODE][S_MULT] = &synt::f21;
		synt_table[AT_NODE][S_DIV] = &synt::f22;			
		synt_table[AF_NODE][S_OPERAND] = synt_table[AF_NODE][S_LEFTBR] = &synt::K7;
		synt_table[AF_NODE][S_LOOPEND] = synt_table[AF_NODE][S_LET] = synt_table[AF_NODE][S_FOR] = synt_table[AF_NODE][S_GOSUB] = synt_table[AF_NODE][S_IF] = synt_table[AF_NODE][S_RETURN] = synt_table[AF_NODE][S_END] = synt_table[AF_NODE][S_REM] = &synt::K4;
		synt_table[AF_NODE][S_POW] = &synt::f25;
		synt_table[AF_NODE][S_ERR] = &synt::C;
		synt_table[AF_NODE][S_ENDM] = &synt::T2;			
		synt_table[ARATIO][S_OPERAND] = &synt::E2;
		synt_table[ARATIO][S_RATIO] = &synt::b;
		synt_table[ARATIO][S_RIGHTBR] = &synt::E3;
		synt_table[ARATIO][S_ERR] = &synt::C;
		synt_table[ARATIO][S_ENDM] = &synt::T3;			
		synt_table[ALOOPEND][S_LOOPEND] = &synt::b;
		synt_table[ALOOPEND][S_END] = &synt::L;			
		synt_table[ATRANSIT][S_GOTO] = &synt::b;
		synt_table[ATRANSIT][S_LABEL] = &synt::M2;
		synt_table[ATRANSIT][S_RATIO] = &synt::M3;
		synt_table[ATRANSIT][S_LEFTBR] = &synt::M4;
		synt_table[ATRANSIT][S_ERR] = &synt::C;
		synt_table[ATRANSIT][S_ENDM] = &synt::T3;
		synt_table[ARIGHTBR][S_LABEL] = &synt::H2;
		synt_table[ARIGHTBR][S_RIGHTBR] = &synt::a;
		synt_table[ARIGHTBR][S_ERR] = &synt::C;
		synt_table[ARIGHTBR][S_ENDM] = &synt::T3;			
		synt_table[AEND][S_LOOPEND] = &synt::K;
		synt_table[AEND][S_END] = &synt::a;			
		synt_table[ATO][S_LABEL] = &synt::P2;
		synt_table[ATO][S_RIGHTBR] = &synt::P3;
		synt_table[ATO][S_TO] = &synt::a;
		synt_table[ATO][S_STEP] = &synt::P4;
		synt_table[ATO][S_ERR] = &synt::C;
		synt_table[ATO][S_ENDM] = &synt::T3;			
		synt_table[ADOG][S_ERR] = &synt::C;
		synt_table[ADOG][S_ENDM] = &synt::c;
	}
	
	void showTS(ostream&out){ for (std::vector<atom>::iterator i = TA.begin(); i != TA.end(); ++i) out << *i << std::endl; }
	void start(char * filename)
	{
		run(filename);
		HOBT = ++NTO;
		int x = 5;
		table_lexemes.push_back(lexeme(l_endfile, -1));
		ofstream out("example.txt");
		while (poz < table_lexemes.size() && !st.empty())
		{
			translate(table_lexemes[poz]);
			out << "Лексема: " << cur  << endl;
			showStack(out);
			out << endl;
			(this->*synt_table[st.top()][cur])();
		}
	}
	void showAtoms()
	{
		ofstream out("outAtoms.doc");
		if (!out) cout << "Can't open outAtoms.doc\n";
		vector <string> atoms = { "AT_END", "AT_NS", "AT_LET", "AT_GO", "AT_KEEPGO", "AT_RETGO", "AT_IFGO", "AT_KEEP", "AT_LABEL", "AT_CHECK", "AT_INCREASE", "AT_PLUS", "AT_MINUS", "AT_MULT", "AT_DIV", "AT_EXP", "AT_SINGPLUS", "AT_SINGMINUS" };
		for (vector<atom>::iterator i = TA.begin(); i != TA.end(); i++)
		{
			//out.width(20);
			//out.fill(' ');
			//cout << d << "\t" << setw(20) << setfill('_') << d << "\t" <<  setprecision(3) << d << setfill(' ') << endl;
			out << i->type << "\t|" << setw(5) << i->atribute1 << "\t|" << setw(5) << i->atribute2 << "\t|" << setw(5) << i->atribute3 << "\t|" << setw(5) << i->atribute4 << endl;
		}
		out.close();
	}
	void showStack(ofstream&out)
	{
		//vector <string> ntrmnl = { "nt_pr", "nt_body_pr", "nt_step", "nt_dr_str", "nt_expression", "nt_term", "nt_multiplier", "nt_primary", "nt_e_list", "nt_t_list", "nt_f_list", "nt_ratio", "nt_endcicle", "nt_goto", "nt_r_br", "nt_end", "nt_to", "nt_at_sign", "nt_assign", "nt_usl_per", "nt_hranit", "nt_label", "nt_prover", "nt_control", "nt_uveli4", "nt_perehod", "nt_slozh", "nt_vy4it", "nt_umnozh", "nt_delen", "nt_exponent", "nt_plus", "nt_minus" };
		stack<int> temp_st;
		out << "Stack: ";
		while (!st.empty())
		{
			//	cout << ntrmnl[st.top()] << " ";
			out << st.top() << " ";
			temp_st.push(st.top());
			st.pop();
		}
		out << endl;
		while (!temp_st.empty())
		{
			st.push(temp_st.top());
			temp_st.pop();
		}
	}

private:

	void f1()
	{
		st.pop();
		st.push(AEND);
		st.push(APRBODY);
		strNum = table_lexemes[poz].lexeme_value;
		atom a = createAtom(AT_NS, strNum, -1, -1, -1);
		TA.push_back(a);
		poz++;
	}
	void f2()
	{
		st.pop();
	}
	void f3()
	{
		st.pop();
		st.push(APRBODY);
		strNum = table_lexemes[poz].lexeme_value;
		atom a = createAtom(AT_NS, strNum, -1, -1, -1);
		TA.push_back(a);
		poz++;
	}
	void f4()
	{
		st.pop();
		st.push(AOTHERSTR);
		st.push(0);
		st.push(table_lexemes[poz].lexeme_value);
		st.push(ALET);
		st.push(3);
		st.push(AEXPRESS);
		poz++;
	}
	void f5()
	{
		st.pop();
		st.push(AOTHERSTR);
		atom a = createAtom(AT_GO, table_lexemes[poz].lexeme_value, -1, -1, -1);
		TA.push_back(a);
		poz++;
	}
	void f6()
	{
		st.pop();
		st.push(AOTHERSTR);
		st.push(0);
		st.push(0);
		st.push(0);
		st.push(0);
		st.push(AIFGO);
		st.push(5);
		st.push(ATRANSIT);
		st.push(5);
		st.push(AEXPRESS);
		st.push(8);
		st.push(ARATIO);
		st.push(8);
		st.push(AEXPRESS);
		poz++;
	}
	void f7()
	{
		st.pop();
		st.push(AOTHERSTR);
		atom a = createAtom(AT_KEEPGO, table_lexemes[poz].lexeme_value, -1, -1, -1);
		TA.push_back(a);
		poz++;
	}
	void f8()
	{
		st.pop();
		st.push(AOTHERSTR);
		atom a = createAtom(AT_RETGO, -1, -1, -1, -1);
		TA.push_back(a);
		poz++;
	}
	void f9()
	{
		st.pop();
		st.push(AOTHERSTR);
		st.push(HOBTAM + 1);
		st.push(ALABEL);
		st.push(HOBTAM);
		st.push(AGO);
		st.push(HOBTX + 1);
		st.push(table_lexemes[poz].lexeme_value);
		st.push(AINCREASE);
		st.push(strNum);
		st.push(0);
		st.push(table_lexemes[poz].lexeme_value);
		st.push(ACONTROL);
		st.push(3);
		st.push(ALOOPEND);
		st.push(AOTHERSTR);
		st.push(HOBTAM + 1);
		st.push(HOBTX + 1);
		st.push(HOBTX);
		st.push(table_lexemes[poz].lexeme_value);
		st.push(ACHECK);
		st.push(HOBTAM);
		st.push(ALABEL);
		st.push(HOBTX + 1);
		st.push(0);
		st.push(AKEEP);
		st.push(2);
		st.push(ASTEP);
		st.push(HOBTX);
		st.push(0);
		st.push(AKEEP);
		st.push(2);
		st.push(AEXPRESS);
		st.push(ATO);
		st.push(0);
		st.push(table_lexemes[poz].lexeme_value);
		st.push(ALET);
		st.push(3);
		st.push(AEXPRESS);
		HOBTAM++;
		HOBTX++;
		poz++;
	}
	void f10()
	{
		st.pop();
		st.push(AEXPRESS);
		poz++;
	}
	void f11()
	{
		st.pop();
		replace(st.top(), 0);
	}
	void f12()
	{
		st.pop();
		st.push(AOTHERSTR);
		poz++;
	}
	void f13()
	{
		st.pop();
		st.push(APRBODY);
		strNum = table_lexemes[poz].lexeme_value;
		atom a = createAtom(AT_NS, strNum, -1, -1, -1);
		TA.push_back(a);
		poz++;

	}
	void f14()
	{
		st.pop();
		st.push(0);
		st.push(AE_NODE);
		st.push(2);
		st.push(ATERM);
		//poz++;
	}
	void f15()
	{
		st.pop();
		st.push(HOBT);
		st.push(AE_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(ASINGPLUS);
		st.push(2);
		st.push(ATERM);
		HOBT++;
		poz++;
	}
	void f16()
	{
		st.pop();
		st.push(HOBT);
		st.push(AE_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(ASINGMINUS);
		st.push(2);
		st.push(ATERM);
		HOBT++;
		poz++;
	}
	void f17()
	{
		st.pop();
		int v = st.top(); st.pop();
		st.push(HOBT);
		st.push(AE_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(v);
		st.push(APLUS);
		st.push(3);
		st.push(ATERM);
		HOBT++;
		poz++;
	}
	void f18()
	{
		st.pop();
		int v = st.top(); st.pop();
		st.push(HOBT);
		st.push(AE_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(v);
		st.push(AMINUS);
		st.push(3);
		st.push(ATERM);
		HOBT++;
		poz++;
	}
	void f19()
	{
		st.pop();
		int v = st.top();
		st.pop();
		replace(st.top(), v);
		st.pop();

	}
	void f20()
	{
		st.pop();
		st.push(0);
		st.push(AT_NODE);
		st.push(2);
		st.push(AMULTIPLIER);
	}
	void f21()
	{
		st.pop();
		int v = st.top(); st.pop();
		st.push(HOBT);
		st.push(AT_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(v);
		st.push(AMULT);
		st.push(3);
		st.push(AMULTIPLIER);
		HOBT++;
		poz++;
	}
	void f22()
	{
		st.pop();
		int v = st.top(); st.pop();
		st.push(HOBT);
		st.push(AT_NODE);
		st.push(HOBT);
		st.push(0);
		st.push(v);
		st.push(ADIV);
		st.push(3);
		st.push(AMULTIPLIER);
		HOBT++;
		poz++;

	}
	void f23()
	{
		st.pop();
		int v = st.top();
		st.pop();
		replace(st.top(), v);
		st.pop();
	}
	void f24()
	{
		st.pop();
		st.push(0);
		st.push(AF_NODE);
		st.push(2);
		st.push(APRIMARY);
	}
	void f25()
	{
		st.pop();
		int v = st.top(); st.pop();
		st.push(HOBT);
		st.push(AF_NODE);
		st.push(HOBTX);
		st.push(0);
		st.push(v);
		st.push(AEXP);
		st.push(3);
		st.push(APRIMARY);
		HOBT++;
		poz++;
	}
	void f26()
	{
		st.pop();
		int v = st.top();
		st.pop();
		replace(st.top(), v);
		st.pop();

	}
	void f27()
	{
		st.pop();
		//int a = st.top(); st.pop();
		st.pop();
		st.push(ARIGHTBR);
		//st.push(table_lexemes[poz].lexeme_value);
		//st.push(a);
		st.push(3);
		st.push(AEXPRESS);
		poz++;
	}
	void f28()
	{
		st.pop();
		replace(st.top(), table_lexemes[poz].lexeme_value);
		st.pop();
		poz++;
	}

	void a()
	{
		st.pop();
		poz++;
	}
	void b()
	{
		st.pop();
		replace(st.top(), table_lexemes[poz].lexeme_value);
		st.pop();
		poz++;
	}
	void c()
	{
		atom a = createAtom(AT_END, -1, -1, -1, -1);
		TA.push_back(a);
		st.pop();
	}
	void d()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		atom a = createAtom(AT_LET, p, q, -1, -1);
		TA.push_back(a);
	}
	void e()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		int s = st.top();
		st.pop();
		atom a = createAtom(AT_IFGO, p, q, r, s);
		TA.push_back(a);
	}
	void f()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		atom a = createAtom(AT_KEEP, p, q, -1, -1);
		TA.push_back(a);
	}
	void g()
	{
		st.pop();
		int p = st.top();
		st.pop();
		atom a = createAtom(AT_LABEL, p, -1, -1, -1);
		TA.push_back(a);
	}
	void h()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		int v = st.top();
		st.pop();
		atom a = createAtom(AT_CHECK, p, q, r, v);
		TA.push_back(a);
	}
	void i()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		if (q != p)
			cout << "Переменная NEXT-оператора отлична от переменной FOR-оператора. " << st.top() << " - предполагаемая переменная" << endl;
		st.pop();
	}
	void j()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		atom a = createAtom(AT_INCREASE, p, q, -1, -1);
		TA.push_back(a);
	}
	void k()
	{
		st.pop();
		int p = st.top();
		st.pop();
		atom a = createAtom(AT_GO, p, -1, -1, -1);
		TA.push_back(a);

	}
	void l()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		atom a = createAtom(AT_PLUS, p, q, r, -1);
		TA.push_back(a);
	}
	void m()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		atom a = createAtom(AT_MINUS, p, q, r, -1);
		TA.push_back(a);
	}
	void n()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		atom a = createAtom(AT_MULT, p, q, r, -1);
		TA.push_back(a);
	}
	void o()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		atom a = createAtom(AT_DIV, p, q, r, -1);
		TA.push_back(a);
	}
	void p()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		int r = st.top();
		st.pop();
		atom a = createAtom(AT_EXP, p, q, r, -1);
		TA.push_back(a);
	}
	void q()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		atom a = createAtom(AT_SINGPLUS, p, q, -1, -1);
		TA.push_back(a);
	}
	void r()
	{
		st.pop();
		int p = st.top();
		st.pop();
		int q = st.top();
		st.pop();
		atom a = createAtom(AT_SINGMINUS, p, q, -1, -1);
		TA.push_back(a);
	}

	void A(){ cout << "Программа начинается неверно" << endl; poz = table_lexemes.size(); }
	void B(){ cout << "Оператор начинается неверно"<< endl; poz = table_lexemes.size(); }
	void G1(){ cout << "Неожиданный " << to_string(cur) << " в конце FOR - оператора" << endl; }
	void G2(){ cout << "Лишняя правая скобка в выражении после TO" << endl; poz = table_lexemes.size(); }
	void D(){ cout << "Неожиданный " << to_string(cur) << " после окончания оператора" << endl;	poz = table_lexemes.size(); }
	void K1(){ cout << "Выражение незакончено - нет операнда" << endl; poz = table_lexemes.size(); }
	void K2(){ cout << "Оператор незакончен" << endl; poz = table_lexemes.size(); }
	void K3(){ cout << "В выражении пропущен операнд"<< endl; poz = table_lexemes.size(); }
	void K4(){ cout << "Неожиданный " << to_string(cur) << " после выражения" << endl; poz = table_lexemes.size(); }
	void K5(){ cout << "Выражение начинается с " << to_string(cur) << endl; poz = table_lexemes.size(); }
	void K6(){ cout << "Два знака операции подряд в выражении"<< endl; poz = table_lexemes.size(); }
	void K7(){ cout << "В выражении пропущен знак операции"<< endl; poz = table_lexemes.size(); }
	void E1(){ cout << "Неожиданный " << to_string(cur) << " в IF операторе" << endl; poz = table_lexemes.size(); }
	void E2(){ cout << "IF оператор незакончен"<< endl; poz = table_lexemes.size(); }
	void E3(){ cout << "Лишняя правая скобка в выражении после IF" << endl; poz = table_lexemes.size(); }
	void L(){ cout << "FOR операторы вложены неправильно, пропущено NEXT" << endl; poz = table_lexemes.size(); }
	void M1(){ cout << "Неожиданный " << to_string(cur) << " в IF операторе" << endl; poz = table_lexemes.size(); }
	void M2(){ cout << "IF оператор незакончен"<< endl; poz = table_lexemes.size(); }
	void M3(){ cout << "Два знака отношения в IF операторе"<< endl; poz = table_lexemes.size(); }
	void M4(){ cout << "Лишняя правая скобка в выражении после знака отношения"<< endl; poz = table_lexemes.size(); }
	void H1(){ cout << "Пропущена правая скобка перед " << to_string(cur) << endl; poz = table_lexemes.size(); }
	void H2(){ cout << "Пропущена правая скобка в конце строки"<< endl; poz = table_lexemes.size(); }
	void K(){ cout << "FOR-операторы вложены неправильно - лишнее NEXT" << endl; poz = table_lexemes.size(); }
	void P1(){ cout << "Неожиданный " << to_string(cur) << " в FOR операторе" << endl; poz = table_lexemes.size(); }
	void P2(){ cout << "FOR оператор незакончен"<< endl; poz = table_lexemes.size(); }
	void P3(){ cout << "Лишняя правая скобка в выражении после -"<< endl; poz = table_lexemes.size(); }
	void P4(){ cout << "TO пропущено или не на своём месте в FOR операторе"<< endl; poz = table_lexemes.size(); }
	void R(){ cout << "Программа продолжается после END-оператора"<< endl; poz = table_lexemes.size(); }
	void C(){ cout << " "<< endl; poz = table_lexemes.size(); }
	void T1(){ cout << "Нет программы"<< endl; poz = table_lexemes.size(); }
	void T2(){ cout << "Пропущен END-оператор"<< endl; poz = table_lexemes.size(); }
	void T3(){ cout << "Программа кончается посреди оператора"<< endl; poz = table_lexemes.size(); }
	void O(){ cout << "Ошибка компилятора"<< endl; poz = table_lexemes.size(); }
		
	atom createAtom(atoms type, int a1, int a2, int a3, int a4)
	{
		atom a;
		a.type = type;
		a.atribute1 = a1;
		a.atribute2 = a2;
		a.atribute3 = a3;
		a.atribute4 = a4;
		return a;
	}
	void translate(lexeme& lexem)
	{
		switch (lexem.lexeme_type)
		{
		case l_label:cur = S_LABEL; break;
		case l_operand:cur = S_OPERAND; break;
		case l_ratio:cur = S_RATIO; break;
		case l_next:cur = S_LOOPEND; break;
		case l_let:cur = S_LET; break;
		case l_for:cur = S_FOR; break;
		case l_goto:cur = S_GOTO; break;
		case l_gosub:cur = S_GOSUB; break;
		case l_left_bracket:cur = S_LEFTBR; break;
		case l_right_bracket:cur = S_RIGHTBR; break;
		case l_if:cur = S_IF; break;
		case l_return:cur = S_RETURN; break;
		case l_end:cur = S_END; break;
		case l_to:cur = S_TO; break;
		case l_step:cur = S_STEP; break;
		case l_rem:cur = S_REM; break;
		case l_operation:
		{
			switch ((int)lexem.lexeme_value)
			{
			case 1: cur = S_PLUS; break;
			case 2: cur = S_MINUS; break;
			case 3:cur = S_MULT; break;
			case 4: cur = S_DIV; break;
			case 5: cur = S_PLUS; break;//pow а не plus
			}
			break;
		}
		case l_error:cur = S_ERR; break;
		case l_endfile:cur = S_ENDM; break;
		}

	}
	void replace(int depth, int value) // maybe fix
	{
		stack<int> temp_st;
		int temp;
		for (int i = 0; i < depth; i++)
		{
			temp = st.top();
			st.pop();
			temp_st.push(temp);
		}
		st.pop();
		st.push(value);
		for (int i = 0; i < depth; i++)
		{
			temp = temp_st.top();
			temp_st.pop();
			st.push(temp);
		}
	}
};