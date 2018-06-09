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
	//ʷ��ķ
	if (gid >= 35 && gid < 39) {
		ATK = 12;
		DEF = 6;
		HP = 10;
	}
	
	//����
	if (gid >= 23 && gid < 27) {
		ATK = 16;
		DEF = 8;
		HP = 16;
	}
	
	//����
	if (gid >= 3 && gid < 7) {
		ATK = 20;
		DEF = 10;
		HP = 25;
	}
	
	//�ֽ�����
	if (gid >= 7 && gid < 11) {
		ATK = 26;
		DEF = 14;
		HP = 45;
	}

	//��Ӣ����
	if (gid >= 11 && gid < 15) {
		ATK = 34;
		DEF = 20;
		HP = 60;
	}

	//����
	if (gid >= 51 && gid < 55) {
		ATK = 46;
		DEF = 26;
		HP = 55;
	}

	//��������
	if (gid >= 15 && gid < 19) {
		ATK = 40;
		DEF = 36;
		HP = 80;
	}

	//Ѫɫ����
	if (gid >= 55 && gid < 59) {
		ATK = 56;
		DEF = 30;
		HP = 70;
	}

	//����
	if(gid >= 67 && gid < 71) {
		ATK = 46;
		DEF = 48;
		HP = 90;
	}

	//��ʦѧͽ
	if (gid >= 59 && gid < 63) {
		ATK = 70;
		DEF = 40;
		HP = 70;
	}

	//����ʦ
	if (gid >= 63 && gid < 67) {
		ATK = 80;
		DEF = 44;
		HP = 85;
	}

	//����սʿ
	if (gid >= 71 && gid < 75) {
		ATK = 53;
		DEF = 56;
		HP = 110;
	}

	//ʯ���
	if (gid >= 75 && gid < 79) {
		ATK = 130;
		DEF = 30;
		HP = 55;
	}

	//ճҺ��
	if (gid >= 79 && gid < 83) {
		ATK = 58;
		DEF = 68;
		HP = 150;
	}

	//������ʿ
	if (gid >= 191 && gid < 195) {
		ATK = 66;
		DEF = 66;
		HP = 160;
	}

	//��Ӱ�Ļ�
	if (gid >= 195 && gid < 199) {
		ATK = 100;
		DEF = 45;
		HP = 120;
	}

	//���ַ���
	if (gid >= 127 && gid < 131) {
		ATK = 220;
		DEF = 80;
		HP = 600;
	}

	//��������
	if (gid >= 99 && gid < 103) {
		ATK = 78;
		DEF = 78;
		HP = 200;
	}

	//˫������
	if (gid >= 95 && gid < 99) {
		ATK = 100;
		DEF = 65;
		HP = 160;
	}

	//��ħ��
	if (gid >= 211 && gid < 215) {
		ATK = 250;
		DEF = 200;
		HP = 1000;
	}
}
