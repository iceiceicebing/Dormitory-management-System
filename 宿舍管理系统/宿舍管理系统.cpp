// 宿舍管理系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "dorm_system.h"
#include "menu.h"

// 1. 定义在头文件中声明了的全局变量

StudentList all_students = NULL;
DormList all_dorms;
ManagerList all_managers = NULL;
TeacherList all_teachers = NULL;
AnnounceList announcement_board = NULL;
InspectionList all_inspections = NULL;

int main()
{
	printf("数据加载中，请稍候\n");
// 2. 文件载入

	// (1) 初始化

	InitStudentList(all_students);
	InitDormList(all_dorms);
	InitManagerList(all_managers);
	InitTeacherList(all_teachers);
	InitAnnounceList(announcement_board);
	InitInspectionList(all_inspections);

	// (2) 数据载入

	LoadStudentList(all_students);
	LoadDormList(all_dorms);
	LoadManagerList(all_managers);
	LoadTeacherList(all_teachers);
	LoadAnnounceList(announcement_board);
	LoadInspectionList(all_inspections);

	printf("数据加载完毕！\n\n");

// 3. 进行登录

	for (;;) {
		switch (menu_select()) {
		case 1: {
			printf("****************************\n");
			printf("*     学生登录           *\n");
			printf("****************************\n");
			char ID[13], password_real[50];
			StudentNode* student = NULL; // 学生结点的指针，便于学生端对该学生直接操作
			for (;;) {
				printf("请输入您的学号：\n");
				scanf("%s", ID);
				if (FindStudent(all_students, ID, password_real, student) == 1) {
					printf("学号输入正确\n\n");
					break;
				}
				else if (strcmp(ID, "#") == 0) break; // 学号栏输入了#，退出外层for循环
				else {
					printf("学生不存在，请重新输入。或者输入#退出\n");
					continue; // 继续外层for循环
				}
			}
			if (strcmp(ID, "#") == 0) break; // 学号栏输入了#，用户退出学生登录	
			if (password_verify(password_real) == QUIT) break;
			else
				StudentSystem(student); // 登录成功，前往学生系统
			break;
		} // case1

		case 2: {
			printf("****************************\n");
			printf("*      宿管登录            *\n");
			printf("****************************\n");
			char ID[13], password_real[50];
			ManagerNode* manager = NULL;
			for (;;) {
				printf("请输入您的工号：\n");
				scanf("%s", ID);
				if (FindManager(all_managers, ID, password_real, manager) == 1) {
					printf("工号输入正确\n\n");
					break;
				}
				else if (strcmp(ID, "#") == 0) break; // 工号栏输入了#，退出外层for循环
				else {
					printf("工号不存在，请重新输入。或者输入#退出\n");
					continue; // 继续外层for循环
				}
			}
			if (strcmp(ID, "#") == 0) break; // 工号栏输入了#，用户退出宿管登录	
			if (password_verify(password_real) == QUIT) break;
			else
				ManagerSystem(manager); // 登录成功，前往宿管系统
			break;
		} // case2

		case 3: {
			printf("****************************\n");
			printf("*      辅导员登录            *\n");
			printf("****************************\n");
			char ID[13], password_real[50];
			TeacherNode* teacher = NULL;
			for (;;) {
				printf("请输入您的工号：\n");
				scanf("%s", ID);
				if (FindTeacher(all_teachers, ID, password_real, teacher) == 1) {
					printf("工号输入正确\n\n");
					break;
				}
				else if (strcmp(ID, "#") == 0) break; // 工号栏输入了#，退出外层for循环
				else {
					printf("工号不存在，请重新输入。或者输入#退出\n");
					continue; // 继续外层for循环
				}
			}
			if (strcmp(ID, "#") == 0) break; // 工号栏输入了#，用户退出辅导员登录	
			if (password_verify(password_real) == QUIT) break;
			else
				TeacherSystem(all_teachers,teacher); // 登录成功，前往辅导员系统
			break;
		} // case3

		case 4: {
			printf("****************************\n");
			printf("*       学生会登录         *\n");
			printf("****************************\n");
			char ID[13], password_real[50];
			StudentNode* union_member = NULL;
			for (;;) {
				printf("请输入您的学号：\n");
				scanf("%s", ID);
				Status temp = FindUnionMember(all_students, ID, password_real, union_member);
				if (temp == OK) {
					printf("学号输入正确\n\n");
					break;
				}
				else if (temp == FALSE) {
					printf("学生不是学生会成员，请重新输入。或者输入#退出\n");
					continue;
				}
				else if (strcmp(ID, "#") == 0) break; // 学号栏输入了#，退出外层for循环
				else {
					printf("学生不存在，请重新输入。或者输入#退出\n");
					continue; // 继续外层for循环
				}
			}
			if (strcmp(ID, "#") == 0) break; // 学号栏输入了#，用户退出学生登录	
			if (password_verify(password_real) == QUIT) break;
			else
				StudentUnionSystem(union_member); // 登录成功，前往学生系统
			break;
		} // case4

		case 0:
			printf("\t 退  出 ！\n");
			SaveStudentList(all_students);
			SaveDormInfo(all_dorms);
			SaveManagerList(all_managers);
			SaveTeacherList(all_teachers);
			SaveAnnounceList(announcement_board);
			SaveInspectionList(all_inspections);
			return 0;
		} // switch
	} // for
	return 0;
}


