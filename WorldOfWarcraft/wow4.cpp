#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<algorithm>
vector<string> wea{ "sword","bomb","arrow" };


struct Weapon {
    int type;      // 0: sword, 1: bomb, 2: arrow
    int endurance; // 耐久度：arrow初始为2，其余为1（或者不计）
    int force;
    int times;
    Weapon(int t,int ownerforce) : type(t)
	{
		if(t==0)
        {
            force=ownerforce*2/10;
        }
        else if(t==2)
        {
            times=3;
        }
        else
        {
            force=0;
        }
	}
    bool operator<(const Weapon& other) const {
        if (type != other.type) {
            return type < other.type; // 编号小的排前面
        }
        if (type == 2) { // 都是 arrow
            return endurance > other.endurance; // 用过的（1）排在没用过的（2)后
        }
        return false;
    }
    bool isbroken()
    {
        if(type==0)
        {
            return force<=0;
        }
        else
        {
            if(type==2)
            {
                return times<=0;
            }
        }
    }
    void dull()
    {
        if(type==0)
        {
            force=force*8/10;
        }
    }
};
class Warrior {
public:
	int id;
	int life;
	int attactpower;
	string name;        // 0:dragon, 1:ninja, 2:iceman, 3:lion, 4:wolf
	int color;
	int type;
	int pos;
	int citynum;
	int weaponnum=0;
	int curWeaponIdx=0;
    int stepcount=0;
	vector<Weapon> weapon;
	Warrior() {};
	Warrior(int Life,int attact,string Name,int Color,int Type)
	{
		life = Life;
		attactpower=attact;
		name = Name;
		color = Color;
		type = Type;
		if(color==0)
		{
			pos=0;
		}
		else
		{
			pos=citynum+1;
		}
	}
    bool shoulddiewithbomb()
    {

        return false;
    }
    int getsordforce()
    {
        for(auto w:weapon)
        {
            if(w.type==0)
            {
                return w.force;
            }
        }
    }
    void useweapon()
    {
        for(auto it=weapon.begin();it!=weapon.end();it++)
        {
            if(it->type==0)
            {
                it->dull();
                if(it->isbroken())
                {
                    it=weapon.erase(it);
                    continue;
                }
            }
            it++;
        }
    }
	void sortWeapon()
	{
		sort(weapon.begin(), weapon.end(), [](const Weapon& a, const Weapon& b) {if (a.type != b.type) {
            return a.type < b.type; // 种类 0 < 1 < 2
        }
            return a.endurance < b.endurance;});
		curWeaponIdx=0;
	}
	int gethurt(Weapon& w)
	{
		if(w.type==0)
		{
			return attactpower*2/10;
		}
		if(w.type==1)
		{
			if(w.endurance==1)
			{
				w.endurance=0;
				if(type!=1)
				{
					life-=attactpower*2/10;
				}
				return attactpower*4/10;
		    }
			else
			{
				return -1;
			}
		}
		if(w.type==2)
		{
			if(w.endurance>0)
			{
				w.endurance-=1;
				return attactpower*3/10;
			}
			else{
				return -1;
			}

		}
	}
	int fight(Warrior& enemy)
	{
		if(weapon.empty())
		{
			return life>0;
		}
		curWeaponIdx%=weapon.size();
		Weapon& w=weapon[curWeaponIdx];
		int damage=gethurt(w);
		if(damage!=-1)
		{
			enemy.life-=damage;
		}
		if(w.type==1||(w.type==2&&w.endurance==0))
		{
			weapon.erase(weapon.begin()+curWeaponIdx);
		}
		else
		{
			curWeaponIdx++;
		}
		return life;
	}
	void getLoot(Warrior* w)
	{
		if(w->weapon.empty())
		{
			return;
		}
		sort(w->weapon.begin(), w->weapon.end(), [](const Weapon& a, const Weapon& b) {if (a.type != b.type) {
            return a.type < b.type; // 种类 0 < 1 < 2
        }
            return a.endurance > b.endurance;});
		for(int i=0;i<w->weapon.size();i++)
		{
			if(weapon.size()<10)
			{
				weapon.push_back(w->weapon[i]);
			}
			else{
				break;
			}
		}	
	}
	virtual void grabWeaponforwolf(Warrior& w,int cityID,int hour,int minute){}
	virtual bool runLion()
	{
		return false;
	}
	virtual void moveForward()
	{
		if(color==0&&pos<citynum+1)
		{
			pos++;
			if(type==2)
			{
				life-=life/10;
				//printf("DEBUG: Iceman moved and lost blood!\n");
			}
		}
		else if(color==1&&pos>0)
		{
			pos--;
			if(type==2)
			{
				life-=life/10;
				//printf("DEBUG: Iceman moved and lost blood!\n");
			}
		}
	}
	virtual void setAttr(int reLife,int redu) {}
	virtual void outputAttr() {}
};
class Dragon :public Warrior
{
public:
	float morale;
	Dragon() {};
	Dragon(int Life,int attact ,int color) :Warrior(Life, attact,"dragon", color, 0)
	{
	}
	void setAttr(int reLife,int redu)
	{
		weapon.push_back(Weapon{id%3,attactpower});
		morale = float(reLife) / life;
	}
	void outputAttr()
	{
		//printf("It has a %s,and it's morale is %.2f\n", wea[weapon].c_str(),morale);
	}
};
class Ninja :public Warrior
{
public:
	Ninja() {};
	Ninja(int Life,int attact, int color) :Warrior(Life, attact,"ninja", color, 1)
	{
	}
	void setAttr(int reLife,int redu)
	{
		weapon.push_back(Weapon{id%3,attactpower});
		weapon.push_back(Weapon{(id+1)%3,attactpower});
	}
	void outputAttr()
	{
		
		//printf("It has a %s and a %s\n",wea[weapon[0]].c_str(),wea[weapon[1]].c_str());
	}
};
class Iceman :public Warrior
{
public:
	Iceman() {};
	Iceman(int Life,int attact,  int color) :Warrior(Life,attact,  "iceman", color, 2)
	{
	}
	void setAttr(int reLife,int redu)
	{
		weapon.push_back(Weapon{id%3,attactpower});
	}
	void outputAttr()
	{
		//printf("It has a %s\n",wea[weapon].c_str());
	}
	// void moveForward()
	// {
	// 	if(color==0&&pos<citynum+1)
	// 	{
	// 		pos++;
	// 		life=life*9/10;
	// 	}
	// 	else if(color==1&&pos>0)
	// 	{
	// 		pos--;
	// 		life=life*9/10;
	// 	}
	// }
};
class Lion :public Warrior
{
public:
	int loyalty;
	int reduLoyalty;
	Lion() {};
	Lion(int Life, int attact, int color) :Warrior(Life,attact,  "lion", color, 3)
	{
	}
	void setAttr(int reLife,int redu)
	{
		weapon.push_back(Weapon{id%3,attactpower});
		loyalty = reLife;
		reduLoyalty=redu;
	}
	void outputAttr()
	{
		printf("Its loyalty is %d\n", loyalty);
	}
	bool runLion()
	{
		if(color==0&&pos<citynum+1&&loyalty<=0)
		{
			return true;
		}
		else if(color==1&&pos>0&&loyalty<=0)
		{
			return true;
		}
		else{
			return false;
		}
	}
	void moveForward()
	{
		if(color==0&&pos<citynum+1)
		{
			pos++;
			loyalty-=reduLoyalty;
		}
		else if(color==1&&pos>0)
		{
			pos--;
			loyalty-=reduLoyalty;
		}
	}
};
class Wolf :public Warrior
{
public:
	Wolf() {};
	Wolf(int Life, int attact, int color) :Warrior(Life, attact, "wolf", color, 4) 
	{
	};
	void grabWeaponforwolf(Warrior& w,int cityID,int hour,int minute) {
    // 如果敌人也是 wolf，或者敌人根本没有武器，直接返回
    if (w.type == 4 || w.weapon.empty()) {
        return;
    }
    // 1. 找出敌人手中最小的武器编号
    int minType = 3; 
    for (int i = 0; i < w.weapon.size(); ++i) {
        if (w.weapon[i].type < minType) {
            minType = w.weapon[i].type;
        }
    }
    // 2. 将敌人手中所有该编号的武器提取出来
    vector<Weapon> targetWeapons;
    for (auto it = w.weapon.begin(); it != w.weapon.end(); ) {
        if (it->type == minType) {
            targetWeapons.push_back(*it);
            it = w.weapon.erase(it); // 从敌人手中移除
        } else {
            ++it;
        }
    }
    // 3. 如果抢的是 arrow (type 2)，根据题目要求：优先抢没用过的 (endurance=2)
    if (minType == 2) {
        // 使用自定义比较：耐久度大的排前面
        sort(targetWeapons.begin(), targetWeapons.end(), [](const Weapon& a, const Weapon& b) {
            return a.endurance > b.endurance;
        });
    }
	int getweaponnum=0;
    // 4. 开始抢夺，直到 Wolf 满了 (10件) 或者抢光了
    for (int i=0; i < targetWeapons.size(); ++i) {
        if (this->weapon.size() < 10) {
            this->weapon.push_back(targetWeapons[i]);
			getweaponnum++;
        } else {
            // 如果 Wolf 已经满了，剩下的还给敌人
            w.weapon.push_back(targetWeapons[i]);
        }
    }
	if(getweaponnum!=0)
	{
		if(color==0)
		{
			printf("%03d:%02d red wolf %d took %d %s from blue %s %d in city %d\n",hour,minute,id,getweaponnum,wea[minType].c_str(),w.name.c_str(),w.id,cityID);
		}
		else
		{
			printf("%03d:%02d blue wolf %d took %d %s from red %s %d in city %d\n",hour,minute,id,getweaponnum,wea[minType].c_str(),w.name.c_str(),w.id,cityID);
		}
	}
}
};
class City
{
	public:
	Warrior* red=nullptr;
	Warrior* blue=nullptr;
	void wolfTakeWeapon(int cityid,int hour,int minute)
	{
		if(red&&blue)
		{
			if(red->type==4&&blue->type!=4)
			{
				red->grabWeaponforwolf(*blue,cityid,hour,minute);
			}
			else if(red->type!=4&&blue->type==4)
			{
				blue->grabWeaponforwolf(*red,cityid,hour,minute);
			}
		}
	}
	void battle(int cityid,int hour,int minute)
	{
		if(!red||!blue)
		{
			return;
		}
		Warrior* attacter=nullptr;
		Warrior* defender=nullptr;
		if(cityid%2==1)
		{
			attacter=red;
			defender=blue;
		}
		else
		{
			attacter=blue;
			defender=red;
		}
		attacter->sortWeapon();
		defender->sortWeapon();
		while(true)
		{
			int oldAttaclife=attacter->life;
			int oldDefenlife=defender->life;
			int oldAttacNum=attacter->weapon.size();
			int oldDefenNum=defender->weapon.size();
			attacter->fight(*defender);
			if(attacter->life<=0||defender->life<=0)
			{
				break;
			}
			defender->fight(*attacter);
			if(attacter->life<=0||defender->life<=0)
			{
				break;
			}
			bool attahasnoendu=true;
			bool defehasnoendu=true;
			for(int i=0;i<attacter->weapon.size();i++)
			{
				if(attacter->weapon[i].type!=0&&attacter->weapon[i].endurance>0)
				{
					attahasnoendu=false;
				}
			}
			for(int i=0;i<defender->weapon.size();i++)
			{
				if(defender->weapon[i].type!=0&&defender->weapon[i].endurance>0)
				{
					defehasnoendu=false;
				}
			}
			if(attacter->life==oldAttaclife&&defender->life==oldDefenlife&&attacter->weapon.size()==oldAttacNum&&defender->weapon.size()==oldDefenNum&&attahasnoendu&&defehasnoendu)
			{
				break;
			}
		}
		if(red->life<=0&&blue->life>0)
		{
			blue->getLoot(red);
			printf("%03d:%02d blue %s %d killed red %s %d in city %d remaining %d elements\n",hour,minute,blue->name.c_str(),blue->id,red->name.c_str(),red->id,cityid,blue->life);
			if(blue->type==0)
			{
				printf("%03d:%02d blue dragon %d yelled in city %d\n",hour,minute,blue->id,cityid);
			}
			delete red;
			red=nullptr;
		}
		else if(red->life>0&&blue->life<=0)
		{
			red->getLoot(blue);
			printf("%03d:%02d red %s %d killed blue %s %d in city %d remaining %d elements\n",hour,minute,red->name.c_str(),red->id,blue->name.c_str(),blue->id,cityid,red->life);
			if(red->type==0)
			{
				printf("%03d:%02d red dragon %d yelled in city %d\n",hour,minute,red->id,cityid);
			}
			delete blue;
			blue=nullptr;
		}
		else if(red->life<=0&&blue->life<=0)
		{
			printf("%03d:%02d both red %s %d and blue %s %d died in city %d\n",hour,minute,red->name.c_str(),red->id,blue->name.c_str(),blue->id,cityid);
			delete red;
			delete blue;
			red=nullptr;
			blue=nullptr;
		}
		else if(red->life>0&&blue->life>0)
		{
			printf("%03d:%02d both red %s %d and blue %s %d were alive in city %d\n",hour,minute,red->name.c_str(),red->id,blue->name.c_str(),blue->id,cityid);
			if(red->type==0)
			{
				printf("%03d:%02d red dragon %d yelled in city %d\n",hour,minute,red->id,cityid);
			}
			if(blue->type==0)
			{
				printf("%03d:%02d blue dragon %d yelled in city %d\n",hour,minute,blue->id,cityid);
			}

		}
	}
	void outputWarriorCondition(int cityid,int hour,int minute)
	{
		if(red)
		{
			int swords=0;
			int bombs=0;
			int arrows=0;
			for(int i=0;i<red->weapon.size();i++)
			{
				if(red->weapon[i].type==0)
				{
					swords++;
				}
				else if(red->weapon[i].type==1)
				{
					bombs++;
				}
				else{
					arrows++;
				}
			}
			printf("%03d:%02d red %s %d has %d sword %d bomb %d arrow and %d elements\n",hour,minute,red->name.c_str(),red->id,swords,bombs,arrows,red->life);
		}
		if(blue)
		{
			int swords=0;
			int bombs=0;
			int arrows=0;
			for(int i=0;i<blue->weapon.size();i++)
			{
				if(blue->weapon[i].type==0)
				{
					swords++;
				}
				else if(blue->weapon[i].type==1)
				{
					bombs++;
				}
				else{
					arrows++;
				}
			}
			printf("%03d:%02d blue %s %d has %d sword %d bomb %d arrow and %d elements\n",hour,minute,blue->name.c_str(),blue->id,swords,bombs,arrows,blue->life);
		}
	}
};


class commander
{
public:
	int cLife;
	int color;//0为红方，1为蓝方
	int nextindex;
	int worriorNum;
	int numDragon;
	int numninja;
	int numiceman;
	int numlion;
	int redulion;
	int numwolf;
	vector<int> wnum = { 0,0,0,0,0 };
	vector<int> red = {2,3,4,1,0};
	vector<int> blue = { 3,0,1,2,4 };
	vector<int> warriorLife = { 0,0,0,0,0 };
	vector<int> warriorattact{0,0,0,0,0};
	commander(int l,int dr,int ni,int ice,int li,int wo,int attact_d,int attact_n,int attact_ic,int attact_l,int attact_w,int color,int reduLion):cLife(l),color(color)
	{
		warriorLife = { dr,ni,ice,li,wo };
		warriorattact={attact_d,attact_n,attact_ic,attact_l,attact_w};
		this->color = color;
		nextindex = 0;
		worriorNum = 0;
		redulion=reduLion;
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
					temp = new Dragon(warriorLife[0],warriorattact[0],0);
					break;
				case 1:
					temp = new Ninja(warriorLife[1],warriorattact[1], 0);
					break;
				case 2:
					temp = new Iceman(warriorLife[2],warriorattact[2], 0);
					break;
				case 3:
					temp = new Lion(warriorLife[3], warriorattact[3],0);
					break;
				case 4:
					temp = new Wolf(warriorLife[4],warriorattact[4], 0);
					break;
				}
			}
			else
			{
				switch (blue[nextindex])
				{
				case 0:
					temp = new Dragon(warriorLife[0],warriorattact[0], 1);
					break;
				case 1:
					temp = new Ninja(warriorLife[1],warriorattact[1], 1);
					break;
				case 2:
					temp = new Iceman(warriorLife[2], warriorattact[2],1);
					break;
				case 3:
					temp = new Lion(warriorLife[3],warriorattact[3], 1);
					break;
				case 4:
					temp = new Wolf(warriorLife[4],warriorattact[4], 1);
					break;
				};
			}
			if (temp->life <= cLife)
			{
				cLife -= temp->life;
				worriorNum++;
				temp->id = worriorNum;
				temp->setAttr(cLife,redulion);
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
				//nextindex = (nextindex + 1) % 5;
				// if (nextindex == startindex)
				// {
				// 	temp->id = -1;
				// 	break;
				// }
				temp->id=-1;
				break;
			}
		}
		return temp;
	}
};
int main()
{
	int caseNum;
	int citynum;
	cin >> caseNum;
	for (int i = 0; i < caseNum; i++)
	{
		printf("Case %d:\n", i + 1);
		int iniLife, d, n, ic, l, w,ad,attact_d,attact_n,attact_ic,attact_l,attact_w,redulion,T;
		cin >> iniLife >>citynum>>redulion>>T >>d>> n >> ic >> l >> w>>attact_d>>attact_n>>attact_ic>>attact_l>>attact_w;
		commander r(iniLife, d, n, ic, l, w,attact_d,attact_n,attact_ic,attact_l,attact_w, 0,redulion);
		commander b(iniLife, d, n, ic, l, w,attact_d,attact_n,attact_ic,attact_l,attact_w, 1,redulion);
		vector<City> cities(citynum+2);
		bool gameover=false;
		bool redcanmake=true;
		bool bluecanmake=true;
		for(int currrentTime=0;currrentTime<=T;currrentTime+=5)
		{
			int hour=currrentTime/60;
			int minute=currrentTime%60;
			if(minute==0)//造人开始
			{
				if(redcanmake)
				{
					Warrior* rw=r.creWarrior();
					if(rw->id==-1)
					{
						redcanmake=false;
					}
					else
					{
						printf("%03d:%02d red %s %d born\n", hour,minute, rw->name.c_str(), rw->id);
						rw->pos=0;
						rw->citynum=citynum;
                		rw->outputAttr();
						cities[0].red=rw;
					}
				}
				if(bluecanmake)
				{
					Warrior* bw=b.creWarrior();
					if(bw->id==-1)
					{
						bluecanmake=false;
					}
					else
					{
						printf("%03d:%02d blue %s %d born\n", hour,minute,bw->name.c_str(), bw->id);
						bw->citynum=citynum;
						bw->pos=citynum+1;
                    	bw->outputAttr();
						cities[citynum+1].blue=bw;
					}
				}
			}
			else if(minute==5)//狮子逃跑
			{
				for(int j=0;j<citynum+2;j++)
				{
					if(cities[j].red&&cities[j].red->type==3&&cities[j].red->runLion())
					{
						printf("%03d:%02d red lion %d ran away\n",hour,minute,cities[j].red->id);
						delete cities[j].red;
						cities[j].red=nullptr;
					}
					if(cities[j].blue&&cities[j].blue->type==3&&cities[j].blue->runLion())
					{
						printf("%03d:%02d blue lion %d ran away\n",hour,minute,cities[j].blue->id);
						delete cities[j].blue;
						cities[j].blue=nullptr;
					}
				}
			}
			else if(minute==10)//武士前进
			{
				vector<Warrior*> nextRed(citynum+2,nullptr);
				vector<Warrior*> nextBlue(citynum+2,nullptr);
				for(int j=0;j<citynum+1;j++)
				{
					if(cities[j].red)
					{
						cities[j].red->moveForward();
						nextRed[j+1]=cities[j].red;
					}
				}
				for(int j=citynum+1;j>0;j--)
				{
					if(cities[j].blue)
					{
						cities[j].blue->moveForward();
						nextBlue[j-1]=cities[j].blue;
					}
				}
				for(int j=0;j<citynum+2;j++)
				{
					cities[j].red=nextRed[j];
					cities[j].blue=nextBlue[j];
					if(cities[j].red)
					{
						if(j==citynum+1)
						{
							printf("%03d:%02d red %s %d reached blue headquarter with %d elements and force %d\n",hour,minute,cities[j].red->name.c_str(),cities[j].red->id,cities[j].red->life,cities[j].red->attactpower);
							printf("%03d:%02d blue headquarter was taken\n",hour,minute);
							gameover=true;
						}
						else
						{
							printf("%03d:%02d red %s %d marched to city %d with %d elements and force %d\n",hour,minute,cities[j].red->name.c_str(),cities[j].red->id,j,cities[j].red->life,cities[j].red->attactpower);
						}
					}
					if(cities[j].blue)
					{
						if(j==0)
						{
							printf("%03d:%02d blue %s %d reached red headquarter with %d elements and force %d\n",hour,minute,cities[j].blue->name.c_str(),cities[j].blue->id,cities[j].blue->life,cities[j].blue->attactpower);
							printf("%03d:%02d red headquarter was taken\n",hour,minute);
							gameover=true;
						}
						else
						{
							printf("%03d:%02d blue %s %d marched to city %d with %d elements and force %d\n",hour,minute,cities[j].blue->name.c_str(),cities[j].blue->id,j,cities[j].blue->life,cities[j].blue->attactpower);
						}
					}

				}

			}
			else if(minute==35)
			{
				for(int j=0;j<citynum+2;j++)
				{
					cities[j].wolfTakeWeapon(j,hour,minute);
				}
			}
			else if(minute==40)
			{
				for(int j=0;j<citynum+2;j++)
				{
					cities[j].battle(j,hour,minute);
				}
			}
			else if(minute==50)
			{
				printf("%03d:50 %d elements in red headquarter\n",hour,r.cLife);
				printf("%03d:50 %d elements in blue headquarter\n",hour,b.cLife);
			}
			else if(minute==55)
			{
				for(int j=0;j<citynum+2;j++)
				{
					cities[j].outputWarriorCondition(j,hour,minute);
				}
			}
			if(gameover)
			{
				break;
			}
		}
		
	}
	return 0;
}