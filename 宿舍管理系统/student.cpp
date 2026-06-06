// student.cpp：学生端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitStudentList(StudentList& L){	
	L = (StudentList)malloc(sizeof(StudentNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadStudentList(StudentList& L) {
	// 学生表结构体没有存储学生总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	FILE* fp = fopen("data/StudentList.txt", "r");
	if (!fp) 
		return ERROR;
	StudentList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立学生表
	StudentInfo temp;
	while (fscanf(fp, 
		"%s %s %s %s %s %s %s %d %s %s %d %s %d %s %s %s %d %s %s %s %s %d %s %s %s %s %d %s\n",
		temp.name, temp.ID,temp.gender, temp.school_name, temp.class_name, temp.dorm_building, temp.room_number, 
		&temp.bed_number, temp.contact_number, temp.guardian_number, &temp.in_dorm, temp.password, 
		&temp.in_student_union, 
		temp.on_leave.reason, temp.on_leave.start_date, temp.on_leave.end_date, 
		&temp.on_leave.approved, temp.on_leave.text, 
		temp.dorm_alter.reason, temp.dorm_alter.start_date, temp.dorm_alter.end_date, 
		&temp.dorm_alter.approved, temp.dorm_alter.text, 
		temp.dorm_exit.reason, temp.dorm_exit.start_date, temp.dorm_exit.end_date,
		&temp.dorm_exit.approved, temp.dorm_exit.text) != EOF) 
		// 以上为while循环结束的条件，在判断同时会把一行数据写入temp，
		// 如果这一行没有数据（即fp到了文件末尾），则循环结束
	{
		// 以下为循环体内的具体内容
		StudentList s = (StudentNode*)malloc(sizeof(StudentNode)); // 生成新尾结点
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行学生信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
	}
	fclose(fp);
	return OK;
}

Status StudentInsert(StudentList& L, int i, StudentInfo e){	
	// 从头开始，找到第i个位置，进行学生信息结点的插入
	int k = 0;
	StudentNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q){
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i){   
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (StudentNode*)malloc(sizeof(StudentNode)); // 生成待插入结点
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

Status SaveStudentList(StudentList& L) {
	// 保存学生表
	FILE* fp = fopen("data/StudentList.txt", "w");
	if (!fp)
		return ERROR;
	StudentList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL){
		fprintf(fp,
		"%s %s %s %s %s %s %s %d %s %s %d %s %d %s %s %s %d %s %s %s %s %d %s %s %s %s %d %s\n",
		s->data.name, s->data.ID, s->data.gender,s->data.school_name, s->data.class_name, s->data.dorm_building, s->data.room_number,
		s->data.bed_number, s->data.contact_number, s->data.guardian_number, s->data.in_dorm, s->data.password,
		s->data.in_student_union,
		s->data.on_leave.reason, s->data.on_leave.start_date, s->data.on_leave.end_date,
		s->data.on_leave.approved, s->data.on_leave.text,
		s->data.dorm_alter.reason, s->data.dorm_alter.start_date, s->data.dorm_alter.end_date,
		s->data.dorm_alter.approved, s->data.dorm_alter.text,
		s->data.dorm_exit.reason, s->data.dorm_exit.start_date, s->data.dorm_exit.end_date,
		s->data.dorm_exit.approved, s->data.dorm_exit.text);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

Status FindStudent(StudentList L, char ID[13], char password[50], StudentNode*& student) {
	// 元素查找，找到匹配的ID则返回密码
	StudentNode* p = L->next;
	while (p){
		if (strcmp(p->data.ID, ID) == 0) {
			// 若字符相等，则把当前结点的密码部分写入字符数组password
			strcpy(password, p->data.password);
			student = p;
			return OK;
		}
		p = p->next;
	}
	return ERROR; // 学生不存在
}

// 剩下的为学生端具体的操作函数……

void StudentSystem(StudentNode* student) {
	// ……该函数为学生端总操作函数，从这里可以调用任一个学生端操作函数
	// 待开发
}