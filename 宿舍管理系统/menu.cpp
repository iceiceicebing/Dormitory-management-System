// menu.cpp：主界面相关函数的定义（选择用户端函数、密码校验函数）

#define _CRT_SECURE_NO_WARNINGS 1
#include "menu.h"

/***********************/
/* 菜单选择函数     */
/***********************/
int menu_select()
{
	int sele;
	printf("\n         宿舍管理系统\n");
	printf("===================================\n");
	printf("      1. 学生登录\n");
	printf("      2. 宿管登录\n");
	printf("      3. 辅导员登录\n");
	printf("      4. 学生会登录\n");
	printf("      0. 退出系统\n");
	printf("===================================\n");
	printf("      请选择 0--4：\n");
	for (;;)
	{
		scanf("%d", &sele);
		if (sele < 0 || sele > 4) {
			printf("\n\t输入错误，重选0--4：\n");
			while (getchar() != '\n'); // 把用户乱敲的字母全部清除
		}
		else
			break;
	}
	return sele;
}

Status password_verify(char password_real[]) {
	char password_input[50];

	for (;;) {
		printf("请输入密码：\n");
		scanf("%s", password_input);
		if (strcmp(password_input, password_real) == 0) {
			printf("欢迎使用\n\n");
			return TRUE;
		}
		else if (strcmp(password_input, "#") == 0) return QUIT; // 学号栏输入了#，退出
		else {
			printf("密码错误，请重新输入。或者输入#退出\n");
			continue; // 继续for循环
		}
	} // for
}