#include<iostream>
using namespace std;
#include<string>
#include<vector>
struct Warrior {
	int id;
	int life;
	string name;        // 0:dragon, 1:ninja, 2:iceman, 3:lion, 4:wolf
	string color;
	int type;
};
class commander
{
public:
	int cLife;
	int color;//0是红方，1是蓝方
	int nextindex;
	int worriorNum;
	Warrior dragon;
	Warrior ninja;
	Warrior iceman;
	Warrior lion;
	Warrior wolf;
	int numDragon;
	int numninja;
	int numiceman;
	int numlion;
	int numwolf;
	vector<Warrior> w;
	vector<int> wnum = { 0,0,0,0,0 };
	vector<int> red = {2,3,4,1,0};
	vector<int> blue = { 3,0,1,2,4 };
	commander(int l,int dr,int ni,int ice,int li,int wo,int color)
	{
		cLife = l;
		dragon.life = dr;
		dragon.name = "dragon";
		dragon.type = 0;
		ninja.life = ni;
		ninja.name = "ninja";
		ninja.type = 1;
		iceman.life = ice;
		iceman.name = "iceman";
		iceman.type = 2;
		lion.life = li;
		lion.name = "lion";
		lion.type = 3;
		wolf.life = wo;
		wolf.name = "wolf";
		wolf.type = 4;
		this->color = color;
		nextindex = 0;
		worriorNum = 0;
		w = { dragon,ninja,iceman,lion,wolf };
	}
	Warrior creWarrior()
	{
		int startindex = nextindex;
		Warrior temp;
		while (1)
		{
			if (color == 0)
			{
				temp = w[red[nextindex]];
			}
			else
			{
				temp = w[blue[nextindex]];
			}
			if (temp.life <= cLife)
			{
				worriorNum++;
				temp.id = worriorNum;
				if (color == 0)
				{
					wnum[red[nextindex]]++;
				}
				else
				{
					wnum[blue[nextindex]]++;
				}
				cLife -= temp.life;
				nextindex = (nextindex + 1) % 5;
				break;
			}
			else
			{
				nextindex = (nextindex + 1) % 5;
				if (nextindex == startindex)
				{
					temp.id = -1;
					break;
				}
			}
		}
		return temp;
	}
};
int main()
{
	int caseNum;
	cin >> caseNum;
	for (int i = 0; i < caseNum; i++)
	{
		int iniLife, d, n, ic, l, w;
		cin >> iniLife >> d >> n >> ic >> l >> w;
		commander r(iniLife, d, n, ic, l, w, 0);
		commander b(iniLife, d, n, ic, l, w, 1);
		bool b1 = true;
		bool b2 = true;
		int time = 0;
		printf("Case:%d\n", i + 1);
		while (b1 || b2)
		{
			if (b1)
			{
				Warrior rw = r.creWarrior();
				if (rw.id == -1)
				{
					b1 = false;
					printf("%03d red headquarter stops making warriors\n", time);
				}
				else
				{
					//000 blue lion 1 born with strength 6,1 lion in blue headquarter
					printf("%03d red %s %d born with strength %d,%d %s in red headquarter\n", time, rw.name.c_str(), rw.id, rw.life, r.wnum[rw.type], rw.name.c_str());
				}
			}
			if (b2)
			{
				Warrior bw = b.creWarrior();
				if (bw.id == -1)
				{
					b2 = false;
					printf("%03d blue headquarter stops making warriors\n", time);
				}
				else
				{
					//000 blue lion 1 born with strength 6,1 lion in blue headquarter
					printf("%03d blue %s %d born with strength %d,%d %s in blue headquarter\n", time,bw.name.c_str(), bw.id, bw.life, b.wnum[bw.type], bw.name.c_str());
				}
			}
			time++;
		}
		
	}
	return 0;
}