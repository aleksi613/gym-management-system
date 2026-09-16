#ifndef MEMBER_H
#define MEMBER_H

#include "gym_types.h"

void printMember(const Member *member);
void addMember(MemberList *list, Member *member);
void deleteMember(MemberList *list, int memberID);
void listMembers(MemberList *list);
Member *findMemberByID(MemberList *list, int memberID);
void searchMembers(MemberList *list);

#endif
