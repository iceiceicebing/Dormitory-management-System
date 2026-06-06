// dorm.cpp：载入宿舍数据的操作函数的定义。
// 此文件仅是对宿舍信息的载入，不属于其他用户操作的范围。

#define _CRT_SECURE_NO_WARNINGS 1
#include "dorm_system.h"

Status InitDormList(DormList& L) {	
	// 构造空的顺序表
	L.data = (DormInfo*)malloc(LIST_INIT_SIZE * sizeof(DormInfo)); // 申请存储空间
	if (!L.data) 
		return ERROR;
	L.count = 0;
	L.list_size = LIST_INIT_SIZE;
	return OK;
}

Status LoadDormList(DormList& L) {
	FILE* fp = fopen("data/DormList.txt", "r");
	if (!fp)
		return ERROR;
	while (1){
		if (L.count >= L.list_size) {
			// 如果宿舍数量超过预设，则重新申请空间
			DormInfo* newbase = (DormInfo*)realloc(L.data, (L.list_size + LISTINCREMENT) * sizeof(DormInfo));
			if (!newbase) {
				fclose(fp);
				return ERROR; // 存储分配失败
			}
			L.data = newbase; // 新基址
			L.list_size += LISTINCREMENT;
		}
		else {
			int temp = fscanf(fp, "%s %s %s %f %f %f %f %f %f %f %f %f %f %f %f %d %s %s %s %s %s",
				L.data[L.count].dorm_building, L.data[L.count].room_number, 
				L.data[L.count].school_name, &L.data[L.count].water_bill, &L.data[L.count].elec_bill,
				&L.data[L.count].all_scores.scores[0], &L.data[L.count].all_scores.scores[1], 
				&L.data[L.count].all_scores.scores[2], &L.data[L.count].all_scores.scores[3], 
				&L.data[L.count].all_scores.scores[4], &L.data[L.count].all_scores.scores[5], 
				&L.data[L.count].all_scores.scores[6], &L.data[L.count].all_scores.scores[7],
				&L.data[L.count].all_scores.scores[8], &L.data[L.count].all_scores.scores[9],
				&L.data[L.count].all_scores.count, L.data[L.count].honor_info,
				L.data[L.count].ID1, L.data[L.count].ID2, L.data[L.count].ID3, L.data[L.count].ID4);
			if (temp == EOF) break;
			else L.count++;
		}
	}
	fclose(fp);
	return OK;
}

Status SaveDormInfo(DormList& L) {
	FILE* fp = fopen("data/DormList.txt", "w");
	if (!fp)
		return ERROR;
	int i = 0;
	while (i < L.count) {
		fprintf(fp, "%s %s %s %f %f %f %f %f %f %f %f %f %f %f %f %d %s %s %s %s %s\n",
				L.data[i].dorm_building, L.data[i].room_number,
				L.data[i].school_name, L.data[i].water_bill, L.data[i].elec_bill,
				L.data[i].all_scores.scores[0], L.data[i].all_scores.scores[1],
				L.data[i].all_scores.scores[2], L.data[i].all_scores.scores[3],
				L.data[i].all_scores.scores[4], L.data[i].all_scores.scores[5],
				L.data[i].all_scores.scores[6], L.data[i].all_scores.scores[7],
				L.data[i].all_scores.scores[8], L.data[i].all_scores.scores[9],
				L.data[i].all_scores.count, L.data[i].honor_info,
				L.data[i].ID1, L.data[i].ID2, L.data[i].ID3, L.data[i].ID4);
		i++;
	}
	fclose(fp);
	return OK;
}