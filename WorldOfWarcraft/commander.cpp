#include<iostream>
using namespace std;
#include<string>
#include<vector>
vector<string> wea{ "sword","bomb","arrow" };
class Warrior {
public:
	int id;
	int life;
	string name;        // 0:dragon, 1:ninja, 2:iceman, 3:lion, 4:wolf
	int color;
	int type;
	Warrior() {};
	Warrior(int Life,string Name,int Color,int Type)
	{
		life = Life;
		name = Name;
		color = Color;
		type = Type;
	}
	virtual void setAttr(int reLife) {};
	virtual void outputAttr() {};
};
class Dragon :public Warrior
{
public:
	int weapon;
	float morale;
	Dragon() {};
	Dragon(int Life, int color) :Warrior(Life, "dragon", color, 0)
	{
	}
	void setAttr(int reLife)
	{
		weapon = id % 3;
		morale = float(reLife) / life;
	}
	void outputAttr()
	{
		printf("It has a %s,and it's morale is %.2f\n", wea[weapon].c_str(),morale);
	}
};
class Ninja :public Warrior
{
public:
	int weapon[2];
	Ninja() {};
	Ninja(int Life, int color) :Warrior(Life, "ninja", color, 1)
	{
	}
	void setAttr(int reLife)
	{
		weapon[0] = id % 3;
		weapon[1] = (id + 1) % 3;
	}
	void outputAttr()
	{
		
		printf("It has a %s and a %s\n",wea[weapon[0]].c_str(),wea[weapon[1]].c_str());
	}
};
class Iceman :public Warrior
{
public:
	int weapon;
	Iceman() {};
	Iceman(int Life, int color) :Warrior(Life, "iceman", color, 2)
	{
	}
	void setAttr(int reLife)
	{
		weapon = id % 3;
	}
	void outputAttr()
	{
		printf("It has a %s\n",wea[weapon].c_str());
	}
};
class Lion :public Warrior
{
public:
	int loyalty;
	Lion() {};
	Lion(int Life, int color) :Warrior(Life, "lion", color, 3)
	{
	}
	void setAttr(int reLife)
	{
		loyalty = reLife;
	}
	void outputAttr()
	{
		printf("It's loyalty is %d\n", loyalty);
	}
};
class Wolf :public Warrior
{
public:
	Wolf() {};
	Wolf(int Life, int color) :Warrior(Life, "wolf", color, 4) 
	{
	};
};



class commander
{
public:
	int cLife;
	int color;//0是红方，1是蓝方
	int nextindex;
	int worriorNum;
	int numDragon;
	int numninja;
	int numiceman;
	int numlion;
	int numwolf;
	vector<int> wnum = { 0,0,0,0,0 };
	vector<int> red = {2,3,4,1,0};
	vector<int> blue = { 3,0,1,2,4 };
	vector<int> warriorLife = { 0,0,0,0,0 };
	commander(int l,int dr,int ni,int ice,int li,int wo,int color):cLife(l),color(color)
	{
		warriorLife = { dr,ni,ice,li,wo };
		this->color = color;
		nextindex = 0;
		worriorNum = 0;
	}
	Warrior* creWarrior()
	{
		int startindex = nextindex;
		Warrior* temp=NULL;
		while (1)
		{
			if (color == 0)
			{
				switch (red[nextindex])
				{
				case 0:
					temp = new Dragon(warriorLife[0],0);
					break;
				case 1:
					temp = new Ninja(warriorLife[1], 0);
					break;
				case 2:
					temp = new Iceman(warriorLife[2], 0);
					break;
				case 3:
					temp = new Lion(warriorLife[3], 0);
					break;
				case 4:
					temp = new Wolf(warriorLife[4], 0);
					break;
				}
			}
			else
			{
				switch (blue[nextindex])
				{
				case 0:
					temp = new Dragon(warriorLife[0], 1);
					break;
				case 1:
					temp = new Ninja(warriorLife[1], 1);
					break;
				case 2:
					temp = new Iceman(warriorLife[2], 1);
					break;
				case 3:
					temp = new Lion(warriorLife[3], 1);
					break;
				case 4:
					temp = new Wolf(warriorLife[4], 1);
					break;
				};
			}
			if (temp->life <= cLife)
			{
				cLife -= temp->life;
				worriorNum++;
				temp->id = worriorNum;
				temp->setAttr(cLife);
				if (color == 0)
				{
					wnum[red[nextindex]]++;
				}
				else
				{
					wnum[blue[nextindex]]++;
				}
				nextindex = (nextindex + 1) % 5;
				break;
			}
			else
			{
				nextindex = (nextindex + 1) % 5;
				if (nextindex == startindex)
				{
					temp->id = -1;
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
				Warrior* rw = r.creWarrior();
				if (rw->id == -1)
				{
					b1 = false;
					printf("%03d red headquarter stops making warriors\n", time);
				}
				else
				{
					//000 blue lion 1 born with strength 6,1 lion in blue headquarter
					printf("%03d red %s %d born with strength %d,%d %s in red headquarter\n", time, rw->name.c_str(), rw->id, rw->life, r.wnum[rw->type], rw->name.c_str());
					rw->outputAttr();
					delete rw;
				}
			}
			if (b2)
			{
				Warrior* bw = b.creWarrior();
				if (bw->id == -1)
				{
					b2 = false;
					printf("%03d blue headquarter stops making warriors\n", time);
				}
				else
				{
					//000 blue lion 1 born with strength 6,1 lion in blue headquarter
					printf("%03d blue %s %d born with strength %d,%d %s in blue headquarter\n", time,bw->name.c_str(), bw->id, bw->life, b.wnum[bw->type], bw->name.c_str());
					bw->outputAttr();
					delete bw;
				}
			}
			time++;
		}
		
	}
	return 0;
}