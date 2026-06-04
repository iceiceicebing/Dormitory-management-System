// teacher.cpp：导员端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitTeacherList(TeacherList& L) {
	L = (TeacherList)malloc(sizeof(TeacherNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadTeacherList(TeacherList& L) {
	// 导员表结构体没有存储导员总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	FILE* fp = fopen("data/TeacherList.txt", "r");
	if (!fp)
		return ERROR;
	TeacherList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立导员表
	TeacherInfo temp;
	while (fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %d",
		temp.name, temp.ID, temp.school_name, temp.password, temp.classes.class_in_charge[0], 
		temp.classes.class_in_charge[1], temp.classes.class_in_charge[2], temp.classes.class_in_charge[3], 
		temp.classes.class_in_charge[4], temp.classes.class_in_charge[5], temp.classes.class_in_charge[6], 
		temp.classes.class_in_charge[7], temp.classes.class_in_charge[8], temp.classes.class_in_charge[9], 
		&temp.classes.count, temp.contact_number, &temp.is_leader) != EOF)
		// 以上为while循环结束的条件，在判断同时会把一行数据写入temp，
		// 如果这一行没有数据（即fp到了文件末尾），则循环结束
	{
		// 以下为循环体内的具体内容
		TeacherList s = (TeacherNode*)malloc(sizeof(TeacherNode)); // 生成新尾结点
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行导员信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
	}
	fclose(fp);
	return OK;
}

Status TeacherInsert(TeacherList& L, int i, TeacherInfo e) {
	// 从头开始，找到第i个位置，进行导员信息结点的插入
	int k = 0;
	TeacherNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q) {
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i) {
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (TeacherNode*)malloc(sizeof(TeacherNode)); // 生成待插入结点
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

Status SaveTeacherList(TeacherList& L) {
	// 保存导员表
	FILE* fp = fopen("data/TeacherList.txt", "w");
	if (!fp)
		return ERROR;
	TeacherList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL) {
		fprintf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %d\n",
			s->data.name, s->data.ID, s->data.school_name, s->data.password, 
			s->data.classes.class_in_charge[0], s->data.classes.class_in_charge[1], 
			s->data.classes.class_in_charge[2], s->data.classes.class_in_charge[3],
			s->data.classes.class_in_charge[4], s->data.classes.class_in_charge[5], 
			s->data.classes.class_in_charge[6], s->data.classes.class_in_charge[7], 
			s->data.classes.class_in_charge[8], s->data.classes.class_in_charge[9],
			s->data.classes.count, s->data.contact_number, s->data.is_leader);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

Status FindTeacher(TeacherList L, char ID[13], char password[50], TeacherNode*& teacher) {
	// 元素查找，找到匹配的ID则返回密码
	TeacherNode* p = L->next;
	while (p) {
		if (strcmp(p->data.ID, ID) == 0) {
			// 若字符相等，则把当前结点的密码部分写入字符数组password
			strcpy(password, p->data.password);
			teacher = p;
			return OK;
		}
		p = p->next;
	}
	return ERROR; // 辅导员不存在
}

// 剩下的为辅导员端具体的操作函数……

void TeacherSystem(TeacherNode* teacher) {
	// ……该函数为辅导员端总操作函数，从这里可以调用任一个辅导员端操作函数
	// 待开发
}