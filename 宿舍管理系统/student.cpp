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

//辅助函数：根据学生学号查找该生所在宿舍在顺序表中的下标，找不到返回-1
int FindDormByStuID(DormList& dormList, char stuID[])
{
	for (int i = 0; i < dormList.count; i++)
	{
		DormInfo d = dormList.data[i];
		if (strcmp(d.ID1, stuID) == 0 || strcmp(d.ID2, stuID) == 0
			|| strcmp(d.ID3, stuID) == 0 || strcmp(d.ID4, stuID) == 0)
		{
			return i;
		}
	}
	return -1;
}

//1. 抢宿舍+选床位：指定楼栋、房间、床位分配给当前学生
Status SelectDormBed(StudentNode* stu, DormList& dormList, char build[], char room[], int bed)
{
	//先判断学生是否已有宿舍
	if (FindDormByStuID(dormList, stu->data.ID) != -1)
	{
		printf("你已分配宿舍，不可重复抢宿！\n");
		return ERROR;
	}

	//遍历寻找目标宿舍
	int targetIdx = -1;
	for (int i = 0; i < dormList.count; i++)
	{
		if (strcmp(dormList.data[i].dorm_building, build) == 0
			&& strcmp(dormList.data[i].room_number, room) == 0)
		{
			targetIdx = i;
			break;
		}
	}
	if (targetIdx == -1)
	{
		printf("不存在该楼栋房间！\n");
		return ERROR;
	}

	DormInfo* dorm = &dormList.data[targetIdx];
	//检查床位是否被占用
	char* bedID = NULL;
	switch (bed)
	{
	case 1: bedID = dorm->ID1; break;
	case 2: bedID = dorm->ID2; break;
	case 3: bedID = dorm->ID3; break;
	case 4: bedID = dorm->ID4; break;
	default:
		printf("床位号仅支持1-4！\n");
		return ERROR;
	}
	if (strlen(bedID) > 0)
	{
		printf("该床位已被他人选择！\n");
		return ERROR;
	}

	//分配床位，写入自己学号
	strcpy(bedID, stu->data.ID);
	stu->data.in_dorm = 1;
	printf("选宿成功！楼栋：%s 房间：%s 床位：%d\n", build, room, bed);
	return OK;
}

//2. 查询当前宿舍电费余额
void QueryElecBill(StudentNode* stu, DormList& dormList)
{
	int idx = FindDormByStuID(dormList, stu->data.ID);
	if (idx == -1)
	{
		printf("暂未分配宿舍，无法查询电费！\n");
		return;
	}
	DormInfo d = dormList.data[idx];
	printf("================电费查询===============\n");
	printf("楼栋：%s 房间：%s\n", d.dorm_building, d.room_number);
	printf("当前电费余额：%.2f 元\n", d.elec_bill);
	printf("========================================\n");
}

//电费充值
Status RechargeElec(StudentNode* stu, DormList& dormList, float money)
{
	if (money <= 0)
	{
		printf("充值金额必须大于0！\n");
		return ERROR;
	}
	int idx = FindDormByStuID(dormList, stu->data.ID);
	if (idx == -1)
	{
		printf("未分配宿舍，无法充值！\n");
		return ERROR;
	}
	dormList.data[idx].elec_bill += money;
	printf("充值成功！本次充值%.2f元，当前电费余额：%.2f\n", money, dormList.data[idx].elec_bill);
	return OK;
}

//3. 请假申请
void ApplyLeave(StudentNode* stu, char start[], char end[], char reason[])
{
	ApplyElem* leave = &stu->data.on_leave;
	strcpy(leave->start_date, start);
	strcpy(leave->end_date, end);
	strcpy(leave->reason, reason);
	leave->approved = -1; //待审批
	strcpy(leave->text, "等待辅导员审批");
	printf("请假申请提交完成，状态：待审批\n");
}

//校外住宿（退宿申请）
void ApplyOutDorm(StudentNode* stu, char start[], char end[], char reason[])
{
	ApplyElem* exit = &stu->data.dorm_exit;
	strcpy(exit->start_date, start);
	strcpy(exit->end_date, end);
	strcpy(exit->reason, reason);
	exit->approved = -1;
	strcpy(exit->text, "等待辅导员审批");
	printf("校外住宿退宿申请提交完成，状态：待审批\n");
}

//转宿舍申请
void ApplyChangeDorm(StudentNode* stu, char start[], char end[], char reason[])
{
	ApplyElem* alter = &stu->data.dorm_alter;
	strcpy(alter->start_date, start);
	strcpy(alter->end_date, end);
	strcpy(alter->reason, reason);
	alter->approved = -1;
	strcpy(alter->text, "等待辅导员审批");
	printf("转宿舍申请提交完成，状态：待审批\n");
}

//4. 查看个人基础信息
void ShowStudentInfo(StudentNode* stu)
{
	printf("=============个人信息=============\n");
	printf("姓名：%s\n", stu->data.name);
	printf("学号：%s\n", stu->data.ID);
	printf("性别：%s\n", stu->data.gender);
	printf("学院：%s\n", stu->data.school_name);
	printf("班级：%s\n", stu->data.class_name);
	printf("登录密码：%s\n", stu->data.password);
	printf("联系电话：%s\n", stu->data.contact_number);
	printf("监护人电话：%s\n", stu->data.guardian_number);
	printf("是否在校住宿：%s\n", stu->data.in_dorm ? "在寝" : "离校");
	if (stu->data.in_student_union == 0)
		printf("学生会身份：无\n");
	else if (stu->data.in_student_union == 1)
		printf("学生会身份：干事\n");
	else
		printf("学生会身份：会长\n");
	printf("===================================\n");
}

//查看所有申请审批状态
void ShowAllApplyStatus(StudentNode* stu)
{
	ApplyElem leave = stu->data.on_leave;
	ApplyElem alter = stu->data.dorm_alter;
	ApplyElem exit = stu->data.dorm_exit;

	printf("========各类申请审批状态========\n");
	//请假
	printf("【请假申请】\n");
	printf("时间：%s ~ %s 理由：%s\n", leave.start_date, leave.end_date, leave.reason);
	if (leave.approved == -1) printf("审批状态：待审核 导员留言：%s\n", leave.text);
	else if (leave.approved == 1) printf("审批状态：已通过 导员留言：%s\n", leave.text);
	else printf("审批状态：驳回 导员留言：%s\n", leave.text);

	printf("【转宿申请】\n");
	printf("时间：%s ~ %s 理由：%s\n", alter.start_date, alter.end_date, alter.reason);
	if (alter.approved == -1) printf("审批状态：待审核 导员留言：%s\n", alter.text);
	else if (alter.approved == 1) printf("审批状态：已通过 导员留言：%s\n", alter.text);
	else printf("审批状态：驳回 导员留言：%s\n", alter.text);

	printf("【校外退宿申请】\n");
	printf("时间：%s ~ %s 理由：%s\n", exit.start_date, exit.end_date, exit.reason);
	if (exit.approved == -1) printf("审批状态：待审核 导员留言：%s\n", exit.text);
	else if (exit.approved == 1) printf("审批状态：已通过 导员留言：%s\n", exit.text);
	else printf("审批状态：驳回 导员留言：%s\n", exit.text);
	printf("================================\n");
}

void ReadLine(char buf[], int maxLen)
{
	fgets(buf, maxLen, stdin);
	int len = strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
	{
		buf[len - 1] = '\0';
	}
}

void StudentSystem(StudentNode* student) {
	// ……该函数为学生端总操作函数，从这里可以调用任一个学生端操作函数
	int op;
	while (1)
	{
		printf("\n=========学生自助系统=========\n");
		printf("1. 查看个人信息\n");
		printf("2. 抢宿舍、选择床位\n");
		printf("3. 电费余额查询\n");
		printf("4. 电费充值\n");
		printf("5. 提交请假申请\n");
		printf("6. 提交校外住宿（退宿）申请\n");
		printf("7. 提交转宿舍申请\n");
		printf("8. 查看所有申请审批状态\n");
		printf("0. 退出学生端\n");
		printf("==============================\n");
		printf("请输入功能序号：");
		scanf("%d", &op);
		getchar(); // 吸收数字后的换行，防止ReadLine读空

		char build[20], room[10], start[9], end[9], reason[110];
		int bed;
		float money;

		switch (op)
		{
		case 1:
			ShowStudentInfo(student);
			break;
		case 2:
			printf("请输入宿舍楼栋：");
			scanf("%s", build);
			getchar();
			printf("请输入房间号：");
			scanf("%s", room);
			getchar();
			printf("请输入床位(1-4)：");
			scanf("%d", &bed);
			getchar();
			SelectDormBed(student, all_dorms, build, room, bed);
			SaveDormInfo(all_dorms);
			SaveStudentList(all_students);
			break;
		case 3:
			QueryElecBill(student, all_dorms);
			break;
		case 4:
			printf("输入充值金额：");
			scanf("%f", &money);
			getchar();
			if (RechargeElec(student, all_dorms, money) == OK)
			{
				SaveDormInfo(all_dorms);
			}
			break;
		case 5:
			printf("请假开始日期(YYYYMMDD)：");
			scanf("%s", start);
			getchar();
			printf("请假结束日期(YYYYMMDD)：");
			scanf("%s", end);
			getchar();
			printf("请假理由：");
			ReadLine(reason, 110);
			ApplyLeave(student, start, end, reason);
			SaveStudentList(all_students);
			break;
		case 6:
			printf("校外住宿开始日期(YYYYMMDD)：");
			scanf("%s", start);
			getchar();
			printf("校外住宿结束日期(YYYYMMDD)：");
			scanf("%s", end);
			getchar();
			printf("申请理由：");
			ReadLine(reason, 110);
			ApplyOutDorm(student, start, end, reason);
			SaveStudentList(all_students);
			break;
		case 7:
			printf("转宿起始日期(YYYYMMDD)：");
			scanf("%s", start);
			getchar();
			printf("转宿结束日期(YYYYMMDD)：");
			scanf("%s", end);
			getchar();
			printf("转宿理由：");
			ReadLine(reason, 110);
			ApplyChangeDorm(student, start, end, reason);
			SaveStudentList(all_students);
			break;
		case 8:
			ShowAllApplyStatus(student);
			break;
		case 0:
			printf("退出学生自助端...\n");
			return;
		default:
			printf("输入序号错误，请重新选择！\n");
		}
		system("pause");
		system("cls");
	}
}
