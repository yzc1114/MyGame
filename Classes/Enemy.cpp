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
	//Ê·À³Ä·
	if (gid >= 35 && gid < 39) {
		ATK = 12;
		DEF = 6;
		HP = 10;
	}
	
	//òùòð
	if (gid >= 23 && gid < 27) {
		ATK = 16;
		DEF = 8;
		HP = 16;
	}
	
	//÷¼÷Ã
	if (gid >= 3 && gid < 7) {
		ATK = 20;
		DEF = 10;
		HP = 25;
	}
	
	//³Ö½£÷¼÷Ã
	if (gid >= 7 && gid < 11) {
		ATK = 26;
		DEF = 14;
		HP = 45;
	}

	//¾«Ó¢÷¼÷Ã
	if (gid >= 11 && gid < 15) {
		ATK = 36;
		DEF = 24;
		HP = 60;
	}

	//ÓÄÁé
	if (gid >= 51 && gid < 55) {
		ATK = 48;
		DEF = 28;
		HP = 55;
	}

	//¿ø¼×÷¼÷Ã
	if (gid >= 15 && gid < 19) {
		ATK = 44;
		DEF = 38;
		HP = 85;
	}

	//ÑªÉ«ÓÄÁé
	if (gid >= 55 && gid < 59) {
		ATK = 58;
		DEF = 30;
		HP = 70;
	}

	//ÊÞÈË
	if(gid >= 67 && gid < 71) {
		ATK = 54;
		DEF = 50;
		HP = 90;
	}

	//Î×Ê¦Ñ§Í½
	if (gid >= 59 && gid < 63) {
		ATK = 70;
		DEF = 40;
		HP = 70;
	}

	//´óÎ×Ê¦
	if (gid >= 63 && gid < 67) {
		ATK = 78;
		DEF = 46;
		HP = 85;
	}

	//ÊÞÈËÕ½Ê¿
	if (gid >= 71 && gid < 75) {
		ATK = 62;
		DEF = 58;
		HP = 110;
	}

	//Ê¯Ïñ¹í
	if (gid >= 75 && gid < 79) {
		ATK = 150;
		DEF = 55;
		HP = 40;
	}

	//Õ³ÒºÈË
	if (gid >= 79 && gid < 83) {
		ATK = 65;
		DEF = 65;
		HP = 160;
	}

	//ÉîÀ¶ÆïÊ¿
	if (gid >= 191 && gid < 195) {
		ATK = 75;
		DEF = 70;
		HP = 150;
	}

	//²ÔÓ°ÓÄ»ê
	if (gid >= 195 && gid < 199) {
		ATK = 115;
		DEF = 50;
		HP = 110;
	}

	//¿ìÀÖ·çÄÐ
	if (gid >= 127 && gid < 131) {
		ATK = 230;
		DEF = 75;
		HP = 500;
	}

	//Ðþ¼×ÖØÆï
	if (gid >= 99 && gid < 103) {
		ATK = 80;
		DEF = 80;
		HP = 200;
	}

	//Ë«½£ÏÀ¿Í
	if (gid >= 95 && gid < 99) {
		ATK = 120;
		DEF = 60;
		HP = 150;
	}

	//¹íÍ·
	if (gid >= 103 && gid < 107) {
		ATK = 120;
		DEF = 90;
		HP = 100;
	}

	//´óÄ§Íõ
	if (gid >= 211 && gid < 215) {
		ATK = 0;//250;
		DEF = 0;//160;
		HP = 1;// 1000;
		startGID = 211;
	}
}
