// student_union.cpp：学生会任务端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

// 按楼栋存储人数
typedef struct
{
	char name[20];
	int count;
} BuildingStat;
BuildingStat stats[50];

Status InitInspectionList(InspectionList& L) {
	L = (InspectionList)malloc(sizeof(InspectionNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadInspectionList(InspectionList& L) {
	FILE* fp = fopen("data/InspectionList.txt", "r");
	if (!fp) return ERROR;
	InspectionList rear = L;
	InspectionInfo temp;
	// 格式：时间 楼栋 房间号 10个姓名 count score is_finished
	while (fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %d %f %d\n",
		temp.time, temp.dorm_building, temp.room_number,
		temp.all_inspectors.inspectors[0],
		temp.all_inspectors.inspectors[1],
		temp.all_inspectors.inspectors[2],
		temp.all_inspectors.inspectors[3],
		temp.all_inspectors.inspectors[4],
		temp.all_inspectors.inspectors[5],
		temp.all_inspectors.inspectors[6],
		temp.all_inspectors.inspectors[7],
		temp.all_inspectors.inspectors[8],
		temp.all_inspectors.inspectors[9],
		&temp.all_inspectors.count,
		&temp.score, &temp.is_finished) == 16) {  // 正确检查返回值
		InspectionNode* s = (InspectionNode*)malloc(sizeof(InspectionNode));

		s->data = temp;
		s->next = NULL;
		rear->next = s;
		rear = s;
	}
	fclose(fp);
	return OK;
}

Status InspectionInsert(InspectionList& L, int i, InspectionInfo e)
{
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

Status SaveInspectionList(InspectionList& L)
{
	// 保存学生会任务表
	FILE* fp = fopen("data/InspectionList.txt", "w");
	if (!fp)
		return ERROR;
	InspectionList s = L->next; // 初始化指针，指向头结点下一个结点（第一个存有数据的结点）；后续利用s进行输出
	while (s != NULL)
	{
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


// 卫生打分与录入函数
Status ScoreDorm(StudentNode* union_member)
{
	char building[20], room[10];// 输入的宿舍楼和房间号
	float score;// 输入的分数
	int inspectorCount;//检查者数
	InspectorList inspectors;//检查者数组

	printf("\n===== 宿舍卫生打分 =====\n");
	printf("请输入宿舍楼(x园x栋）：");
	scanf("%s", building);
	printf("请输入房间号（xxx）：");
	scanf("%s", room);
	printf("请输入卫生分数（0-100）：");
	scanf("%f", &score);
	if (score < 0 || score > 100)
	{
		printf("分数无效，请输入0-100之间的数值。\n");
		return ERROR;
	}

	// 查找宿舍
	DormInfo* dorm = NULL;
	for (int i = 0; i < all_dorms.count; ++i)
	{
		if (strcmp(all_dorms.data[i].dorm_building, building) == 0 && strcmp(all_dorms.data[i].room_number, room) == 0)
		{
			dorm = &all_dorms.data[i];
			break;
		}
	}
	if (!dorm)
	{
		printf("宿舍 %s %s 不存在！\n", building, room);
		return ERROR;
	}

	// 录入检查人员信息
	printf("请输入检查人数（最多10人，最少1人）：");
	scanf("%d", &inspectorCount);
	if (inspectorCount < 1 || inspectorCount > 10)
	{
		printf("输入检查人数错误\n");
		return ERROR;
	}

	inspectors.count = inspectorCount;
	printf("请输入%d位检查人员姓名（每行一个）：\n", inspectorCount);
	for (int i = 0; i < inspectorCount; ++i)
	{
		scanf("%s", inspectors.inspectors[i]);
	}
	for (int i = inspectors.count; i < 10; ++i)
	{
		strcpy(inspectors.inspectors[i], "无"); // 多余位置填空字符串
	}

	// 更新宿舍的历史分数
	ScoreList* sl = &dorm->all_scores;
	int maxCount = (sl->count > 9) ? 9 : sl->count;   // 最多保留10条
	for (int i = maxCount; i > 0; --i)
	{
		sl->scores[i] = sl->scores[i - 1];
	}
	sl->scores[0] = score;
	sl->count++;

	// 计算平均分
	float avg = 0.0f;
	if (sl->count > 0)
	{
		float sum = 0.0f;
		for (int i = 0; i < sl->count; ++i)
		{
			sum += sl->scores[i];
		}
		avg = sum / sl->count;
	}
	else {
		avg = score;   // 如果没有历史分数，平均分就是当前分数
	}
	printf("宿舍 %s %s 分数已更新，当前平均分：%.2f\n", building, room, avg);

	// 生成检查任务记录（插入到 all_inspections 链表头部）
	InspectionInfo newTask;
	// 获取当前日期（格式：YYYYMMDD）
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	sprintf(newTask.time, "%04d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
	strcpy(newTask.dorm_building, building);
	strcpy(newTask.room_number, room);
	newTask.all_inspectors = inspectors;
	newTask.score = score;
	newTask.is_finished = 1;   // 任务已完成

	InspectionNode* newNode = (InspectionNode*)malloc(sizeof(InspectionNode));
	if (!newNode)
	{
		printf("内存分配失败，检查任务未保存。\n");
		return ERROR;
	}
	newNode->data = newTask;
	newNode->next = all_inspections->next;
	all_inspections->next = newNode;

	printf("检查任务已记录。\n");

	// 保存宿舍表到 data/DormList.txt
	if (SaveDormInfo(all_dorms) != OK)
	{
		printf("警告：保存宿舍分数失败！\n");
		return ERROR;
	}
	else {
		printf("宿舍分数已保存。\n");
	}

	// 保存检查任务表到 data/InspectionList.txt
	if (SaveInspectionList(all_inspections) != OK)
	{
		printf("警告：保存检查任务失败！\n");
		return ERROR;
	}
	else {
		printf("检查任务已保存。\n");
	}
	return OK;
}

// 比较函数
static int CompareDormByAvg(const void* a, const void* b)
{
	DormInfo* da = *(DormInfo**)a;
	DormInfo* db = *(DormInfo**)b;
	float sumA = 0, sumB = 0;
	for (int i = 0; i < da->all_scores.count; i++) sumA += da->all_scores.scores[i];
	for (int i = 0; i < db->all_scores.count; i++) sumB += db->all_scores.scores[i];
	float avgA = (da->all_scores.count == 0) ? 0 : sumA / da->all_scores.count;
	float avgB = (db->all_scores.count == 0) ? 0 : sumB / db->all_scores.count;
	if (avgA > avgB) return -1;
	if (avgA < avgB) return 1;
	return 0;
}

//宿舍排名
Status ShowDormRanking(StudentNode* union_member)
{
	if (all_dorms.count == 0)
	{
		printf("暂无宿舍数据\n");
		return ERROR;
	}

	// 收集有效宿舍
	DormInfo** valid = (DormInfo**)malloc(all_dorms.count * sizeof(DormInfo*));
	int validCount = 0;
	for (int i = 0; i < all_dorms.count; i++)
	{
		if (all_dorms.data[i].all_scores.count > 0)
		{
			valid[validCount++] = &all_dorms.data[i];
		}
	}

	if (validCount == 0)
	{
		printf("暂无任何宿舍有卫生打分记录\n");
		free(valid);
		return ERROR;
	}

	// 排序
	qsort(valid, validCount, sizeof(DormInfo*), CompareDormByAvg);

	printf("\n===== 宿舍卫生排名=====\n");
	printf("排名\t楼栋\t房间号\t平均成绩\t次数\n");
	for (int i = 0; i < validCount; i++)
	{
		DormInfo* d = valid[i];
		float sum = 0;
		for (int j = 0; j < d->all_scores.count; j++) sum += d->all_scores.scores[j];
		float avg = sum / d->all_scores.count;
		printf("%d\t%s\t%s\t%.2f\t%5d\n", i + 1, d->dorm_building, d->room_number, avg, d->all_scores.count);
	}
	free(valid);
	return OK;
}

// 按楼栋统计学生人数
Status StatByBuilding()
{
	if (all_students == NULL || all_students->next == NULL)
	{
		printf("暂无学生数据。\n");
		return ERROR;
	}

	int statCount = 0;

	StudentNode* p = all_students->next;
	while (p != NULL)
	{
		char* building = p->data.dorm_building;
		// 查找是否已统计过该楼栋
		int found = 0;
		for (int i = 0; i < statCount; ++i)
		{
			if (strcmp(stats[i].name, building) == 0)
			{
				stats[i].count++;
				found = 1;
				break;
			}
		}
		// 如果是新楼栋，且未超过数组容量
		if (!found && statCount < 50)
		{
			strcpy(stats[statCount].name, building);
			stats[statCount].count = 1;
			statCount++;
		}
		p = p->next;
	}
	// 输出统计结果
	printf("\n===== 各楼栋学生人数统计 =====\n");
	printf("楼栋名称\t\t学生人数\n");
	for (int i = 0; i < statCount; ++i)
		printf("%-16s\t%d\n", stats[i].name, stats[i].count);
	return OK;
}
// 获取当前日期字符串（格式：YYYYMMDD）
void GetCurrentDate(char* dateStr)
{
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	sprintf(dateStr, "%04d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

// 按时间统计宿舍人数（当前在寝情况）
Status StatByTime()
{
	if (all_students == NULL || all_students->next == NULL)
	{
		printf("暂无学生数据。\n");
		return ERROR;
	}

	int total = 0;
	int inDorm = 0;
	StudentNode* p = all_students->next;
	while (p != NULL)
	{
		total++;
		if (p->data.in_dorm == 1)
		{
			inDorm++;
		}
		p = p->next;
	}

	char today[9];
	GetCurrentDate(today);
	printf("\n===== 当前在寝情况统计 =====\n");
	printf("统计时间：%s\n", today);
	printf("学生总数：%d\n", total);
	printf("在寝人数：%d\n", inDorm);
	printf("离寝人数：%d\n", total - inDorm);
	return OK;
}


// 导出统计报表
Status ExportReport()
{
	FILE* report = fopen("data/stat_report.txt", "w");
	if (!report)
	{
		printf("无法创建报表文件，请检查 data 目录是否存在。\n");
		return ERROR;
	}

	char currentDate[9];
	GetCurrentDate(currentDate);

	// 写入报表头
	fprintf(report, "===== 宿舍管理系统统计报表 =====\n");
	fprintf(report, "生成时间：%s\n\n", currentDate);

	// ================= 1. 宿舍卫生排名 =================
	fprintf(report, "--- 宿舍卫生排名（按平均分） ---\n");
	if (all_dorms.count == 0)
	{
		fprintf(report, "暂无宿舍数据。\n");
	}
	else {
		// 收集有打分的宿舍
		DormInfo** valid = (DormInfo**)malloc(all_dorms.count * sizeof(DormInfo*));
		if (!valid) {
			fprintf(report, "内存分配失败，无法生成排名。\n");
			return ERROR;
		}
		else {
			int validCnt = 0;
			for (int i = 0; i < all_dorms.count; ++i)
			{
				if (all_dorms.data[i].all_scores.count > 0)
				{
					valid[validCnt++] = &all_dorms.data[i];
				}
			}
			if (validCnt == 0)
			{
				fprintf(report, "暂无宿舍有卫生打分记录。\n");
			}
			else
			{
				qsort(valid, validCnt, sizeof(DormInfo*), CompareDormByAvg);
				fprintf(report, "排名\t楼栋\t\t房间号\t平均分\t打分次数\n");
				for (int i = 0; i < validCnt; ++i)
				{
					DormInfo* d = valid[i];
					float sum = 0;
					for (int j = 0; j < d->all_scores.count; ++j) sum += d->all_scores.scores[j];
					float avg = sum / d->all_scores.count;
					fprintf(report, "%d\t%-10s\t%-6s\t%.2f\t%d\n",
						i + 1, d->dorm_building, d->room_number, avg, d->all_scores.count);
				}
			}
			free(valid);
		}
	}

	// ================= 2. 按楼栋统计学生人数 =================
	fprintf(report, "\n--- 各楼栋学生人数统计 ---\n");
	if (!all_students || !all_students->next)
	{
		fprintf(report, "暂无学生数据。\n");
	}
	else
	{
		typedef struct { char name[20]; int count; } BuildingStat;
		BuildingStat stats[50];
		int statCnt = 0;

		StudentNode* p = all_students->next;
		while (p)
		{
			char* building = p->data.dorm_building;
			int found = 0;
			for (int i = 0; i < statCnt; ++i)
			{
				if (strcmp(stats[i].name, building) == 0)
				{
					stats[i].count++;
					found = 1;
					break;
				}
			}
			if (!found && statCnt < 50)
			{
				strcpy(stats[statCnt].name, building);
				stats[statCnt].count = 1;
				statCnt++;
			}
			p = p->next;
		}
		fprintf(report, "楼栋名称\t\t学生人数\n");
		for (int i = 0; i < statCnt; ++i)
		{
			fprintf(report, "%-16s\t%d\n", stats[i].name, stats[i].count);
		}
	}

	// ================= 3. 当前在寝情况统计 =================
	fprintf(report, "\n--- 当前在寝情况 ---\n");
	if (!all_students || !all_students->next)
	{
		fprintf(report, "暂无学生数据。\n");
	}
	else {
		int total = 0, inDorm = 0;
		StudentNode* p = all_students->next;
		while (p) {
			total++;
			if (p->data.in_dorm == 1) inDorm++;
			p = p->next;
		}
		fprintf(report, "统计时间：%s\n", currentDate);
		fprintf(report, "学生总数：%d\n", total);
		fprintf(report, "在寝人数：%d\n", inDorm);
		fprintf(report, "离寝人数：%d\n", total - inDorm);
	}

	fclose(report);
	printf("统计报表已导出到 data/stat_report.txt\n");
}

// 学生会总菜单函数
void StudentUnionSystem(StudentNode* union_member)
{

	if (!union_member)
	{
		printf("错误：未获取到学生会成员信息！\n");
		return;
	}

	int choice;
	do {

		printf("\n========== 学生会任务管理 ==========\n");
		printf("欢迎您，%s（学号：%s，职务：%s）\n",
			union_member->data.name,
			union_member->data.ID,
			union_member->data.in_student_union == 2 ? "会长" : "干事");
		printf("----------------------------------------\n");
		printf("  1. 宿舍卫生打分与录入\n");
		printf("  2. 查看宿舍卫生排名（按平均分）\n");
		printf("  3. 按楼栋统计学生人数\n");
		printf("  4. 按时间统计宿舍在寝情况\n");
		printf("  5. 导出统计报表到文本文件\n");
		printf("  0. 退出学生会端\n");
		printf("----------------------------------------\n");
		printf("请选择操作（0-5）：");

		scanf("%d", &choice);
		getchar();  // 吸收回车符

		switch (choice) {
		case 1:
			ScoreDorm(union_member);      // 卫生打分与录入
			break;
		case 2:
			ShowDormRanking(union_member); // 查看宿舍排名
			break;
		case 3:
			StatByBuilding();              // 按楼栋统计人数
			break;
		case 4:
			StatByTime();                  // 按时间统计在寝情况
			break;
		case 5:
			ExportReport();                // 导出报表
			break;
		case 0:
			printf("正在退出学生会系统，感谢使用！\n");
			break;
		default:
			printf("无效输入，请重新选择（0-5）！\n");
			break;
		}

		if (choice != 0)
		{
			printf("\n按回车键继续...");
			getchar();
		}
	} while (choice != 0);
}