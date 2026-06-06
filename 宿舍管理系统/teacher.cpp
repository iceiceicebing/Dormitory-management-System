<<<<<<< Updated upstream:宿舍管理系统/teacher.cpp
﻿// teacher.cpp：导员端操作函数的定义

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
=======
﻿#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#ifndef TEACHER_CLASS_MAX
#define TEACHER_CLASS_MAX 10
#endif

// =======================
// 辅助函数
// =======================

static void ClearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static void StripNewline(char* s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

static void SafeReadString(const char* prompt, char* buf, int size) {
    if (!buf || size <= 0) return;
    printf("%s", prompt);
    fgets(buf, size, stdin);
    StripNewline(buf);
}

// 检查某个班级是否属于该导员负责范围
static int TeacherHasClass(const TeacherInfo& t, const char* class_name) {
    if (!class_name || !*class_name) return 0;
    for (int i = 0; i < t.classes.count && i < TEACHER_CLASS_MAX; ++i) {
        if (strcmp(t.classes.class_in_charge[i], class_name) == 0) {
            return 1;
        }
    }
    return 0;
}

static void PrintTeacherInfo(const TeacherInfo& t) {
    printf("\n========== 导员信息 ==========\n");
    printf("姓名：%s\n", t.name);
    printf("工号：%s\n", t.ID);
    printf("学院：%s\n", t.school_name);
    printf("联系方式：%s\n", t.contact_number);
    printf("是否组长：%d\n", t.is_leader);
    printf("负责班级数：%d\n", t.classes.count);
    printf("负责班级：");
    for (int i = 0; i < t.classes.count && i < TEACHER_CLASS_MAX; ++i) {
        printf("%s ", t.classes.class_in_charge[i]);
    }
    printf("\n=============================\n");
}

static void PrintTeacherClasses(const TeacherInfo& t) {
    printf("\n========== 负责班级 ==========\n");
    if (t.classes.count <= 0) {
        printf("暂无负责班级。\n");
    } else {
        for (int i = 0; i < t.classes.count && i < TEACHER_CLASS_MAX; ++i) {
            printf("%d. %s\n", i + 1, t.classes.class_in_charge[i]);
        }
    }
    printf("=============================\n");
}

static Status ChangeTeacherPassword(TeacherNode* teacher) {
    if (!teacher) return ERROR;

    char oldPwd[50] = { 0 };
    char newPwd1[50] = { 0 };
    char newPwd2[50] = { 0 };

    SafeReadString("请输入旧密码：", oldPwd, 50);
    if (strcmp(oldPwd, teacher->data.password) != 0) {
        printf("旧密码错误，修改失败。\n");
        return ERROR;
    }

    SafeReadString("请输入新密码：", newPwd1, 50);
    SafeReadString("请再次输入新密码：", newPwd2, 50);

    if (strcmp(newPwd1, newPwd2) != 0) {
        printf("两次输入的新密码不一致，修改失败。\n");
        return ERROR;
    }

    strcpy(teacher->data.password, newPwd1);
    printf("密码修改成功。\n");
    return OK;
}

// =======================
// 导员链表基础操作
// =======================

Status InitTeacherList(TeacherList& L) {
    L = (TeacherList)malloc(sizeof(TeacherNode));
    if (!L) return ERROR;
    L->next = NULL;
    return OK;
}

void DestroyTeacherList(TeacherList& L) {
    if (!L) return;
    TeacherNode* p = L;
    while (p) {
        TeacherNode* q = p->next;
        free(p);
        p = q;
    }
    L = NULL;
}

Status LoadTeacherList(TeacherList& L) {
    if (!L) return ERROR;

    FILE* fp = fopen("data/TeacherList.txt", "r");
    if (!fp) return ERROR;

    TeacherNode* rear = L;
    char line[1024];

    while (fgets(line, sizeof(line), fp)) {
        StripNewline(line);
        if (line[0] == '\0') continue;

        TeacherInfo temp;
        memset(&temp, 0, sizeof(temp));

        int ret = sscanf(line,
            "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %d",
            temp.name,
            temp.ID,
            temp.school_name,
            temp.password,
            temp.classes.class_in_charge[0],
            temp.classes.class_in_charge[1],
            temp.classes.class_in_charge[2],
            temp.classes.class_in_charge[3],
            temp.classes.class_in_charge[4],
            temp.classes.class_in_charge[5],
            temp.classes.class_in_charge[6],
            temp.classes.class_in_charge[7],
            temp.classes.class_in_charge[8],
            temp.classes.class_in_charge[9],
            &temp.classes.count,
            temp.contact_number,
            &temp.is_leader
        );

        if (ret != 17) {
            fclose(fp);
            return ERROR;
        }

        TeacherNode* s = (TeacherNode*)malloc(sizeof(TeacherNode));
        if (!s) {
            fclose(fp);
            return ERROR;
        }
        s->data = temp;
        s->next = NULL;

        rear->next = s;
        rear = s;
    }

    fclose(fp);
    return OK;
}

Status SaveTeacherList(TeacherList& L) {
    if (!L) return ERROR;

    FILE* fp = fopen("data/TeacherList.txt", "w");
    if (!fp) return ERROR;

    TeacherNode* s = L->next;
    while (s != NULL) {
        fprintf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s %d\n",
            s->data.name,
            s->data.ID,
            s->data.school_name,
            s->data.password,
            s->data.classes.class_in_charge[0],
            s->data.classes.class_in_charge[1],
            s->data.classes.class_in_charge[2],
            s->data.classes.class_in_charge[3],
            s->data.classes.class_in_charge[4],
            s->data.classes.class_in_charge[5],
            s->data.classes.class_in_charge[6],
            s->data.classes.class_in_charge[7],
            s->data.classes.class_in_charge[8],
            s->data.classes.class_in_charge[9],
            s->data.classes.count,
            s->data.contact_number,
            s->data.is_leader);
        s = s->next;
    }

    fclose(fp);
    return OK;
}

Status TeacherInsert(TeacherList& L, int i, TeacherInfo e) {
    if (!L || i < 1) return ERROR;

    TeacherNode* p = L;
    for (int k = 1; k < i; ++k) {
        if (p->next == NULL) return ERROR;
        p = p->next;
    }

    TeacherNode* s = (TeacherNode*)malloc(sizeof(TeacherNode));
    if (!s) return ERROR;

    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status TeacherDelete(TeacherList& L, const char ID[13]) {
    if (!L) return ERROR;

    TeacherNode* p = L;
    TeacherNode* q = L->next;

    while (q) {
        if (strcmp(q->data.ID, ID) == 0) {
            p->next = q->next;
            free(q);
            return OK;
        }
        p = q;
        q = q->next;
    }
    return ERROR;
}

Status FindTeacher(TeacherList L, char ID[13], char password[50], TeacherNode*& teacher) {
    if (!L) return ERROR;

    TeacherNode* p = L->next;
    while (p) {
        if (strcmp(p->data.ID, ID) == 0) {
            strcpy(password, p->data.password);
            teacher = p;
            return OK;
        }
        p = p->next;
    }
    teacher = NULL;
    return ERROR;
}

Status FindTeacherByID(TeacherList L, const char ID[13], TeacherNode*& teacher) {
    if (!L) return ERROR;

    TeacherNode* p = L->next;
    while (p) {
        if (strcmp(p->data.ID, ID) == 0) {
            teacher = p;
            return OK;
        }
        p = p->next;
    }
    teacher = NULL;
    return ERROR;
}

// =======================
// 下面是导员端功能
// 说明：以下四个功能先用“通用文本文件方式”实现
// =======================

static void TeacherViewStudentsByClass(TeacherNode* teacher) {
    if (!teacher) return;

    char class_name[50] = { 0 };
    PrintTeacherClasses(teacher->data);
    SafeReadString("请输入要查看的班级名称：", class_name, 50);

    if (!TeacherHasClass(teacher->data, class_name)) {
        printf("该班级不在你负责范围内。\n");
        return;
    }

    FILE* fp = fopen("data/StudentList.txt", "r");
    if (!fp) {
        printf("无法打开 StudentList.txt\n");
        return;
    }

    char line[2048];
    int found = 0;

    printf("\n========== %s 班学生信息 ==========\n", class_name);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, class_name) != NULL) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) {
        printf("未找到该班级的学生信息，或文件格式与当前筛选方式不匹配。\n");
    }
    printf("===================================\n");

    fclose(fp);
}

static void TeacherViewOrHandleRepair(TeacherNode* teacher) {
    (void)teacher;

    FILE* fp = fopen("data/RepairList.txt", "r");
    if (!fp) {
        printf("无法打开 RepairList.txt\n");
        return;
    }

    // 第一遍：读取所有行，动态存储到堆内存中
    char** lines = NULL;
    int cnt = 0;
    char buffer[2048];

    while (fgets(buffer, sizeof(buffer), fp)) {
        // 去掉末尾换行符（不影响存储）
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';

        // 动态扩展数组
        char** new_lines = (char**)realloc(lines, (cnt + 1) * sizeof(char*));
        if (!new_lines) {
            // 内存分配失败，清理已分配的内存
            for (int i = 0; i < cnt; ++i) free(lines[i]);
            free(lines);
            fclose(fp);
            printf("内存不足，无法处理报修记录。\n");
            return;
        }
        lines = new_lines;

        // 为当前行分配内存并复制内容
        lines[cnt] = (char*)malloc(strlen(buffer) + 1);
        if (!lines[cnt]) {
            for (int i = 0; i < cnt; ++i) free(lines[i]);
            free(lines);
            fclose(fp);
            printf("内存不足，无法处理报修记录。\n");
            return;
        }
        strcpy(lines[cnt], buffer);
        cnt++;
    }
    fclose(fp);

    if (cnt == 0) {
        printf("当前没有报修记录。\n");
        free(lines);
        return;
    }

    // 显示所有记录（保留原始行内容，不含换行符，显示时补上）
    printf("\n========== 报修记录 ==========\n");
    for (int i = 0; i < cnt; ++i) {
        printf("[%d] %s\n", i + 1, lines[i]);
    }
    printf("==============================\n");

    printf("是否要将某条报修标记为已处理？(1=是, 0=否)：");
    int op = 0;
    if (scanf("%d", &op) != 1) {
        ClearInputBuffer();
        // 释放内存
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }
    ClearInputBuffer();

    if (op != 1) {
        // 释放内存后返回
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }

    int idx = 0;
    printf("请输入要处理的记录编号：");
    if (scanf("%d", &idx) != 1) {
        ClearInputBuffer();
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }
    ClearInputBuffer();

    if (idx < 1 || idx > cnt) {
        printf("编号无效。\n");
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }

    // 修改选中的行：在内容前加上 "[已处理]"
    char new_line[2200];
    snprintf(new_line, sizeof(new_line), "[已处理] %s", lines[idx - 1]);
    free(lines[idx - 1]);                     // 释放原字符串
    lines[idx - 1] = (char*)malloc(strlen(new_line) + 1);
    if (!lines[idx - 1]) {
        printf("内存不足，无法修改记录。\n");
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }
    strcpy(lines[idx - 1], new_line);

    // 写回原文件（全部重写）
    fp = fopen("data/RepairList.txt", "w");
    if (!fp) {
        printf("写回 RepairList.txt 失败。\n");
        for (int i = 0; i < cnt; ++i) free(lines[i]);
        free(lines);
        return;
    }
    for (int i = 0; i < cnt; ++i) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);

    printf("处理完成。\n");

    // 释放动态内存
    for (int i = 0; i < cnt; ++i) free(lines[i]);
    free(lines);
}

static void TeacherViewInspectionResults(TeacherNode* teacher) {
    if (!teacher) return;

    char class_name[50] = { 0 };
    PrintTeacherClasses(teacher->data);
    SafeReadString("请输入要查看的班级名称：", class_name, 50);

    if (!TeacherHasClass(teacher->data, class_name)) {
        printf("该班级不在你负责范围内。\n");
        return;
    }

    FILE* fp = fopen("data/InspectionList.txt", "r");
    if (!fp) {
        printf("无法打开 InspectionList.txt\n");
        return;
    }

    char line[2048];
    int found = 0;

    printf("\n========== %s 班卫生检查结果 ==========\n", class_name);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, class_name) != NULL) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) {
        printf("未找到该班级的卫生检查信息，或文件格式与当前筛选方式不匹配。\n");
    }
    printf("=======================================\n");

    fclose(fp);
}

static void TeacherPublishAnnouncement(TeacherNode* teacher) {
    if (!teacher) return;

    char title[100] = { 0 };
    char content[1000] = { 0 };

    SafeReadString("请输入公告标题：", title, 100);
    SafeReadString("请输入公告内容：", content, 1000);

    FILE* fp = fopen("data/AnnounceList.txt", "a");
    if (!fp) {
        printf("无法打开 AnnounceList.txt\n");
        return;
    }

    time_t now = time(NULL);
    struct tm* lt = localtime(&now);

    fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d %s %s %s %s\n",
        lt->tm_year + 1900,
        lt->tm_mon + 1,
        lt->tm_mday,
        lt->tm_hour,
        lt->tm_min,
        lt->tm_sec,
        teacher->data.name,
        teacher->data.ID,
        title,
        content);

    fclose(fp);
    printf("公告已写入文件。\n");
}

// =======================
// 导员端系统主菜单
// =======================

void TeacherSystem(TeacherList& L, TeacherNode* teacher) {
    if (!teacher) return;

    int choice = -1;
    while (true) {
        printf("\n========== 导员端 ==========\n");
        printf("1. 查看个人信息\n");
        printf("2. 修改密码\n");
        printf("3. 查看负责班级\n");
        printf("4. 查看所负责班级的学生信息\n");
        printf("5. 查看/处理报修\n");
        printf("6. 查看卫生检查结果\n");
        printf("7. 发布公告\n");
        printf("0. 保存导员信息并退出\n");
        printf("============================\n");
        printf("\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1) {
            ClearInputBuffer();
            printf("输入无效，请重新输入。\n");
            continue;
        }
        ClearInputBuffer();

        switch (choice) {
        case 1:
            PrintTeacherInfo(teacher->data);
            break;

        case 2:
            if (ChangeTeacherPassword(teacher) == OK) {
                if (SaveTeacherList(L) != OK) {
                    printf("密码已改，但保存失败。\n");
                }
            }
            break;

        case 3:
            PrintTeacherClasses(teacher->data);
            break;

        case 4:
            TeacherViewStudentsByClass(teacher);
            break;

        case 5:
            TeacherViewOrHandleRepair(teacher);
            break;

        case 6:
            TeacherViewInspectionResults(teacher);
            break;

        case 7:
            TeacherPublishAnnouncement(teacher);
            break;

        case 0:
            if (SaveTeacherList(L) != OK) {
                printf("导员信息保存失败。\n");
            } else {
                printf("导员信息保存成功，退出导员端。\n");
            }
            return;

        default:
            printf("无效选项，请重新输入。\n");
            break;
        }
    }
>>>>>>> Stashed changes:宿舍管理系统/宿舍管理系统/teacher.cpp
}