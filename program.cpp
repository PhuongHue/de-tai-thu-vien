#include <iostream>
#include <fstream>
#include <cstring>
#include "DauSach.cpp"
#include "DanhMuc.cpp"
#include "TheDocGia.cpp"

using namespace std;

void timTheoMSSV(ListNode *list)
{
	int mssv;
	cout << "MSSV: ";
	cin >> mssv;
	fflush(stdin);
	LNData key;
	key.maSach = mssv;
	PairNode ketQua = findBySinhVien(list, key);
	if (ketQua.value == NULL)
	{
		cout << "Khong tim thay.";
		return;
	}
	printSinhVien(ketQua.value->data);
}

void xoaTheoMSSV(ListNode *&first)
{
	int mssv;
	cout << "Nhap ma sinh vien can xoa: ";
	cin >> mssv;
	fflush(stdin);
	LNData key;
	key.maSach = mssv;
	cout << deleteBySinhVien(first, key);
}

void sortByMSSV(ListNode *&first)
{
	for (ListNode *i = first; i != NULL && i->next != NULL; i = i->next)
	{
		for (ListNode *j = i->next; j != NULL; j = j->next)
		{
			if (i->data->maSach > j->data->maSach)
				swapNodeData(i, j);
		}
	}
}

int compareDataDanhMuc(LNData *a, LNData *b)
{
	int cmpTrangThai = a->trangThai - b->trangThai;
	if (cmpTrangThai != 0)
		return cmpTrangThai;
	else
	{
		return a->viTri.compare(a->viTri);
	}
}

void sortByTenHo(ListNode *&first)
{
	for (ListNode *i = first; i != NULL && i->next != NULL; i = i->next)
	{
		for (ListNode *j = i->next; j != NULL; j = j->next)
		{
			if (compareDataDanhMuc(i->data, j->data) > 0)
				swapNodeData(i, j);
		}
	}
}

int main()
{
	ListNode *dssv = NULL;
	int menu_select = 0;
	while (menu_select != -1)
	{
		cout << "0. Thoat.\n"
				 << "1. Nhap sinh vien.\n"
				 << "2. Xem danh sach.\n"
				 << "3. Tong so sinh vien.\n"
				 << "4. Tim sinh vien theo MSSV.\n"
				 << "5. Xoa theo MSSV.\n"
				 << "6. Luu file.\n"
				 << "7. Doc file.\n"
				 << "8. Sap xep tang dan theo MSSV.\n"
				 << "9. Sap xep tang dan theo Ten Ho.\n"
				 << "Lua chon: ";
		int luaChon;
		cin >> luaChon;
		fflush(stdin);
		system("cls");
		switch (luaChon)
		{
		case 0:
		{
			menu_select = -1;
			break;
		}
		case 1:
		{
			nhapDanhMuc(dssv);
			break;
		}
		case 2:
		{
			printList(dssv);
			getchar();
			break;
		}
		case 3:
		{
			cout << "Tong so sinh vien: " << countAll(dssv);
			getchar();
			break;
		}
		case 4:
		{
			timTheoMSSV(dssv);
			getchar();
			break;
		}
		case 5:
		{
			printList(dssv);
			xoaTheoMSSV(dssv);
			getchar();
			printList(dssv);
			getchar();
			break;
		}
		case 6:
		{
			luuFile(dssv);
			getchar();
			break;
		}
		case 7:
		{
			docFile(dssv);
			cout << "Doc file thanh cong.\n";
			printList(dssv);
			getchar();
			break;
		}
		case 8:
		{
			sortByMSSV(dssv);
			cout << "Sort thanh cong.\n";
			printList(dssv);
			getchar();
			break;
		}
		case 9:
		{
			sortByTenHo(dssv);
			cout << "Sort thanh cong.\n";
			printList(dssv);
			getchar();
			break;
		}
		}
		system("cls");
	}
}
