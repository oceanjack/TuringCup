//����
const int STOP = 0;
const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;
const int ATC = 5; //����
//**��������ָ��
const int ONE = 6; //С����1
const int TWO = 7; //С����2
const int THREE = 8; //С����3
const int KILL = 9; //����
const int DEAD = 44; //��ɱ����40��
//**

//ָ��
struct Order
{
	int act;
	int dir[30];
};

//�߽�
const int boderX = 22;
const int boderY = 17;

struct PlayerName
{
	char TeamName[50];		//��������
	char my1Name[50];		//һ��AI������
	char my2Name[50];		//����AI������
};

//��ͼ����
const int STAR = 0; //�����ߣ�ħ���ɴ�
const int LAND = 1; //���ߣ�ħ���ɴ�
const int TREE = 2; //�����ߣ�ħ�����ɴ�

//��Ʒ����
const int BLUE = 0; //�ӳ�ħ������
const int BOMB = 1; //��30��
const int BOOK = 2; //��30����
