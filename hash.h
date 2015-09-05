#include <iostream>

using namespace std;

struct hash_table_row
{
	int key;
	int value;
	int next;

	bool empty()
	{
		return (key == -1) && (value == -1) && (next == -1);
	}
};

class hash_table
{
	hash_table_row* table;
	int n;
	int top_of_stack; //хранит номер €чейки - головы стека
public:
	hash_table()
	{
		n = 300;
		table = new hash_table_row[n];
		for (int i = 0; i < n; i++) table[i] = { -1, -1, -1 };
		top_of_stack = 0;
	}
	hash_table(int n)
	{
		this->n = n;
		table = new hash_table_row[n];
		for (int i = 0; i < n; i++) table[i] = { -1, -1, -1 };
		top_of_stack = 0;
	}

	~hash_table(){ delete[] table; }

	void print()
	{
		for (int i = 0; i < n; i++)
		if (!table[i].empty())
			cout << i << ".  " << table[i].key << " " << table[i].value << " " << table[i].next << endl;
	}

	void printToFile(ofstream& out_file)
	{
		for (int i = 0; i < n; i++)
		if (!table[i].empty())
			out_file << i << ".  " << table[i].key << " " << table[i].value << " " << table[i].next << endl;
	}

	int hash_func(int num)
	{
		return num % 100 + 1;
	}

	int insert(int num)
	{
		int pos = hash_func(num);
		if (table[pos].empty())
		{
			table[pos].key = pos;
			table[pos].value = num;
			return pos;
		}
		else
		{
			int cur = pos;
			while (table[cur].next != -1 && table[cur].value != num) cur = table[cur].next;
			if (table[cur].value == num) return cur;
			else
			{
				int free_cell = next_free_cell();
				table[free_cell].key = pos;
				table[free_cell].value = num;
				table[cur].next = free_cell;
				return free_cell;
			}
		}
	}

	int next_free_cell()
	{
		if (!top_of_stack == 0 || !table[top_of_stack].next == -1)//если стек не пуст
		{
			int temp = table[0].next;
			while (table[temp].next != -1) temp = table[temp].next;
			return temp;
		}
		else
		{
			int pos = 101;
			while (!table[pos].empty() && pos < n) pos++;
			if (pos >= n)
			{
				cout << "space ended" << endl;
				return 0;
			}
			return pos;
		}
	}

	void remove(int num)
	{
		int pos = hash_func(num);
		if (table[pos].next == -1)
		{
			table[pos].key = -1;
			table[pos].value = -1;
		}
		else
		{
			int cur = pos;
			int prev;
			while (table[cur].next != -1 && table[cur].value != num)
			{
				prev = cur;
				cur = table[cur].next;
			}
			if (table[cur].value == num)
			{
				table[prev].next = table[cur].next;
				table[cur].value = -1;
				table[cur].key = -1;
				table[cur].next = -1;
				table[top_of_stack].next = cur;
				top_of_stack = cur;
			}

		}
	}

	int find(int num)
	{
		int pos = hash_func(num);
		while (pos != -1 && table[pos].value != num) pos = table[pos].next;
		return pos;
	}
};
