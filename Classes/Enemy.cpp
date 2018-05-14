#include "Enemy.h"

Enemy::Enemy(void)
{
	HP = 100;
	ATK = 22;
	DEF = 5;
	coinsWorth = 4;
}

Enemy::~Enemy(void)
{
}

Enemy::Enemy(int gid)
{
	if (gid >= 0 && gid < 4) { 
		HP = 100;
		ATK = 22;
		DEF = 5;
		coinsWorth = 4;
	}

	if (gid >= 4 && gid < 8) {
		HP = 100;
		ATK = 22;
		DEF = 5;
		coinsWorth = 4;
	}

	if (gid >= 8 && gid < 12) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
		coinsWorth = 4;
	}

	if (gid >= 12) {
		HP = 100;
		ATK = 22;
		DEF = 5;
		coinsWorth = 4;
	}
	/*
	if (gid == 16) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 24) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 28) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 32) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 36) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 40) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 44) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 48) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 52) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 56) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 60) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 64) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 68) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 72) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 76) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 80) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 84) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 88) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 92) {  
		HP = 100;
		ATK = 22;
		DEF = 5;
	}

	if (gid == 96) {
		HP = 100;
		ATK = 22;
		DEF = 5;
	}
	*/
}
