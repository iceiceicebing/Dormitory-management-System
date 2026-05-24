// student_union.cpp：学生会任务端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitInspectionList(InspectionList& L) {
	L = (InspectionList)malloc(sizeof(InspectionNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadInspectionList(InspectionList& L) {
	// 学生会任务表结构体没有存储学生会任务总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	FILE* fp = fopen("data/InspectionList.txt", "r");
	if (!fp)
		return ERROR;
	InspectionList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立学生会任务表
	InspectionInfo temp;
	while (fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %d %f %d",
		temp.time, temp.dorm_building, temp.room_number, temp.all_inspectors.inspectors[0], 
		temp.all_inspectors.inspectors[1], temp.all_inspectors.inspectors[2], 
		temp.all_inspectors.inspectors[3], temp.all_inspectors.inspectors[4],
		temp.all_inspectors.inspectors[5], temp.all_inspectors.inspectors[6],
		temp.all_inspectors.inspectors[7], temp.all_inspectors.inspectors[8],
		temp.all_inspectors.inspectors[9], &temp.all_inspectors.count,
		&temp.score, &temp.is_finished) != EOF) {
		// 以下为循环体内的具体内容
		InspectionList s = (InspectionNode*)malloc(sizeof(InspectionNode)); // 生成新尾结点
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行学生会任务信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
	}
	fclose(fp);
	return OK;
}

Status InspectionInsert(InspectionList& L, int i, InspectionInfo e) {
	// 从头开始，找到第i个位置，进行学生会任务信息结点的插入
	int k = 0;
	InspectionNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q) {
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i) {
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (InspectionNode*)malloc(sizeof(InspectionNode)); // 生成待插入结点
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

Status SaveInspectionList(InspectionList& L) {
	// 保存学生会任务表
	FILE* fp = fopen("data/InspectionList.txt", "w");
	if (!fp)
		return ERROR;
	InspectionList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL) {
		fprintf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %d %f %d\n",
			s->data.time, s->data.dorm_building, s->data.room_number, s->data.all_inspectors.inspectors[0],
			s->data.all_inspectors.inspectors[1], s->data.all_inspectors.inspectors[2],
			s->data.all_inspectors.inspectors[3], s->data.all_inspectors.inspectors[4],
			s->data.all_inspectors.inspectors[5], s->data.all_inspectors.inspectors[6],
			s->data.all_inspectors.inspectors[7], s->data.all_inspectors.inspectors[8],
			s->data.all_inspectors.inspectors[9], s->data.all_inspectors.count,
			s->data.score, s->data.is_finished);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

Status FindUnionMember(StudentList L, char ID[13], char password[50], StudentNode*& union_member) {
	// 元素查找，找到匹配的ID则返回密码
	StudentNode* p = L->next;
	while (p) {
		if (strcmp(p->data.ID, ID) == 0 && p->data.in_student_union != 0) {
			// 若ID匹配，且该学生是学生会成员（干事或会长），则把当前结点的密码部分写入字符数组password
			strcpy(password, p->data.password);
			union_member = p;
			return OK;
		}
		else if (strcmp(p->data.ID, ID) == 0 && p->data.in_student_union == 0) 
			return FALSE; // 查找到了学生，但不是学生会成员
		p = p->next;
	}
	return ERROR; // 学生不存在
}

// 剩下的为学生会任务端具体的操作函数……

void StudentUnionSystem(StudentNode* union_member) {
	// ……该函数为学生会端总操作函数，从这里可以调用任一个学生会端操作函数
	// 待开发
}