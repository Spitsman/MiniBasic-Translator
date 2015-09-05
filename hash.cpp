#include "stdafx.h"
#include "hash.h"

Hash::Hash()
{
	nts = 0;
	size = 101;
	sv_ts = 100;
	ts = new el_ts[size];
	for (int i = 0; i < size; i++) ts[i] = -1;
}

Hash::~Hash(){ if (ts) delete[] ts; }

int Hash::H_Func(int key) { return key % 100; }

void Hash::H_Print()
{
	cout << endl << "N" << "\t" << "METKA" << "\t" << "INDEX" << "\t" << "NEXT" << endl;
	for (int i = 0; i < size; i++)
	{
		if (ts[i].metka != -1)
			cout << i << "\t" << ts[i].metka << "\t" << ts[i].index << "\t" << ts[i].next << endl;
	}
	cout << endl;
}

int Hash::H_Find(int key)
{
	int i = H_Func(key);
	while (ts[i].next != -1)
	{
		if (ts[i].metka == key) return i;
		i = ts[i].next;
	}
	if (ts[i].metka == key) return i;
	return -1;
}

void Hash::H_New()
{
	el_ts*tmp;
	tmp = new el_ts[size];
	for (int i = 0; i < size; i++) tmp[i] = ts[i];
	delete ts;
	size *= 2;
	ts = new el_ts[size];
	for (int i = 0; i < size/2; i++) ts[i] = tmp[i];
	delete tmp;
	for (int i = size / 2; i <= size; i++) ts[i] = -1;
}

void Hash::H_Add(int key, int ind)
{
	int tmp; 
	int i = H_Func(key);
	int k = i;
	if (H_Find(key) == -1)
	{
		if (ts[i].metka == -1)
		{
			ts[i].metka = key;
			ts[i].index = ind;
		}
		else
		{
			if (st_sv.empty())
			{

			}
		}
	}
}