// menu.h：存放主界面menu_select函数的声明

#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include <stdio.h>
#include <string.h>

// 预定义常量
#define   TRUE        1
#define   QUIT        -2

typedef   int       Status; // 函数返回类型定义

int menu_select();    //菜单选择函数
Status password_verify(char password_real[]); // 登录密码校验函数