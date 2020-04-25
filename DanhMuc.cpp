#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct LNData
{
	int maSach;
	int trangThai;
	string viTri;
};

int compareData(LNData *a, LNData *b)
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
}

struct ListNode
{
	LNData *data = NULL;
	ListNode *next = NULL;
};

struct PairNode
{
	ListNode *before = NULL;
	ListNode *value = NULL;
};

void swapNodeData(ListNode *&a, ListNode *&b)
{
	LNData *c = a->data;
	a->data = b->data;
	b->data = c;
}

void addFirst(ListNode *&first, LNData *info)
{
	if (first == NULL)
	{
		first = new ListNode;
		first->data = info;
		return;
	}
	ListNode *p = new ListNode;
	p->data = info;
	p->next = first;
}

void addLast(ListNode *&first, LNData *info)
{
	if (first == NULL)
	{
		first = new ListNode;
		first->data = info;
		return;
	}
	ListNode *p = first;
	while (p->next)
		p = p->next;
	ListNode *q = new ListNode;
	q->data = info;
	p->next = q;
}

int countAll(ListNode *first)
{
	ListNode *p = first;
	int count = 0;
	while (p != NULL)
	{
		count++;
		p = p->next;
	}
	return count;
}

PairNode findBySinhVien(ListNode *first, LNData key)
{
	ListNode *p = first;
	ListNode *pBefore = NULL;
	ListNode *kq = NULL;
	while (p != NULL)
	{
		LNData *info = p->data;
		if (info->maSach == key.maSach || info->trangThai == key.trangThai || info->viTri.compare(key.viTri) == 0)
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

void deleteAll(ListNode *&first)
{
	ListNode *p = first;
	first = NULL;
	while (p != NULL)
	{
		ListNode *x = p;
		p = p->next;
		delete x->data;
		delete x;
	}
}

bool deleteBySinhVien(ListNode *&first, LNData key)
{
	PairNode pn = findBySinhVien(first, key);
	if (pn.value == NULL)
		return false;
	else {
		if(pn.before == NULL) first = pn.value->next;
		else {
			pn.before->next = pn.value->next;
		}
	}
}
void luuFile(ListNode *&first)
{
	ofstream fout;
	fout.open("DanhMuc.data");
	ListNode *p = first;
	while (p != NULL && p->data != NULL)
	{
		LNData *data = p->data;
		fout << data->maSach << endl
				 << data->trangThai << endl
				 << data->viTri;
		if (p->next)
			fout << endl;
		p = p->next;
	}
	fout.close();
}

void docFile(ListNode *&list)
{
	deleteAll(list);
	ifstream fin;
	fin.open("DanhMuc.data");
	while (!fin.eof())
	{
		LNData *data = new LNData;
		fin >> data->maSach >> data->trangThai >> data->viTri;
		addLast(list, data);
	}
	fin.close();
}

void nhapDanhMuc(ListNode *&first)
{
	while (true)
	{
		LNData *data = new LNData;
		cout << "Nhap ma sv: ";
		cin >> data->maSach;
		if (data->maSach <= 0)
			return;
		fflush(stdin);
		cout << "Nhap ho: ";
		cin >> data->trangThai;
		cout << "Nhap ten: ";
		cin >> data->viTri;
		addLast(first, data);
	}
}
