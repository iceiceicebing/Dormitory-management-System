// dorm_system.h：存放各类用户信息的结构体定义以及函数声明

#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1. 学生信息的结构体定义

typedef struct {
	// 学生办事申请结构体
	char reason[110]; // 理由
	char start_date[9]; // 请假开始时间（格式：YYYYMMDD），例如20260425
	char end_date[9]; // 请假结束时间
	int approved; // 是否批准，0--不准，1--允许，负1--待审批
	char text[110]; // 导员留言
} ApplyElem;

typedef struct StudentInfo {
	// 学生信息结构体
	char name[50]; // 学生姓名
	char ID[13]; // 学号
	char gender [5];//性别
	char school_name[50] ; // 学院
	char class_name[50]; // 班级
	char dorm_building[20]; // 宿舍楼
	char room_number[10]; // 房间号
	int bed_number; // 床位号
	char contact_number[12]; // 联系电话
	char guardian_number[12]; // 监护人电话
	int in_dorm; // 是否在寝，0--离开，1--在寝
	char password[50]; // 登录密码
	int in_student_union; // 是否学生会干部，0--不是，1--干事，2--会长
	ApplyElem on_leave; // 请假申请
	ApplyElem dorm_alter; // 转宿舍申请；
	ApplyElem dorm_exit; // 退宿申请
} StudentInfo;

typedef struct StudentNode {
	StudentInfo data; // 数据域
	struct StudentNode* next; // 指针域，下一名学生的位置
} *StudentList;

extern StudentList all_students; // 学生表的全局变量声明，指向第一个学生的信息（数据+指针域）

// 2. 寝室信息的结构体定义

#define   LIST_INIT_SIZE    2000
#define   LISTINCREMENT     100

typedef struct {
	// 历史分数结构体
	float scores[10];
	int count; // 记录历史分数个数
} ScoreList;

typedef struct {
	char dorm_building[20];
	char room_number[10];
	char school_name[50]; // 宿舍所属学院
	float water_bill; // 水费余额
	float elec_bill; // 电费余额
	ScoreList all_scores; // 学生会近10次打分，score[0]最新打分，...，score[9]最旧打分
	char honor_info[20]; // 宿舍最近所获荣誉
	char ID1[13]; // 宿舍长学号
	char ID2[13]; // 舍员1学号
	char ID3[13]; // 舍员2学号
	char ID4[13]; // 舍员3学号
} DormInfo;

typedef struct {
	DormInfo* data; // 数据域，采用顺序表存储
	int count;		// 当前表元素个数
	int list_size;  // 表最大能容纳的数量
} DormList;

extern DormList all_dorms; // 宿舍表全局变量声明

// 3. 宿管信息的结构体定义

typedef struct {
	char name[50]; // 宿管姓名
	char ID[13]; // 宿管工号
	char password[50]; // 密码
	char building_on_duty[20]; // 值班宿舍楼
	int duty_period; // 值班时间段，值为0--8:00~16；00,值为1--16:00~23:00,值为2--23:00~8：00
	char contact_number[12]; // 联系电话
} ManagerInfo;

typedef struct ManagerNode {
	ManagerInfo data; 
	struct ManagerNode* next;
} *ManagerList;

extern ManagerList all_managers; // 宿管表的全局变量声明

// 4. 辅导员信息的结构体定义

typedef struct {
	char class_in_charge[10][50]; // 管理的班级，最多管理十个
	int count; // 班级数量
} duty;

typedef struct {
	char name[50]; // 导员姓名
	char ID[13]; // 导员工号
	char school_name[50]; // 所属学院
	char password[50]; // 密码
	duty classes; // 管理的班级
	char contact_number[12]; // 联系电话
} TeacherInfo;

typedef struct TeacherNode {
	TeacherInfo data;
	struct TeacherNode* next;
} *TeacherList;

extern TeacherList all_teachers; // 导员表的全局变量声明

// 5. 信息公告信息的结构体定义

typedef struct {
	int type; // 通知类型：0--重要通知，1--寻物启事，2--其他公告
	char time[9]; // 发布日期（格式：YYYYMMDD），例如20260425
	char heading[100]; // 信息标题
	char sender[50]; // 发布人
	char content[600]; // 发布内容
} AnnounceInfo;

typedef struct AnnounceNode {
	AnnounceInfo data;
	struct AnnounceNode* next;
} *AnnounceList;

extern AnnounceList announcement_board; // 全局变量声明：公告栏

// 6. 学生会评分任务信息的结构体定义

typedef struct {
	char inspectors[10][50]; // 最多十人检查，每个人名字长度不超过49
	int count; // 检查者数量
} InspectorList;

typedef struct {
	char time[9]; // 检查宿舍的日期（格式：YYYYMMDD），例如20260425
	char dorm_building[20]; // 宿舍楼
	char room_number[10]; // 房间号
	InspectorList all_inspectors; // 检查者信息
	float score; // 得分
	int is_finished; // 是否查完，0--任务未完成，1--任务已完成
} InspectionInfo;

typedef struct InspectionNode {
	InspectionInfo data;
	struct InspectionNode* next;
} *InspectionList;

extern InspectionList all_inspections; // 全局变量声明：检查单

// 7. 定义常量及变量类型

#define   OK        1
#define   ERROR     0
#define   TRUE      1
#define   FALSE     -1
#define   OVERFLOW  -2

typedef   int       Status; // 函数返回类型定义

// 7. 函数声明

	// (1) 学生端

Status InitStudentList(StudentList& L); // 建立带表头结点的空单链表，L为表头结点，不存数据，L->next为首个数据结点
Status LoadStudentList(StudentList& L); // 载入学生表
Status StudentInsert(StudentList& L, int i, StudentInfo e); // 向表中第i个位置后插入指定元素
Status SaveStudentList(StudentList& L); // 保存学生表
Status FindStudent(StudentList L, char ID[13], char password[50], StudentNode*& student); 
	// 登录操作：按照学号查找密码。若找到学生，则返回密码和指向当前学生的指针

void StudentSystem(StudentNode* student); // 学生端具体操作
// 之后的操作函数声明可以写在这后面……

	// (2) 宿舍信息存储

Status InitDormList(DormList& L); // 构造空的顺序表
Status LoadDormList(DormList& L);  // 载入宿舍表
Status SaveDormInfo(DormList& L); // 保存宿舍表

	// (3) 宿管端

Status InitManagerList(ManagerList& L); // 建立带表头结点的空单链表，L为表头结点，不存数据，L->next为首个数据结点
Status LoadManagerList(ManagerList& L); // 载入学生表
Status ManagerInsert(ManagerList& L, int i, ManagerInfo e); // 向表中第i个位置后插入指定元素
Status SaveManagerList(ManagerList& L); // 保存学生表
Status FindManager(ManagerList L, char ID[13], char password[50], ManagerNode*& manager); // 登录操作：按照工号查找密码

void ManagerSystem(ManagerNode* manager); // 宿管端具体操作
// 之后的操作函数声明可以写在这后面……

	// (4) 辅导员端

Status InitTeacherList(TeacherList& L); // 建立带表头结点的空单链表，L为表头结点，不存数据，L->next为首个数据结点
Status LoadTeacherList(TeacherList& L); // 载入导员表
Status TeacherInsert(TeacherList& L, int i, TeacherInfo e); // 向表中第i个位置后插入指定元素
Status SaveTeacherList(TeacherList& L); // 保存导员表
Status FindTeacher(TeacherList L, char ID[13], char password[50], TeacherNode*& teacher); // 登录操作：按照工号查找密码

void TeacherSystem(TeacherNode* teacher); // 辅导员端具体操作
// 之后的操作函数声明可以写在这后面……

	// (5) 信息公告存储

Status InitAnnounceList(AnnounceList& L); // 建立带表头结点的空单链表，L为表头结点，不存数据，L->next为首个数据结点
Status LoadAnnounceList(AnnounceList& L); // 载入信息公告表
	// 注意：后期添加信息公告时，内容不能出现空格。因为采用的是fscanf函数，读到空格就会停止
Status AnnounceInsert(AnnounceList& L, int i, AnnounceInfo e); // 向表中第i个位置后插入指定元素
Status SaveAnnounceList(AnnounceList& L); // 保存信息公告表
// 之后的操作函数声明可以写在这后面……

	// (6) 学生会端

Status InitInspectionList(InspectionList& L); // 建立带表头结点的空单链表，L为表头结点，不存数据，L->next为首个数据结点
Status LoadInspectionList(InspectionList& L); // 载入学生会任务表
Status InspectionInsert(InspectionList& L, int i, InspectionInfo e); // 向表中第i个位置后插入指定元素
Status SaveInspectionList(InspectionList& L); // 保存学生会任务表
Status FindUnionMember(StudentList L, char ID[13], char password[50], StudentNode*& union_member); 
	// 登录操作：按照学号，判断是否学生会成员，若是则查找密码

void StudentUnionSystem(StudentNode* union_member);
// 之后的操作函数声明可以写在这后面……





