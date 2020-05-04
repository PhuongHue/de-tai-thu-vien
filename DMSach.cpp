#ifndef _DANHMUC_CPP_
#define _DANHMUC_CPP_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Sach
{
	int maSach;
	int trangThai;
	string viTri;
};

int compareData(Sach *a, Sach *b)
{
	int cmpMSSV = a->maSach - b->maSach;
	if (cmpMSSV != 0)
		return cmpMSSV;
	else
	{
		int cmpTrangThai = a->trangThai - b->trangThai;
		if (cmpTrangThai != 0)
			return cmpTrangThai;
		else
		{
			int cmpViTri = a->viTri.compare(b->viTri);
			return cmpViTri;
		}
	}
};

struct DMSach
{
	Sach *data = NULL;
	DMSach *next = NULL;
};

struct PairNode
{
	DMSach *before = NULL;
	DMSach *value = NULL;
};

typedef void (*CallBackDMSach)(DMSach *node);

void foreachDMSach(DMSach *first, CallBackDMSach callBack)
{
	DMSach *p = first;
	while (p != NULL)
	{
		callBack(p);
		p = p->next;
	}
}

void swapNodeData(DMSach *&a, DMSach *&b)
{
	Sach *c = a->data;
	a->data = b->data;
	b->data = c;
}

void addFirst(DMSach *&first, Sach *info)
{
	if (first == NULL)
	{
		first = new DMSach;
		first->data = info;
		return;
	}
	DMSach *p = new DMSach;
	p->data = info;
	p->next = first;
}

void addLast(DMSach *&first, Sach *info)
{
	if (first == NULL)
	{
		first = new DMSach;
		first->data = info;
		return;
	}
	DMSach *p = first;
	while (p->next)
		p = p->next;
	DMSach *q = new DMSach;
	q->data = info;
	p->next = q;
}

int countAll(DMSach *first)
{
	DMSach *p = first;
	int count = 0;
	while (p != NULL)
	{
		count++;
		p = p->next;
	}
	return count;
}

PairNode findByMaSach(DMSach *first, int key)
{
	DMSach *p = first;
	DMSach *pBefore = NULL;
	DMSach *kq = NULL;
	while (p != NULL)
	{
		Sach *info = p->data;
		if (info->maSach == key)
		{
			kq = p;
			break;
		}
		pBefore = p;
		p = p->next;
	}
	PairNode pair;
	pair.before = pBefore;
	pair.value = kq;
	return pair;
}

void deleteAll(DMSach *&first)
{
	DMSach *p = first;
	first = NULL;
	while (p != NULL)
	{
		DMSach *x = p;
		p = p->next;
		delete x->data;
		delete x;
	}
}

bool deleteByMaSach(DMSach *&first, int key)
{
	PairNode pn = findByMaSach(first, key);
	if (pn.value == NULL)
		return false;
	else
	{
		if (pn.before == NULL)
			first = pn.value->next;
		else
		{
			pn.before->next = pn.value->next;
		}
		return true;
	}
}
void luuFile(DMSach *&first, ofstream &fout)
{
	DMSach *p = first;
	fout << countAll(first);
	while (p != NULL && p->data != NULL)
	{
		Sach *data = p->data;
		fout << data->maSach << endl
				 << data->trangThai << endl
				 << data->viTri;
		if (p->next)
			fout << endl;
		p = p->next;
	}
}

void docFile(DMSach *&list, ifstream &fin)
{
	int n;
	fin >> n;
	for (int i = 0; i < n; i++)
	{
		Sach *data = new Sach;
		fin >> data->maSach >> data->trangThai >> data->viTri;
		addLast(list, data);
	}
}

void sortByMS(DMSach *&first)
{
	for (DMSach *i = first; i != NULL && i->next != NULL; i = i->next)
	{
		for (DMSach *j = i->next; j != NULL; j = j->next)
		{
			if (i->data->maSach > j->data->maSach)
				swapNodeData(i, j);
		}
	}
}

int compareDataDanhMuc(Sach *a, Sach *b)
{
	int cmpTrangThai = a->trangThai - b->trangThai;
	if (cmpTrangThai != 0)
		return cmpTrangThai;
	else
	{
		return a->viTri.compare(a->viTri);
	}
}

void sortByTTVT(DMSach *&first)
{
	for (DMSach *i = first; i != NULL && i->next != NULL; i = i->next)
	{
		for (DMSach *j = i->next; j != NULL; j = j->next)
		{
			if (compareDataDanhMuc(i->data, j->data) > 0)
				swapNodeData(i, j);
		}
	}
}

#endif
