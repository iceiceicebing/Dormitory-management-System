// manager.cpp：宿管端操作函数的定义

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Status InitManagerList(ManagerList& L) {
    L = (ManagerList)malloc(sizeof(ManagerNode));
    if (!L) return ERROR;
    memset(L, 0, sizeof(ManagerNode)); // 确保头结点清零
    L->next = NULL;
    return OK;
}

Status LoadManagerList(ManagerList& L) {
    FILE* fp = fopen("data/ManagerList.txt", "r");
    if (!fp)
        return ERROR;
        
    ManagerList rear = L; 
    char line[256];
    
    // 弃用 fscanf，改用 fgets + sscanf，解决错位导致的乱码
    while (fgets(line, sizeof(line), fp)) {
        // 跳过空行
        if (line[0] == '\n' || line[0] == '\r' || strlen(line) < 5) continue;
        
        ManagerInfo temp;
        memset(&temp, 0, sizeof(ManagerInfo)); // 彻底清零脏数据

        if (sscanf(line, "%s %s %s %s %d %s %d",
            temp.name, temp.ID, temp.password, temp.building_on_duty,
            &temp.duty_period, temp.contact_number, &temp.is_leader) == 7) {
            
            ManagerList s = (ManagerNode*)malloc(sizeof(ManagerNode)); 
            memset(s, 0, sizeof(ManagerNode)); // 新结点清零
            s->data = temp;
            s->next = NULL;

            rear->next = s; 
            rear = s; 
        }
    }
    fclose(fp);
    return OK;
}

Status ManagerInsert(ManagerList& L, int i, ManagerInfo e) {
    int k = 0;
    ManagerNode* p = L->next, * q = L, * s;
    if (i < 1) return ERROR;
    while (q) {
        k++; 
        if (k == i) {
            s = (ManagerNode*)malloc(sizeof(ManagerNode)); 
            memset(s, 0, sizeof(ManagerNode));
            s->data = e;
            s->next = p; 
            q->next = s;
            return OK;
        }
        else {
            q = p;
            p = p->next;
        }
    }
    return ERROR; 
}

Status SaveManagerList(ManagerList& L) {
    FILE* fp = fopen("data/ManagerList.txt", "w");
    if (!fp) return ERROR;
    ManagerList s = L->next; 
    while (s != NULL) {
        fprintf(fp, "%s %s %s %s %d %s %d\n",
            s->data.name, s->data.ID, s->data.password, s->data.building_on_duty,
            s->data.duty_period, s->data.contact_number, s->data.is_leader);
        s = s->next; 
    }
    fclose(fp);
    return OK;
}

Status FindManager(ManagerList L, char ID[13], char password[50], ManagerNode*& manager) {
    ManagerNode* p = L->next;
    while (p) {
        if (strcmp(p->data.ID, ID) == 0) {
            strcpy(password, p->data.password);
            manager = p;
            return OK;
        }
        p = p->next;
    }
    return ERROR; 
}

void trim(char* str) {
    if (str == NULL) return;
    char* start = str;
    char* end = str + strlen(str) - 1;
    while (isspace((unsigned char)*start)) start++;
    while (end > start && isspace((unsigned char)*end)) end--;
    if (str != start) memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

// 宿管端总操作函数
void ManagerSystem(ManagerNode* manager) {
    if (manager == NULL) {
        printf("错误：宿管信息无效！\n");
        return;
    }

    int choice;
    char building[20] = { 0 };
    char room[10] = { 0 };
    int score = 0;
    int found = 0;
    int i = 0, j = 0;
    char input[100] = { 0 };

    while (1) {
        system("cls");
        printf("========== 宿管管理系统 ==========\n");
        printf("欢迎, %s\n", manager->data.name);
        printf("负责楼栋: %s\n", manager->data.building_on_duty);
        printf("==================================\n");
        printf("  1. 查房打分\n");
        printf("  2. 审批换宿申请\n");
        printf("  3. 审批退宿申请\n");
        printf("  4. 发布公告\n");
        printf("  5. 查看所有公告\n");
        printf("  6. 查看宿舍信息\n");
        printf("  0. 退出并返回主菜单\n");
        printf("==================================\n");
        printf("请选择:");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("输入错误，请输入数字！\n");
            printf("按回车键继续...");
            getchar();
            continue;
        }
        getchar(); // 吸收回车

        switch (choice) {
        case 1:  // 查房打分
        {
            char building_input[20] = { 0 };
            char room_input[10] = { 0 };
            int score_input = 0;
            int found_input = 0;
            int i_input = 0;

            printf("\n请输入要查房的宿舍楼：");
            fgets(building_input, sizeof(building_input), stdin);
            building_input[strcspn(building_input, "\n")] = '\0';
            printf("请输入房间号（如：101）：");
            fgets(room_input, sizeof(room_input), stdin);
            room_input[strcspn(room_input, "\n")] = '\0';

            for (i_input = 0; i_input < all_dorms.count; i_input++) {
                if (strcmp(all_dorms.data[i_input].dorm_building, building_input) == 0 &&
                    strcmp(all_dorms.data[i_input].room_number, room_input) == 0) {
                    found_input = 1;
                    printf("请输入本次查寝分数（0-100）：");
                    if (scanf("%d", &score_input) != 1) {
                        printf("分数输入错误！\n");
                        while (getchar() != '\n');
                        break;
                    }
                    getchar();

                    if (all_dorms.data[i_input].all_scores.count < 10) {
                        all_dorms.data[i_input].all_scores.scores[all_dorms.data[i_input].all_scores.count] = (float)score_input;
                        all_dorms.data[i_input].all_scores.count++;

                    }
                    else {
                        for (j = 0; j < 9; j++) {
                            all_dorms.data[i_input].all_scores.scores[j] = all_dorms.data[i_input].all_scores.scores[j + 1];
                        }
                        all_dorms.data[i_input].all_scores.scores[9] = (float)score_input;
                    }
                    printf("打分成功！该宿舍当前共有 %d 次查寝记录。\n", all_dorms.data[i_input].all_scores.count);
                    SaveDormInfo(all_dorms);
                    break;
                }
            }
            if (!found_input) {
                printf("未找到宿舍 %s %s，请检查楼栋和房间号是否正确。\n", building_input, room_input);
            }
            break;
        }

        case 2:  // 审批换宿申请
        {
            StudentNode* p = all_students->next;
            int count_pending = 0;
            StudentNode* pending[100] = { NULL };

            while (p != NULL) {
                if (p->data.dorm_alter.approved == -1) {
                    pending[count_pending] = p;
                    count_pending++;
                }
                p = p->next;
            }

            if (count_pending == 0) {
                printf("当前没有待审批的换宿申请。\n");
                break;
            }

            printf("\n===== 待审批的换宿申请列表 =====\n");
            for (i = 0; i < count_pending; i++) {
                printf("%d. 学生：%s（学号：%s）\n", i + 1, pending[i]->data.name, pending[i]->data.ID);
                printf("   原宿舍：%s %s  床位：%d\n",
                    pending[i]->data.dorm_building,
                    pending[i]->data.room_number,
                    pending[i]->data.bed_number);
                printf("   申请理由：%s\n", pending[i]->data.dorm_alter.reason);
                printf("   申请日期：%s\n", pending[i]->data.dorm_alter.start_date);
                printf("--------------------------------\n");
            }

            int sel;
            printf("请输入要审批的申请编号（1-%d，输入0取消）：", count_pending);
            if (scanf("%d", &sel) != 1) {
                while (getchar() != '\n');
                printf("输入无效。\n");
                break;
            }
            getchar();

            if (sel >= 1 && sel <= count_pending) {
                StudentNode* target = pending[sel - 1];
                printf("批准(1)或驳回(0)：");
                int approve;
                if (scanf("%d", &approve) != 1) {
                    while (getchar() != '\n');
                    printf("输入无效。\n");
                    break;
                }
                getchar();

                if (approve == 1) {
                    target->data.dorm_alter.approved = 1;
					SaveStudentList(all_students); // 立即保存审批结果
                    strcpy(target->data.dorm_alter.text, "宿管已经批准,请等待导员意见或重新申请");
                    printf("已批准学生 %s 的换宿申请。\n", target->data.name);

                }
                else if (approve == 0) {
                    target->data.dorm_alter.approved = 0;
                    SaveStudentList(all_students);
                    printf("请输入驳回理由：");
                    fgets(target->data.dorm_alter.text, sizeof(target->data.dorm_alter.text), stdin);
                    target->data.dorm_alter.text[strcspn(target->data.dorm_alter.text, "\n")] = '\0';
                    printf("已驳回学生 %s 的换宿申请。\n", target->data.name);
                }
                else {
                    printf("输入错误，未作修改。\n");
                }
            }
            else if (sel != 0) {
                printf("无效编号。\n");
            }
            break;
        }

        case 3:  // 审批退宿申请
        {
            StudentNode* p = all_students->next;
            int count_pending = 0;
            StudentNode* pending[100] = { NULL };

            while (p != NULL) {
                if (p->data.dorm_exit.approved == -1) {
                    pending[count_pending] = p;
                    count_pending++;
                }
                p = p->next;
            }

            if (count_pending == 0) {
                printf("当前没有待审批的退宿申请。\n");
                break;
            }

            printf("\n===== 待审批的退宿申请列表 =====\n");
            for (i = 0; i < count_pending; i++) {
                printf("%d. 学生：%s（学号：%s）\n", i + 1, pending[i]->data.name, pending[i]->data.ID);
                printf("   原宿舍：%s %s  床位：%d\n",
                    pending[i]->data.dorm_building,
                    pending[i]->data.room_number,
                    pending[i]->data.bed_number);
                printf("   申请理由：%s\n", pending[i]->data.dorm_exit.reason);
                printf("   申请日期：%s\n", pending[i]->data.dorm_exit.start_date);
                printf("--------------------------------\n");
            }

            int sel;
            printf("请输入要审批的申请编号（1-%d，输入0取消）：", count_pending);
            if (scanf("%d", &sel) != 1) {
                while (getchar() != '\n');
                printf("输入无效。\n");
                break;
            }
            getchar();

            if (sel >= 1 && sel <= count_pending) {
                StudentNode* target = pending[sel - 1];
                printf("批准(1)或驳回(0)：");
                int approve;
                if (scanf("%d", &approve) != 1) {
                    while (getchar() != '\n');
                    printf("输入无效。\n");
                    break;
                }
                getchar();
                if (approve == 1) {
                    target->data.dorm_exit.approved = 1;
                    SaveStudentList(all_students);
                    strcpy(target->data.dorm_exit.text, "宿管已经批准,请等待导员意见或重新申请");
                    printf("已批准学生 %s 的退宿申请。\n", target->data.name);
                }
                else if (approve == 0) {
                    target->data.dorm_exit.approved = 0;
                    printf("请输入驳回理由：");
                    fgets(target->data.dorm_exit.text, sizeof(target->data.dorm_exit.text), stdin);
                    target->data.dorm_exit.text[strcspn(target->data.dorm_exit.text, "\n")] = '\0';
                    printf("已驳回学生 %s 的退宿申请。\n", target->data.name);
                }
                else {
                    printf("输入错误，未作修改。\n");
                }
            }
            else if (sel != 0) {
                printf("无效编号。\n");
            }
            break;
        }

        case 4:  // 发布公告
        {
            AnnounceNode* newAnnounce = (AnnounceNode*)malloc(sizeof(AnnounceNode));
            if (newAnnounce == NULL) {
                printf("内存分配失败，无法发布公告。\n");
                break;
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
                break;
            }
            getchar();

            printf("请输入标题：");
            fgets(newAnnounce->data.heading, sizeof(newAnnounce->data.heading), stdin);
            newAnnounce->data.heading[strcspn(newAnnounce->data.heading, "\n")] = '\0';

            printf("请输入内容（支持多行，以单独一行'END'结束）：\n");
            char line[500] = { 0 };
            char content[1024] = { 0 }; // 扩大了容量防止溢出
            while (1) {
                fgets(line, sizeof(line), stdin);
                if (strcmp(line, "END\n") == 0 || strcmp(line, "END") == 0) {
                    break;
                }
                strncat(content, line, sizeof(content) - strlen(content) - 1);
            }
            strcpy(newAnnounce->data.content, content);

            strcpy(newAnnounce->data.sender, manager->data.name);
            printf("请输入发布日期（字符串，格式如20260603）：");
            fgets(newAnnounce->data.time, sizeof(newAnnounce->data.time), stdin);
            newAnnounce->data.time[strcspn(newAnnounce->data.time, "\n")] = '\0';

            // 头插法加入链表
            newAnnounce->next = announcement_board->next;
            announcement_board->next = newAnnounce;
			SaveAnnounceList(announcement_board);
            printf("公告发布成功\n");
            break;
        }

        case 5:  // 查看所有公告
        {
            if (announcement_board == NULL) {
                printf("\n暂无任何公告。\n");
                break;
            }
            printf("\n===== 全部公告 =====\n");
            AnnounceNode* p = announcement_board->next;
            int idx = 1;
            while (p != NULL) {
                printf("【公告 %d】\n", idx++);
                printf("类型：%d   发布人：%s   日期：%s\n", p->data.type, p->data.sender, p->data.time);
                printf("标题：%s\n", p->data.heading);
                printf("内容：\n%s\n", p->data.content);
                printf("----------------------\n");
                p = p->next;
            }
            break;
        }

        case 6:  // 查看宿舍信息
        {
            char building_filter[20] = { 0 };
            printf("\n请输入要查看的宿舍楼（如文园1栋，全部输入 *）：");
            fgets(building_filter, sizeof(building_filter), stdin);
            building_filter[strcspn(building_filter, "\n")] = '\0';

            printf("\n===== 宿舍信息 =====\n");
            int printed = 0;
            for (i = 0; i < all_dorms.count; i++) {
                if (strcmp(building_filter, "*") == 0 ||
                    strcmp(all_dorms.data[i].dorm_building, building_filter) == 0) {
                    printed = 1;
                    printf("楼栋：%s   房间：%s\n", all_dorms.data[i].dorm_building, all_dorms.data[i].room_number);
                    printf("  所属学院：%s\n", all_dorms.data[i].school_name);
                    printf("  水费：%.2f   电费：%.2f\n", all_dorms.data[i].water_bill, all_dorms.data[i].elec_bill);
                    printf("  床位：%s, %s, %s, %s\n",
                        all_dorms.data[i].ID1, all_dorms.data[i].ID2,
                        all_dorms.data[i].ID3, all_dorms.data[i].ID4);
                    printf("  最近%d次查寝分数：", all_dorms.data[i].all_scores.count);
                    for (j = 0; j < all_dorms.data[i].all_scores.count; j++) {
                        printf("%.0f ", all_dorms.data[i].all_scores.scores[j]);
                    }
                    printf("\n  荣誉：%s\n", all_dorms.data[i].honor_info);
                    printf("-----------------------------------\n");
                }
            }
            if (!printed) {
                printf("未找到宿舍楼 \"%s\" 的信息。\n", building_filter);
            }
            break;
        }

        case 0:
            printf("正在退出，返回主菜单...\n");
            return;

        default:
            printf("输入错误，请重新选择！\n");
            break;
        }

        printf("\n按回车键继续...");
        getchar();
    }
}