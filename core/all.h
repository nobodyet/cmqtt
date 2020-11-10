#ifndef __MOZIT_ALL_H
#define __MOZIT_ALL_H

/* 玩家在数据库中的比赛状态 */
#define M_SIGNUP			1		// 报名
#define M_MATCHSTART		2		// 大厅服务器 判断比赛开始
#define M_MATCHCANCEL		3		// 大厅服务器 判断比赛取消
#define M_SIGNIN			8		// 在游戏服务器 签到
#define M_GIVEUP			9		// 在游戏服务器 放弃比赛
#define M_MATCHOVER			10		// 在游戏服务器 比赛结束
#define M_MATCHING		 	16		// 在游戏服务器 比赛中

/*
	在玩家退赛，或者比赛结束后 玩家在数据库中的状态就删除了
*/

#define R_CHECK				1
#define R_GAMEOVER			10
#define R_GAMING			16

#endif /* #ifndef __MOZIT_ALL_H */

/* END */

