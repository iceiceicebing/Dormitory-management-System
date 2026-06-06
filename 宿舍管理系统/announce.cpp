<<<<<<< Updated upstream:宿舍管理系统/announce.cpp
﻿// announce.cpp：信息公告端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitAnnounceList(AnnounceList& L) {
	L = (AnnounceList)malloc(sizeof(AnnounceNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadAnnounceList(AnnounceList& L) {
	// 信息公告表结构体没有存储信息公告总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	FILE* fp = fopen("data/AnnounceList.txt", "r");
	if (!fp)
		return ERROR;
	AnnounceList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立信息公告表
	AnnounceInfo temp;
	while (fscanf(fp, "%d %s %s %s %[^\n]", // [^\n]是为了防止信息公告的内容中有空格，导致读取提前结束
		&temp.type, temp.time, temp.heading, temp.sender, temp.content) != EOF){
		// 以下为循环体内的具体内容
		AnnounceList s = (AnnounceNode*)malloc(sizeof(AnnounceNode)); // 生成新尾结点
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行信息公告信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
	}
	fclose(fp);
	return OK;
}

Status AnnounceInsert(AnnounceList& L, int i, AnnounceInfo e) {
	// 从头开始，找到第i个位置，进行信息公告信息结点的插入
	int k = 0;
	AnnounceNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q) {
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i) {
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (AnnounceNode*)malloc(sizeof(AnnounceNode)); // 生成待插入结点
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

Status SaveAnnounceList(AnnounceList& L) {
	// 保存信息公告表
	FILE* fp = fopen("data/AnnounceList.txt", "w");
	if (!fp)
		return ERROR;
	AnnounceList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL) {
		fprintf(fp, "%d %s %s %s %s\n",
			s->data.type, s->data.time, s->data.heading, s->data.sender, s->data.content);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

=======
﻿// announce.cpp：信息公告端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitAnnounceList(AnnounceList& L) {
	L = (AnnounceList)malloc(sizeof(AnnounceNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadAnnounceList(AnnounceList& L) {
    FILE* fp = fopen("data/AnnounceList.txt", "r");
    if (!fp) return ERROR;

    AnnounceList rear = L; // 尾指针，指向头结点
    char line[1024];       // 用于存储每行内容

    while (fgets(line, sizeof(line), fp)) {
        // 去除末尾的换行符
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        // 跳过空行
        if (line[0] == '\0') continue;

        AnnounceInfo temp;
        // 格式：type time heading sender content
        // 注意：content 可能包含空格，所以最后一个字段使用 %[^\n] 读取该行剩余全部内容
        int matched = sscanf(line, "%d %s %s %s %[^\n]",
                             &temp.type, temp.time, temp.heading, temp.sender, temp.content);
        if (matched != 5) {
            // 解析失败，跳过该行（可根据需要增加错误提示）
            continue;
        }

        // 创建新结点
        AnnounceList s = (AnnounceNode*)malloc(sizeof(AnnounceNode));
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

Status AnnounceInsert(AnnounceList& L, int i, AnnounceInfo e) {
	// 从头开始，找到第i个位置，进行信息公告信息结点的插入
	int k = 0;
	AnnounceNode* p = L->next, * q = L, * s;
	if (i < 1) // i非法（过小，i至少为1）
		return ERROR;
	while (q) {
		k++; // k最开始为1，表示到了第一个和第二个结点之间
		if (k == i) {
			// 如果到了第i个位置，则把新元素插入在q与p之间
			s = (AnnounceNode*)malloc(sizeof(AnnounceNode)); // 生成待插入结点
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

Status SaveAnnounceList(AnnounceList& L) {
	// 保存信息公告表
	FILE* fp = fopen("data/AnnounceList.txt", "w");
	if (!fp)
		return ERROR;
	AnnounceList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL) {
		fprintf(fp, "%d %s %s %s %s\n",
			s->data.type, s->data.time, s->data.heading, s->data.sender, s->data.content);
		s = s->next; // s到下一个结点
	}
	fclose(fp);
	return OK;
}

>>>>>>> Stashed changes:宿舍管理系统/宿舍管理系统/announce.cpp
// 剩下的为信息公告端具体的操作函数……