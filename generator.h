#include <iostream>

class generator : public synt
{
public:
	generator(){}
	string name(int index)
	{
		return "x" + to_string(index);
	}
	void go(char*str)
	{
		start(str);
		ofstream out("output.txt");
		out << "include\\masm32\\include\\masm32rt.inc\n\n";
		out << ".data?" << endl;
		for (int i = 1; i < 286; i++)
			if (table_operands[i]>0)	out << 'x' << i << "  dd  ?\n";
		out << "\n.data\n\n";
		out << "x0 dd 0" << endl;
		for (int i = 286; i <= HOBT; i++)
			out << 'x' << i << "  dd  " << table_operands[i] << "\n";
		out << "\n.code\nstart:\n\n";
		out << "call main\ninkey\nexit \n";
		out << "\nmain proc \n\n";
		for (vector<atom>::iterator i = TA.begin(); i != TA.end(); i++)
		{
			switch (i->type)
			{
			case AT_END:
				//PRINT R
				//out << "print str$(x18)" << endl;
				out << "mov eax, x18" << endl;
				out << "cmp eax, x0" << endl;
				out << "je line228" << endl;
				out << "jmp line322" << endl;
				out << "line228:" << endl;
				out << "print \"yes\"" << endl;
				out << "jmp line155" << endl;
				out << "line322:" << endl;
				out << "print \"no\"" << endl;
				out << "line155:" << endl;
				out << "ret" << endl;
				break;
			case AT_NS:
				out << "\nline" << i->atribute1 << ":" << endl;
				break;
			case AT_LET:
				out << "mov eax," << name(i->atribute2) << endl;
				out << "mov " << name(i->atribute1) << ", eax" << endl;
				break;
			case AT_GO:
				out << "jmp line" << i->atribute1 << endl;
				break;
				/*
			case AT_KEEPGO:
				out << "push " << "returnlabel" << cur_return << "\n";
				out << "jmp " << "line" << i->atribute1 << "\n";
				out << "returnlabel" << cur_return << ":\n";
				cur_return++;
				break;
				*/
			case AT_RETGO:
				out << "ret\n";
				break; 
			case AT_IFGO:
				out << "mov eax, " << name(i->atribute1) << endl;
				out << "cmp eax, " << name(i->atribute2) << endl;
				switch (i->atribute3 - 1)
				{
				case 0:
					out << "je line" << i->atribute4 << endl;
					break;
				case 1:
					out << "jne line" << i->atribute4 << endl;
					break;
				case 2:
					out << "jl line" << i->atribute4 << endl;
					break;
				case 3:
					out << "jg line" << i->atribute4 << endl;
					break;
				case 4:
					out << "jle line" << i->atribute4 << endl;
					break;
				case 5:
					out << "jge line" << i->atribute4 << endl;
					break;
				default:
					break;
				}
			break;			
			case AT_KEEP:
				out << "mov eax, " <<i->atribute1 << endl;
				out << "mov " << i->atribute2 << " , eax" << endl;
				break;
			case AT_LABEL:
				out << "line" << i->atribute1 << ":" << endl;
				break;
				/*
			case AT_CHECK:
				out << "mov eax, " <<i->atribute3 << "\n";
				out << "cmp eax, " << 0 << "\n";
				out << "JGE labelM" << i->atribute4 << "\n";
				out << "mov eax, " <<i->atribute1 << "\n";
				out << "cmp eax, " << i->atribute2 << "\n";
				out << "JL line" << i->atribute4 << "\n";
				out << "jmp labelL" << i->atribute4 << "\n";
				out << "labelM" << i->atribute4 << ":\n";
				out << "mov eax, " << i->atribute1 << "\n";
				out << "cmp eax, " << i->atribute2 << "\n";
				out << "JG line" << i->atribute4 << "\n";
				out << "labelL" << i->atribute4 << ":\n";
				break;
				*/
			case AT_INCREASE:
				out << "mov eax, " << name(i->atribute1) << endl;
				out << "add eax, " << name(i->atribute2) << endl;
				out << "mov " << name(i->atribute1) << " , eax" << endl;
				break;
			case AT_PLUS:
				out << "mov eax, " << name(i->atribute1) << endl;
				out << "add eax," << name(i->atribute2) << endl;
				out << "mov " << name(i->atribute3) << ", eax" << endl;
				break;
			case AT_MINUS:
				out << "mov eax, " << name(i->atribute1) << endl;
				out << "sub eax," << name(i->atribute2) << endl;
				out << "mov " << name(i->atribute3) << ", eax" << endl;
				break;
			case AT_MULT:
				out << "mov eax, " << name(i->atribute1) << endl;
				out << "imul eax, " << name(i->atribute2) << endl;
				out << "mov " << name(i->atribute3) << ", eax" << endl;
				break;
			case AT_DIV:
				out << "mov   eax, " << name(i->atribute1) << endl;
				out << "cdq\n";
				out << "mov   ebx, " << name(i->atribute2) << endl;
				out << "idiv  ebx\n";
				out << "mov " << name(i->atribute3) << ", eax" << endl;
				break;
			case AT_EXP:
				out << "mov   eax, " << name(i->atribute2) << endl;
				out << "mov ebx, " << name(i->atribute1) << endl;
				out << "B:" << endl;
				out << "cmp eax, 1" << endl;
				out << "je E" << endl;
				out << "sub eax, 1" << endl;
				out << "imul ebx, " << name(i->atribute1) << endl;
				out << "jmp B" << endl;
				out << "E:" << endl;
				out << "mov " << name(i->atribute3) << ", ebx" << endl;
				break;
			case AT_SINGPLUS:
				out << "mov eax, " << "x" << i->atribute1 << "\n";
				out << "mov " << "x" << i->atribute2 << ", eax\n";
				break;
			case AT_SINGMINUS:
				out << "mov eax, " << "x" << i->atribute1 << "\n";
				out << "imul eax, -1\n";
				out << "mov " << "x" << i->atribute2 << ", eax\n";
				break;
			default:
				break;
			}
		}
		out << "main endp \n\nend start\n";
		out.close();
	}
};