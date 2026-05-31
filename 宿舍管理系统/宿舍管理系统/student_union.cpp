// student_union.cpp：学生会任务端操作函数的定义

#include "dorm_system.h"

Status InitInspectionList(InspectionList& L) 
{//建立检查者信息链表
	L = (InspectionList)malloc(sizeof(InspectionNode));
	if (!L) return ERROR;
	L->next = NULL;
	return OK;
}

Status LoadInspectionList(InspectionList& L) 
{// 学生会任务表结构体没有存储学生会任务总数。如有需要，可以在该函数的while循环体内增加一个计数的变量。
	if (!L) {
		if (InitInspectionList(L) != OK)
			return ERROR;
	} else {
		InspectionNode* p = L->next;
		while (p) {
			InspectionNode* q = p->next;
			free(p);
			p = q;
		}
		L->next = NULL;
	}

	FILE* fp = fopen("data/InspectionList.txt", "r");
	if (!fp)
		return OK; // 文件不存在时当作空列表处理

	InspectionList rear = L; // 初始化尾指针，指向头结点；后续采用尾插法建立学生会任务表
	InspectionInfo temp;
	int count = 0; // 统计加载的检查任务数量
	while (1) {
		int ret = fscanf(fp, "%8s %19s %9s %49s %49s %49s %49s %49s %49s %49s %49s %49s %d %f %d",
			temp.time, temp.dorm_building, temp.room_number,
			temp.all_inspectors.inspectors[0], temp.all_inspectors.inspectors[1],
			temp.all_inspectors.inspectors[2], temp.all_inspectors.inspectors[3],
			temp.all_inspectors.inspectors[4], temp.all_inspectors.inspectors[5],
			temp.all_inspectors.inspectors[6], temp.all_inspectors.inspectors[7],
			temp.all_inspectors.inspectors[8], temp.all_inspectors.inspectors[9],
			&temp.all_inspectors.count, &temp.score, &temp.is_finished);
		if (ret == EOF)
			break;
		if (ret != 16) {
			fclose(fp);
			return ERROR;
		}

		InspectionList s = (InspectionNode*)malloc(sizeof(InspectionNode)); // 生成新尾结点
		if (!s) {
			fclose(fp);
			return ERROR;
		}
		s->data = temp;
		s->next = NULL;

		rear->next = s; // 采用带头结点的链表进行学生会任务信息的插入，第一个结点（即L指向的头结点）不存数据
		rear = s; // 尾指针后移
		count++;
	}
	fclose(fp);
	printf("已加载 %d 条检查任务\n", count);
	return OK;
}

Status InspectionInsert(InspectionList& L, int i, InspectionInfo e) 
{// 从头开始，找到第i个位置，进行学生会任务信息结点的插入
	if (i < 1 || !L) // i非法（过小，i至少为1）
		return ERROR;

	InspectionNode* q = L;
	int k = 0;
	while (q && k < i - 1) 
	{
		q = q->next;
		k++;
	}
	if (!q)
		return ERROR;

	InspectionNode* s = (InspectionNode*)malloc(sizeof(InspectionNode)); // 生成待插入结点
	if (!s)
		return ERROR;
	s->data = e;
	s->next = q->next;
	q->next = s;
	return OK;
}

Status SaveInspectionList(InspectionList& L)
 {// 保存学生会任务表
	if (!L)
		return ERROR;
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

int CompareInspectionScore(const void* a, const void* b)
 {// 比较函数
	InspectionNode* const* p1 = (InspectionNode* const*)a;
	InspectionNode* const* p2 = (InspectionNode* const*)b;
	if ((*p2)->data.score > (*p1)->data.score)
		return 1;
	if ((*p2)->data.score < (*p1)->data.score)
		return -1;
	return strcmp((*p1)->data.time, (*p2)->data.time);
}

static void ShowInspectionRanking(InspectionList L)
 {// 按照分数从高到低展示检查任务排名，分数相同则按照日期先后排序
	if (!L || !L->next) {
		printf("暂无检查任务\n");
		return;
	}
	int count = 0;
	InspectionNode* p = L->next;
	while (p) { count++; p = p->next; }
	InspectionNode** arr = (InspectionNode**)malloc(sizeof(InspectionNode*) * count);
	if (!arr) return;
	p = L->next;
	for (int i = 0; i < count; i++) {
		arr[i] = p;
		p = p->next;
	}
	qsort(arr, count, sizeof(InspectionNode*), CompareInspectionScore);
	printf("评分排名：\n");
	for (int i = 0; i < count; i++)
	{
		printf("%d. 日期：%s | 楼栋：%s | 房间：%s | 分数：%.1f | 状态：%s\n",i + 1,arr[i]->data.time,arr[i]->data.dorm_building,arr[i]->data.room_number,arr[i]->data.score);
		if (arr[i]->data.is_finished)
			printf("| 状态: 已打分\n");
		else
			printf("| 状态: 未打分\n");
	}
	free(arr);
}

static int DateInRange(const char* date, const char* start, const char* end) 
{//判断日期是否合法
	if (!date || !start || !end || start[0] == '\0' || end[0] == '\0')
		return 0;
	return strcmp(date, start) >= 0 && strcmp(date, end) <= 0;
}

static int IsStudentInDormOnDate(StudentInfo* s, const char* date) 
{// 判断学生在某日期是否在寝
	if (!s || !date) return 0;
	if (s->on_leave.approved == 1 && DateInRange(date, s->on_leave.start_date, s->on_leave.end_date))
		return 0;
	return s->in_dorm == 1;
}

static void CollectDormCountByBuilding(StudentList L, char buildings[100][20], int counts[100], int* building_count)
 {
	if (!L || !L->next) {
		*building_count = 0;
		return;
	}
	*building_count = 0;
	StudentNode* p = L->next;
	while (p) {
		if (p->data.in_dorm == 1) {
			int idx = -1;
			for (int i = 0; i < *building_count; i++) {
				if (strcmp(buildings[i], p->data.dorm_building) == 0) {
					idx = i;
					break;
				}
			}
			if (idx == -1) {
				idx = (*building_count)++;
				strcpy(buildings[idx], p->data.dorm_building);
			}
			counts[idx]++;
		}
		p = p->next;
	}
}

static void ShowDormCountByBuilding(StudentList L)
 {
	if (!L || !L->next) {
		printf("暂无学生数据，无法统计宿舍人数\n");
		return;
	}
	char buildings[100][20];
	int counts[100] = {0};
	int building_count = 0;
	CollectDormCountByBuilding(L, buildings, counts, &building_count);
	if (building_count == 0) {
		printf("当前无学生在寝。\n");
		return;
	}
	printf("按楼栋统计当前在寝宿舍人数：\n");
	for (int i = 0; i < building_count; i++) {
		printf("楼栋：%s | 在寝人数：%d\n", buildings[i], counts[i]);
	}
}

void ShowDormCountByDate(StudentList L) 
{// 按日期统计在寝人数
	if (!L || !L->next) 
	{
		printf("暂无学生数据，无法统计宿舍人数\n");
		return;
	}
	printf("请输入要统计的日期（格式：YYYYMMDD）：");
	char date[9];
	scanf(" %8s", date);
	if (strlen(date) != 8) 
	{
		printf("日期格式错误，请输入YYYYMMDD。\n");
		return;
	}
	char buildings[100][20];// 按照索引存储楼栋名称
	int counts[100] = {0};// 存储每栋楼人数
	int building_count = 0;
	StudentNode* p = L->next;
	while (p) {
		if (IsStudentInDormOnDate(&p->data, date)) 
		{
			int idx = -1;
			for (int i = 0; i < building_count; i++) 
			{
				if (strcmp(buildings[i], p->data.dorm_building) == 0)
				 {
					idx = i;
					break;
				}
			}
			if (idx == -1) // 学生所在楼栋未统计过，添加新楼栋
			{
				idx = building_count++;
				strcpy(buildings[idx], p->data.dorm_building);
			}
			counts[idx]++;
		}
		p = p->next;
	}
	printf("%s 按楼栋在寝人数统计：\n", date);
	if (building_count == 0) {
		printf("该日期无在寝学生。\n");
		return;
	}
	for (int i = 0; i < building_count; i++)
	 {
		printf("楼栋：%s | 在寝人数：%d\n", buildings[i], counts[i]);
	}
}

static Status ExportStatisticsReport(InspectionList inspections, StudentList students, const char* filename)
 {// 导出统计报表到文本文件
	FILE* fp = fopen(filename, "w");
	if (!fp) return ERROR;
	fprintf(fp, "学生会检查与宿舍统计报表\n");
	fprintf(fp, "==============================\n");
	int total = 0, finished = 0, unfinished = 0;
	InspectionNode* p = inspections ? inspections->next : NULL;
	while (p)
	 {
		total++;
		if (p->data.is_finished) finished++;
		else unfinished++;
		p = p->next;
	}
	fprintf(fp, "检查任务总数：%d\n", total);
	fprintf(fp, "已完成：%d\n", finished);
	fprintf(fp, "未完成：%d\n", unfinished);
	fprintf(fp, "\n检查任务列表：\n");
	p = inspections ? inspections->next : NULL;
	while (p) 
	{
		fprintf(fp, "日期：%s | 楼栋：%s | 房间：%s | 分数：%.1f", p->data.time, p->data.dorm_building, p->data.room_number, p->data.score);
	if (p->data.is_finished)
		fprintf(fp, "| 状态：已打分\n ");
	else
		fprintf(fp, "| 状态：未打分\n");
		p = p->next;
	}
	fprintf(fp, "\n当前在寝学生按楼栋统计：\n");
	char buildings[100][20];
	int counts[100] = {0};
	int building_count = 0;
	CollectDormCountByBuilding(students, buildings, counts, &building_count);
	if (building_count == 0) {
		fprintf(fp, "当前无学生在寝。\n");
	} else {
		for (int i = 0; i < building_count; i++) {
			fprintf(fp, "楼栋：%s | 在寝人数：%d\n", buildings[i], counts[i]);
		}
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

Status CreateInspection(InspectionList& L, InspectionInfo e) 
{// 在学生会任务表末尾插入新的学生会任务
	InspectionList p = L;
	while (p->next) p = p->next;

	InspectionList s = (InspectionNode*)malloc(sizeof(InspectionNode));
	s->data = e;
	s->next = NULL;
	p->next = s;
	return OK;
}

Status FindInspection(InspectionList L, char building[], char room[], InspectionNode*& task) 
{// 根据楼栋、房间号查找检查任务
	InspectionNode* p = L->next;
	while (p) {
		if (strcmp(p->data.dorm_building, building) == 0 &&
			strcmp(p->data.room_number, room) == 0)
		{
			task = p;
			return OK;
		}
		p = p->next;
	}
	return ERROR;
}

Status ModifyInspectionScore(InspectionList& L, char building[], char room[], float new_score) 
{// 修改评分（打分）
	InspectionNode* p = NULL;
	if (FindInspection(L, building, room, p) != OK)
		return ERROR;

	p->data.score = new_score;
	p->data.is_finished = 1;
	return OK;
}

Status DeleteInspection(InspectionList& L, char building[], char room[])
 {// 删除一条检查任务
	InspectionNode *p = L->next, *q = L;
	while (p) {
		if (strcmp(p->data.dorm_building, building) == 0 &&
			strcmp(p->data.room_number, room) == 0)
		{
			q->next = p->next;
			free(p);
			return OK;
		}
		q = p;
		p = p->next;
	}
	return ERROR;
}

void ShowAllInspections(InspectionList L)
 {// 展示所有检查任务
	InspectionNode* p = NULL;
	if (L)
		p = L->next;
	if (!p) 
	{
		printf("暂无检查任务\n");
		return;
	}
	while (p)
	 {
		printf("日期：%s | 楼栋：%s | 房间：%s | 分数：%.1f ", p->data.time, p->data.dorm_building, p->data.room_number, p->data.score);
		if (p->data.is_finished)
			printf("| 状态: 已打分\n");
		else
			printf("| 状态: 未打分\n");
		p = p->next;
	}
}

// 按条件搜索检查任务（date/building/room 可传空字符串表示不筛选该项）
static Status SearchInspections(InspectionList L, const char* date, const char* building, const char* room)
{
	if (!L || !L->next) {
		printf("暂无检查任务\n");
		return ERROR;
	}
	InspectionNode* p = L->next;
	int found = 0;
	while (p) {
		int match_date = (date == NULL || date[0] == '\0' || strcmp(p->data.time, date) == 0);
		int match_building = (building == NULL || building[0] == '\0' || strcmp(p->data.dorm_building, building) == 0);
		int match_room = (room == NULL || room[0] == '\0' || strcmp(p->data.room_number, room) == 0);

		if (match_date && match_building && match_room) {
			printf("日期：%s | 楼栋：%s | 房间：%s | 分数：%.1f | 检查者：",
				p->data.time, p->data.dorm_building, p->data.room_number, p->data.score);
			for (int i = 0; i < p->data.all_inspectors.count; i++) {
				if (i) printf(", ");
				printf("%s", p->data.all_inspectors.inspectors[i]);
			}
		if(p->data.is_finished)
			printf(" | 状态：已打分\n");
		else
			printf(" | 状态：未打分\n");
			found++;
		}
		p = p->next;
	}
	if (found == 0) {
		printf("未找到匹配的检查任务。\n");
		return ERROR;
	}
	return OK;
}

// 统计并显示每位检查者参与次数（降序）
static Status ShowInspectorCounts(InspectionList L)
{
	if (!L || !L->next) {
		printf("暂无检查任务，无法统计检查者\n");
		return ERROR;
	}
	// 简单上限，按现状最多不会超过200名不同检查者
	char names[200][50];
	int counts[200] = { 0 };
	int n = 0;

	InspectionNode* p = L->next;
	while (p) {
		for (int i = 0; i < p->data.all_inspectors.count; i++) {
			char* name = p->data.all_inspectors.inspectors[i];
			if (name[0] == '\0') continue;
			int idx = -1;
			for (int j = 0; j < n; j++) {
				if (strcmp(names[j], name) == 0) {
					idx = j;
					break;
				}
			}
			if (idx == -1) {
				if (n < 200) {
					strcpy(names[n], name);
					counts[n] = 1;
					n++;
				}
			} else {
				counts[idx]++;
			}
		}
		p = p->next;
	}
	if (n == 0) {
		printf("暂无检查者数据\n");
		return ERROR;
	}
	// 简单排序（选择或冒泡都可，保持代码风格）
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (counts[j] > counts[i]) {
				int tmpc = counts[i]; counts[i] = counts[j]; counts[j] = tmpc;
				char tmpn[50]; strcpy(tmpn, names[i]); strcpy(names[i], names[j]); strcpy(names[j], tmpn);
			}
		}
	}
	printf("检查者贡献次数统计：\n");
	for (int i = 0; i < n; i++) {
		printf("%d. %s | 次数：%d\n", i + 1, names[i], counts[i]);
	}
	return OK;
}

// 按日期范围导出检查任务到文本文件（包含检查者列表），日期格式要求 YYYYMMDD，字符串比较可直接用 strcmp
static Status ExportInspectionsByDate(InspectionList L, const char* startDate, const char* endDate, const char* outPath)
{
	if (!L) return ERROR;
	FILE* fp = fopen(outPath, "w");
	if (!fp) return ERROR;
	fprintf(fp, "日期,楼栋,房间,分数,状态,检查者\n");
	InspectionNode* p = L->next;
	while (p) {
		int in_range = 1;
		if (startDate && startDate[0] != '\0' && strcmp(p->data.time, startDate) < 0) in_range = 0;
		if (endDate && endDate[0] != '\0' && strcmp(p->data.time, endDate) > 0) in_range = 0;
		if (in_range) {
			fprintf(fp, "%s,%s,%s,%.1f,%s,\"", p->data.time, p->data.dorm_building, p->data.room_number, p->data.score);
			if (p->data.is_finished)
				fprintf(fp, " | 状态：已打分\n");
			else
				fprintf(fp," | 状态：未打分\n");
			for (int i = 0; i < p->data.all_inspectors.count; i++) {
				if (i) fprintf(fp, ";");
				fprintf(fp, "%s", p->data.all_inspectors.inspectors[i]);
			}
			fprintf(fp, "\"\n");
		}
		p = p->next;
	}
	fclose(fp);
	return OK;
}

void StudentUnionSystem(StudentNode* union_member) // 学生会主界面
{
	int choice;
	float score;
	char building[20], room[10];
	InspectionInfo task;
	InspectionList L = NULL;

	InitInspectionList(L); // 初始化学生会任务表
	LoadInspectionList(L);

	while (1) {
		printf("\n=====学生会评分====\n");
		printf("当前登录：%s\n", union_member->data.name);
		printf("1.查看所有检查任务\n");
		printf("2.新增检查任务\n");
		printf("3.给宿舍打分\n");
		printf("4.删除检查·1任务\n");
		printf("5.保存并退出学生会评分\n");
		printf("6.查看评分排名\n");
		printf("7.按照楼栋查看当前在寝人数\n");
		printf("8.按照日期查看当前在寝人数\n");
		printf("9.导出统计报表\n");
		printf("10.搜索检查任务\n");
		printf("11.检查者贡献统计\n");
		printf("12.按照日期区间导出检查任务\n");
		printf("请选择:");
		if (scanf("%d", &choice) != 1)
		{
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF);
			printf("输入无效，请输入数字。\n");
			continue;
		}

		switch (choice)
		{
		case 1:
			ShowAllInspections(L);
			break;
		case 2:
			printf("请输入要统计的日期（格式：YYYYMMDD）：");
			scanf(" %8s", task.time);
			printf("请输入楼栋：");
			scanf(" %19s", task.dorm_building);
			printf("输入房间号：");
			scanf(" %9s", task.room_number);
			printf("请输入检查人数（小于等于10人）");
			scanf("%d", &task.all_inspectors.count);
			if (task.all_inspectors.count < 0)
				task.all_inspectors.count = 0;
			if (task.all_inspectors.count > 10)
				task.all_inspectors.count = 10;
			for (int i = 0; i < task.all_inspectors.count; i++)
			{
				printf("请输入检查者名字：", i + 1);
				scanf(" %49s", task.all_inspectors.inspectors[i]);
			}
			for (int i = task.all_inspectors.count; i < 10; i++)
				task.all_inspectors.inspectors[i][0] = '\0';
			task.score = 0.0f;
			task.is_finished = 0;
			if (CreateInspection(L, task) == OK)
				printf("新增检查任务成功。\n");
			else
				printf("新增失败。\n");
			break;
		case 3:
			printf("请输入楼栋：");
			scanf(" %19s", building);
			printf("输入房间号：");
			scanf(" %9s", room);
			printf("请输入分数：");
			scanf("%f", &score);
			if (ModifyInspectionScore(L, building, room, score) == OK)
				printf("打分成功！\n");
			else
				printf("任务不存在\n");
			break;
		case 4:
			printf("请输入要删除的楼栋：");
			scanf(" %19s", building);
			printf("输入要删除的房间号：");
			scanf(" %9s", room);
			if (DeleteInspection(L, building, room) == OK)
				printf("删除成功。\n");
			else
				printf("删除失败，未找到任务。\n");
			break;
		case 6:
			ShowInspectionRanking(L);
			break;
		case 7:
			ShowDormCountByBuilding(all_students);
			break;
		case 8:
			ShowDormCountByDate(all_students);
			break;
		case 9:
			if (ExportStatisticsReport(L, all_students, "data/StatisticsReport.txt") == OK)
				printf("统计报表已导出到 data/StatisticsReport.txt\n");
			else
				printf("导出失败。\n");
			break;
		case 10:
		{
			char date_q[9] = { 0 }, building_q[20] = { 0 }, room_q[10] = { 0 };
			printf("输入日期(YYYYMMDD)或回车跳过：");
			scanf(" %8s", date_q);
			printf("输入楼栋或回车跳过：");
			scanf(" %19s", building_q);
			printf("请输入房间号或者使用回车跳过：");
			scanf(" %9s", room_q);
			if (SearchInspections(L, date_q, building_q, room_q) != OK)
				printf("未找到匹配项。\n");
		}
		break;
		case 11:
			ShowInspectorCounts(L);
			break;
		case 12:
		{
			char startDate[9] = { 0 }, endDate[9] = { 0 }, outPath[256] = { 0 };
			printf("请输入开始时间（YYYYMMDD）或者使用回车键表示不限制:");
			scanf(" %8s", startDate);
			printf("请输入结束时间（YYYYMMDD）或者使用回车键表示不限制：");
			scanf(" %8s", endDate);
			printf("输入导出文件路径（示例 data/InspectionsExport.txt）：");
			scanf(" %255s", outPath);
			if (ExportInspectionsByDate(L, startDate, endDate, outPath) == OK)
				printf("已导出到 %s\n", outPath);
			else
				printf("导出失败。\n");
		}
		break;
		case 5:
			if (SaveInspectionList(L) == OK)
				printf("已保存\n");
			else
				printf("保存失败。\n");
			return;
		default:
			printf("无效选项，请重新输入。\n");
			break;
		}
	}
}