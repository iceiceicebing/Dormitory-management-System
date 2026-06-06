#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"
#include "announce.h"
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
            "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s",
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
            temp.contact_number
        );

        if (ret != 16) {
            // 不直接返回，记录警告并跳过该行，避免整表加载失败
            printf("警告：解析 TeacherList.txt 行失败（字段数=%d），跳过该行：\n  %s\n", ret, line);
            continue;
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
        fprintf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %d %s\n",
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
            s->data.contact_number);
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



// 16. 导员端：查看所负责班级的卫生检查结果（已修复图片 image_e264e9.png 中数量对不上的Bug）
void TeacherViewClassDormScores() {
    if (all_students == NULL || all_students->next == NULL) {
        printf("\n系统中暂无任何学生数据。\n");
        return;
    }
    if (all_dorms.count == 0) {
        printf("\n系统中暂无任何宿舍打分数据。\n");
        return;
    }

    char target_class[50] = { 0 };
    printf("\n===== 查看负责班级的卫生检查结果 =====\n");
    printf("请输入你要查询的班级名称（如：计科2401）：");
    fgets(target_class, sizeof(target_class), stdin);
    target_class[strcspn(target_class, "\n")] = '\0';

    typedef struct {
        char building[20];
        char room[10];
    } ClassDormInfo;

    ClassDormInfo class_dorms[200];
    int class_dorm_count = 0;

    // 1. 提取该班级学生分布的所有宿舍
    StudentNode* p = all_students->next;
    while (p != NULL) {
        if (strcmp(p->data.class_name, target_class) == 0) {
            int is_duplicate = 0;
            for (int i = 0; i < class_dorm_count; i++) {
                if (strcmp(class_dorms[i].building, p->data.dorm_building) == 0 &&
                    strcmp(class_dorms[i].room, p->data.room_number) == 0) {
                    is_duplicate = 1;
                    break;
                }
            }
            if (!is_duplicate && class_dorm_count < 200) {
                strcpy(class_dorms[class_dorm_count].building, p->data.dorm_building);
                strcpy(class_dorms[class_dorm_count].room, p->data.room_number);
                class_dorm_count++;
            }
        }
        p = p->next;
    }

    if (class_dorm_count == 0) {
        printf("未找到班级 \"%s\" 的学生住宿信息。\n", target_class);
        return;
    }

    // 2. 匹配并打印
    printf("\n===== 班级 [%s] 所属宿舍卫生检查结果 =====\n", target_class);
    printf("--------------------------------------------------\n");
    
    int found_dorm_system = 0; // 真实在系统中找到并打印的宿舍数量
    for (int i = 0; i < class_dorm_count; i++) {
        for (int j = 0; j < all_dorms.count; j++) {
            if (strcmp(all_dorms.data[j].dorm_building, class_dorms[i].building) == 0 &&
                strcmp(all_dorms.data[j].room_number, class_dorms[i].room) == 0) {
                
                found_dorm_system++;
                printf("【宿舍】%s %s\n", all_dorms.data[j].dorm_building, all_dorms.data[j].room_number);
                
                int score_cnt = all_dorms.data[j].all_scores.count;
                if (score_cnt <= 0) {
                    printf("    暂无历史卫生检查打分记录。\n");
                } else {
                    float total = 0;
                    printf("    历史得分记录：");
                    for (int k = 0; k < score_cnt; k++) {
                        printf("%.0f ", all_dorms.data[j].all_scores.scores[k]);
                        total += all_dorms.data[j].all_scores.scores[k];
                    }
                    printf("\n    综合平均分数：%.1f 分\n", total / score_cnt);
                }
                printf("    当前所获荣誉：%s\n", all_dorms.data[j].honor_info);
                printf("--------------------------------------------------\n");
                break;
            }
        }
    }

    // 修正：使用 found_dorm_system 保证界面显示数量与提示完全一致
    printf("共拉取到该班级对应的 %d 间宿舍卫生档案。\n", found_dorm_system);
}

// 17. 导员端新增：标记/评定优秀宿舍荣誉功能
void TeacherAwardDormHonor() {
    if (all_dorms.count == 0) {
        printf("\n系统中暂无宿舍数据。\n");
        return;
    }

    char building_input[20] = { 0 };
    char room_input[10] = { 0 };
    char honor_input[100] = { 0 };
    int found = 0;

    printf("\n===== 评定/修改宿舍荣誉称号 =====\n");
    printf("请输入宿舍楼栋（如文园1栋）：");
    fgets(building_input, sizeof(building_input), stdin);
    building_input[strcspn(building_input, "\n")] = '\0';

    printf("请输入房间号（如101）：");
    fgets(room_input, sizeof(room_input), stdin);
    room_input[strcspn(room_input, "\n")] = '\0';

    // 寻找对应的宿舍
    for (int i = 0; i < all_dorms.count; i++) {
        if (strcmp(all_dorms.data[i].dorm_building, building_input) == 0 &&
            strcmp(all_dorms.data[i].room_number, room_input) == 0) {
            
            found = 1;
            printf("\n已找到该宿舍！当前荣誉为：[%s]\n", all_dorms.data[i].honor_info);
            
            // 计算一下平均分给导员做参考
            int score_cnt = all_dorms.data[i].all_scores.count;
            if (score_cnt > 0) {
                float total = 0;
                for (int k = 0; k < score_cnt; k++) total += all_dorms.data[i].all_scores.scores[k];
                printf("该宿舍历史平均卫生分数为：%.1f 分\n", total / score_cnt);
            }

            printf("请输入授予该宿舍的荣誉名称（如\"文明宿舍\"、\"标兵宿舍\"，输入\"无\"清空）：");
            fgets(honor_input, sizeof(honor_input), stdin);
            honor_input[strcspn(honor_input, "\n")] = '\0';

            // 写入结构体
            strcpy(all_dorms.data[i].honor_info, honor_input);
            
            // 立即调用现有的保存函数将数据同步到本地文本文件
            // 假设你的宿舍保存函数叫 SaveDormInfo 或 SaveDormList，请确保名字对得上
            SaveDormInfo(all_dorms); 

            printf("荣誉评定成功！%s %s 已成功标记为: [%s]\n", 
                   building_input, room_input, honor_input);
            break;
        }
    }

    if (!found) {
        printf("未在系统数据库中找到宿舍：%s %s，请检查输入。\n", building_input, room_input);
    }
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
		printf("5. 查看所负责班级的卫生检查结果\n");
        printf("6. 发布公告\n");
		printf("7. 查看公告\n");
		printf("8. 评定/修改宿舍荣誉称号\n");
        printf("0. 保存导员信息并退出\n");
        printf("=========================\n");
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
            TeacherViewClassDormScores();
			break;
        case 6:
            HandlePublishAnnouncement(teacher->data.name); // 传入导员的名字
            break;
        case 7:  
            HandleViewAnnouncements(); // 直接调用，不需要传任何参数
            break;
        case 8:
            TeacherAwardDormHonor();
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
}