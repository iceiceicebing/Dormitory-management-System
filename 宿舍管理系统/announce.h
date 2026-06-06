#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include "dorm_system.h" // 确保能用到 AnnounceNode, ManagerNode 等结构体和全局变量

Status InitAnnounceList(AnnounceList& L);
Status LoadAnnounceList(AnnounceList& L);
Status AnnounceInsert(AnnounceList& L, int i, AnnounceInfo e);
Status SaveAnnounceList(AnnounceList& L);
void HandlePublishAnnouncement(const char* sender_name);
void HandleViewAnnouncements();


#endif // ANNOUNCEMENT_H