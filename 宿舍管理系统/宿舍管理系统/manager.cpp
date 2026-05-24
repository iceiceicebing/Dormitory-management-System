// manager.cpp：宿管端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitManagerList(ManagerList& L) {
	L = (ManagerList)malloc(sizeof(ManagerNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadManagerList(ManagerList& L) {
	// 宿管表结构体没有存储宿管总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	FILE* fp = fopen("data/ManagerList.txt", "r");
	if (!fp)
		return ERROR;
	ManagerList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立学生表
	ManagerInfo temp;
	while (fscanf(fp, "%s %s %s %s %d %s",
		temp.name, temp.ID, temp.password, temp.building_on_duty, 
		&temp.duty_period, temp.contact_number) != EOF){
		// 以下为循环体内的具体内容
		ManagerList s = (ManagerNode*)malloc(sizeof(ManagerNode)); // 生成新尾结点
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行宿管信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
	}
	fclose(fp);
	return OK;
}

Status ManagerInsert(ManagerList& L, int i, ManagerInfo e) {
	// 从头开始，找到第i个位置，进行学生信息结点的插入
	int k = 0;
	ManagerNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q) {
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i) {
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (ManagerNode*)malloc(sizeof(ManagerNode)); // 生成待插入结点
			s->data = e;
			s->next = p; // 修改链接
			q->next = s;
			return OK;
		}
		else {
			// 如果还没到，则把指针后移，k在下一个循环自增1
			q = p;
			p = p->next;
		}
	}
	return ERROR; // i非法（i过大，循环已经到了指针尾部了）
}

Status SaveManagerList(ManagerList& L) {
	// 保存学生表
	FILE* fp = fopen("data/ManagerList.txt", "w");
	if (!fp)
		return ERROR;
	ManagerList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL) {
		fprintf(fp, "%s %s %s %s %d %s\n",
			s->data.name, s->data.ID, s->data.password, s->data.building_on_duty,
			s->data.duty_period, s->data.contact_number);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

Status FindManager(ManagerList L, char ID[13], char password[50], ManagerNode*& manager) {
	// 元素查找，找到匹配的ID则返回密码
	ManagerNode* p = L->next;
	while (p) {
		if (strcmp(p->data.ID, ID) == 0) {
			// 若字符相等，则把当前结点的密码部分写入字符数组password
			strcpy(password, L->data.password);
			manager = p;
			return OK;
		}
		p = p->next;
	}
	return ERROR; // 宿管不存在
}

// 剩下的为宿管端具体的操作函数……

void ManagerSystem(ManagerNode* manager) {
	// ……该函数为宿管端总操作函数，从这里可以调用任一个宿管端操作函数
	// 待开发
}