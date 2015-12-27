#include "stdafx.h"
#include "BattleSoccer.h"
// GS-N 0.99.60T 0x00459AC0 - Completed
// GS-N 1.00.18 JPN 0x004691D0 - Completed

CBattleSoccer * gBSGround[MAX_BATTLESOCCER_GROUND];

CBattleSoccer::CBattleSoccer()
{
	this->m_goalmove = 0;
	this->m_BallIndex = 0;
}


CBattleSoccer::~CBattleSoccer()
{
	return;
}

void CBattleSoccer::SetGoalMove(int set_code)
{
	this->m_goalmove = set_code;
}

int CBattleSoccer::GetGoalMove()
{
	return this->m_goalmove;
}

void CBattleSoccer::SetGoalLeftRect(int x, int y, int xl, int yl)
{
	this->m_GoalLeft.left = x;
	this->m_GoalLeft.top = y;
	this->m_GoalLeft.right = xl;
	this->m_GoalLeft.bottom = yl;
}


void CBattleSoccer::SetGoalRightRect(int x, int y, int xl, int yl)
{
	this->m_GoalRight.left = x;
	this->m_GoalRight.top = y;
	this->m_GoalRight.right = xl;
	this->m_GoalRight.bottom = yl;
}
