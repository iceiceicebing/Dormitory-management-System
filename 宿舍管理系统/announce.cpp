// announce.cpp：信息公告端操作函数的定义

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

//发布公告函数
// 在 announced.cpp 中修改后的通用发布函数
void HandlePublishAnnouncement(const char* sender_name) { // <-- 参数改为直接接收名字字符串
    AnnounceNode* newAnnounce = (AnnounceNode*)malloc(sizeof(AnnounceNode));
    if (newAnnounce == NULL) {
        printf("内存分配失败，无法发布公告。\n");
        return;
    }
    // 内存全置为0，防止乱码
    memset(newAnnounce, 0, sizeof(AnnounceNode));
    newAnnounce->next = NULL;

    printf("\n===== 发布新公告 =====\n");
    printf("公告类型（0-重要通知，1-寻物启事，2-其他）：");
    if (scanf("%d", &newAnnounce->data.type) != 1) {
        printf("类型输入错误！\n");
        free(newAnnounce);
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("请输入标题：");
    fgets(newAnnounce->data.heading, sizeof(newAnnounce->data.heading), stdin);
    newAnnounce->data.heading[strcspn(newAnnounce->data.heading, "\n")] = '\0';

    printf("请输入内容（支持多行，以单独一行'END'结束）：\n");
    char line[500] = { 0 };
    char content[1024] = { 0 }; 
    while (1) {
        fgets(line, sizeof(line), stdin);
        if (strcmp(line, "END\n") == 0 || strcmp(line, "END") == 0) {
            break;
        }
        strncat(content, line, sizeof(content) - strlen(content) - 1);
    }
    strcpy(newAnnounce->data.content, content);

    // 【核心修改】直接拷贝传入的名字，不关心它是宿管还是导员
    strcpy(newAnnounce->data.sender, sender_name); 
    
    printf("请输入发布日期（字符串，格式如20260603）：");
    fgets(newAnnounce->data.time, sizeof(newAnnounce->data.time), stdin);
    newAnnounce->data.time[strcspn(newAnnounce->data.time, "\n")] = '\0';

    // 头插法加入链表
    newAnnounce->next = announcement_board->next;
    announcement_board->next = newAnnounce;
    SaveAnnounceList(announcement_board);
    printf("公告发布成功\n");
}



// 查看所有公告
void HandleViewAnnouncements() {
    if (announcement_board == NULL || announcement_board->next == NULL) {
        printf("\n暂无任何公告。\n");
        return;
    }
    printf("\n===== 全部公告 =====\n");
    AnnounceNode* p = announcement_board->next;
    int idx = 1;
    while (p != NULL) {
        printf("【公告 %d】\n", idx++);
        
        // 把类型数字转换成汉字，体验更好
        const char* type_str = "其他";
        if (p->data.type == 0) type_str = "重要通知";
        else if (p->data.type == 1) type_str = "寻物启事";

        printf("类型：%s   发布人：%s   日期：%s\n", type_str, p->data.sender, p->data.time);
        printf("标题：%s\n", p->data.heading);
        printf("内容：\n%s\n", p->data.content);
        printf("----------------------\n");
        p = p->next;
    }
}