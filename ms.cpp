#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;
int t, M, N, R, K, T;
int hp[5], strength[5];
int com_create[2][5] = {{3,0,1,2,4}, {2,3,4,1,0}};
class monster;

//时间类
class mytime
{
  public:
    int hour, minute;
    void refresh()
    {
        hour = minute = 0;
    }
    void get_time()
    {
        printf("%03d:%02d ", hour, minute);
    }
    bool is_end(int n)
    {
        minute += n;
        if (minute >= 60)
        {
            minute = 0;
            hour += 1;
        }
        if ((hour * 60 + minute) > T)
            return true;
        else
            return false;
    }
} Mytime;

//司令部
class command
{
  public:
    int command_hp, num, id, next; //生命元、到达数量，制造id
    monster *temp;
    monster *store; //暂存前进武士
    //循环开始用于更新的函数
    void refresh()
    {
        command_hp = M;
        num = 0;
        id = 1;
        next = 0;
        store = NULL;
    }
} commands[2]; //0红 1蓝

//武器
class weapon
{
  public:
    int attack;
    weapon(int _attack = 0) : attack(_attack) {}
    virtual void info(){};
    virtual int get_attack(){
        return 0;
    };
    virtual bool whether_broken(){
        return false;
    };
};

class sword : public weapon
{
  public:
    sword(int _attack) : weapon(_attack) {}
    int get_attack()
    {
        int temp = attack;
        attack = int(temp * 0.8);
        return temp;
    }
    void info()
    {
        printf("sword(%d)", attack);
    }

    bool whether_broken()
    {
        if (attack <= 0)
            return true;
        else
            return false;
    }
};

class arrow : public weapon
{
  public:
    int time;
    arrow(int _attack) : weapon(_attack)
    {
        time = 3;
    }
    int get_attack()
    {
        time -= 1;
        return attack;
    }
    void info()
    {
        printf("arrow(%d)", time);
    }

    bool whether_broken()
    {
        if (time <= 0)
            return true;
        else
            return false;
    }
};

class bomb : public weapon
{
  public:
    bool used;
    bomb() : weapon(0)
    {
        used = false;
    }
    void info()
    {
        printf("bomb");
    }
    int get_attack()
    {
        return 0;
    }
    bool whether_broken()
    {
        return used;
    }
};

class monster
{
  public:
    int monster_hp, monster_id, monster_attack, loyalty;
    bool colour, win;
    bool arrow_kill;
    string name;
    weapon *weapons[3];
    monster(bool _colour, int _monster_hp, int _monster_id, int _monster_attack)
    {
        colour = _colour;
        monster_hp = _monster_hp;
        monster_id = _monster_id;
        monster_attack = _monster_attack;
        loyalty = 0;
        for (int i = 0; i < 3; i++)
        {
            weapons[i] = NULL;
        }
        arrow_kill = false;
    }
    void win_hp(int n)
    {
        Mytime.get_time();
        info();
        printf("earned %d elements for his headquarter\n", n);
    }
    void die(int n)
    {
        if (arrow_kill)
            return;
        Mytime.get_time();
        info();
        printf("was killed in city %d\n", n);
    }

    bool whether_die()
    {
        if (monster_hp <= 0)
            return true;
        else
            return false;
    }

    int get_sword_attack(){
        int temp = 0;
        if(weapons[0]){
            temp += weapons[0] -> get_attack();
            if(weapons[0] -> whether_broken()){
                delete weapons[0];
                weapons[0] = NULL;
            }
        }
        return temp;
    }

    int get_arrow_attack(){
        int temp = 0;
        if(weapons[2]){
            temp += weapons[2] -> get_attack();
            if(weapons[2] -> whether_broken()){
                delete weapons[2];
                weapons[2] = NULL;
            }
        }
        return temp;
    }


    virtual void strike(monster *aim, int n)
    {
        int aim_hp = aim->monster_hp;
        aim->monster_hp -= monster_attack + get_sword_attack();
        Mytime.get_time();
        info();
        cout << "attacked ";
        aim->info();
        printf("in city %d ", n);
        info_s();
        if (aim->monster_hp > 0)
            aim->strike_back(this, n);
        else
        {
            //杀死狮子
            if (aim->name[0] == 'l')
                monster_hp += aim_hp;
        }
    }

    virtual void strike_back(monster *aim, int n)
    {
        int aim_hp = aim->monster_hp;
        aim->monster_hp -= monster_attack / 2 + get_sword_attack();
        Mytime.get_time();
        info();
        cout << "fought back against ";
        aim->info();
        printf("in city %d\n", n);
        //反击杀死狮子
        if (aim->whether_die() && aim->name[0] == 'l')
        {
            monster_hp += aim_hp;
        }
    }

    //名称
    void info()
    {
        if (colour)
            printf("red ");
        else
            printf("blue ");
        cout << name << " " << monster_id << " ";
    }

    //力量武器
    void info_s()
    {
        printf("with %d elements and force %d\n", monster_hp, monster_attack);
    }

    //武器信息
    void info_w()
    {
        int report_num = 0;
        Mytime.get_time();
        info();
        cout << "has ";
        if (weapons[2] != NULL)
        {
            weapons[2]->info();
            report_num++;
        }
        if (weapons[1] != NULL)
        {
            if (report_num)
            {
                cout << ",";
            }
            weapons[1]->info();
            report_num++;
        }
        if (weapons[0] != NULL)
        {
            if (weapons[0]->attack > 0)
            {
                if (report_num)
                {
                    cout << ",";
                }
                weapons[0]->info();
                report_num++;
            }
        }
        if (report_num == 0)
        {
            cout << "no weapon";
        }
        cout << endl;
    }

    void use_arrow(monster *aim)
    {
        aim->monster_hp -= get_arrow_attack();
        if (aim->monster_hp <= 0)
        {
            Mytime.get_time();
            info();
            cout << "shot and killed ";
            aim->info();
            cout << endl;
            aim->arrow_kill = true;
        }
        else
        {
            Mytime.get_time();
            info();
            cout << "shot" << endl;
        }
    }

    virtual void add_step() {}
    virtual void yell(int) {}

    //需要知道是否先手
    bool use_bomb(monster *aim, bool first_hand)
    {
        if (first_hand)
        {   
            if(aim -> name[0] == 'n')
                return false;
            
            int temp = 0;
            if (weapons[0])
            {
                temp += weapons[0]->attack;
            }
            if (temp + monster_attack >= aim->monster_hp)
            {
                return false;
            }

            temp = 0;
            if (aim->weapons[0])
            {
                temp = aim->weapons[0]->attack;
            }
            if (monster_hp <= aim->monster_attack / 2 + temp)
            {
                Mytime.get_time();
                info();
                cout << "used a bomb and killed ";
                aim->info();
                cout << endl;
                return true;
            }
        }
        else
        {   
            int temp = 0;
            if (aim->weapons[0])
            {
                temp = aim->weapons[0]->attack;
            }
            if (monster_hp <= aim->monster_attack + temp)
            {
                Mytime.get_time();
                info();
                cout << "used a bomb and killed ";
                aim->info();
                cout << endl;
                return true;
            }
        }
        return false;
    }
};

class wolf : public monster
{
  public:
    wolf(bool _colour, int _monster_id) : monster(_colour, hp[4], _monster_id, strength[4])
    {
        name = "wolf";
        Mytime.get_time();
        info();
        cout << "born" << endl;
    }
    void add_step() {}

    void strike(monster *aim, int n)
    {
        if (whether_die())
        {
            return;
        }

        if (!aim->whether_die())
        {
            int aim_hp = aim->monster_hp;
            int temp = 0;
            if (weapons[0])
            {
                temp += weapons[0]->get_attack();
            }
            aim->monster_hp -= monster_attack + temp;
            if (weapons[0] && weapons[0]->whether_broken())
            {
                delete weapons[0];
                weapons[0] = NULL;
            }
            Mytime.get_time();
            info();
            cout << "attacked ";
            aim->info();
            printf("in city %d ", n);
            info_s();
            if (aim->monster_hp > 0)
                aim->strike_back(this, n);
            else
            {
                //杀死狮子
                if (aim->name[0] == 'l')
                    monster_hp += aim_hp;
            }
        }

        if (!whether_die() && aim->whether_die())
        {
            for (int i = 0; i < 3; i++)
            {
                if (weapons[i] == NULL && aim->weapons[i] != NULL)
                {
                    weapons[i] = aim->weapons[i];
                    aim->weapons[i] = NULL;
                }
            }
        }
    }

    void strike_back(monster *aim, int n)
    {
        int aim_hp = aim->monster_hp;
        int temp = 0;
        if (weapons[0])
        {
            temp += weapons[0]->get_attack();
        }
        aim->monster_hp -= monster_attack / 2 + temp;
        if (weapons[0] && weapons[0]->whether_broken())
        {
            delete weapons[0];
            weapons[0] = NULL;
        }
        Mytime.get_time();
        info();
        cout << "fought back against ";
        aim->info();
        printf("in city %d\n", n);
        //反击杀死狮子
        if (aim->whether_die() && aim->name[0] == 'l')
        {
            monster_hp += aim_hp;
        }

        if (aim->whether_die())
        {
            for (int i = 0; i < 3; i++)
            {
                if (!weapons[i] && aim->weapons[i])
                {
                    weapons[i] = aim->weapons[i];
                    aim->weapons[i] = NULL;
                }
            }
        }
    }
};

class lion : public monster
{
  public:
    lion(bool _colour, int _monster_id) : monster(_colour, hp[3], _monster_id, strength[3])
    {
        name = "lion";
        loyalty = commands[_colour].command_hp;
        Mytime.get_time();
        info();
        cout << "born" << endl;
        printf("Its loyalty is %d\n", loyalty);
    }
    void add_step() {}
    void strike(monster *aim, int n)
    {
        if (whether_die() || aim->whether_die())
        {
            return;
        }
        int aim_hp = aim->monster_hp;
        int temp = 0;
        if (weapons[0])
        {
            temp += weapons[0]->get_attack();
        }
        aim->monster_hp -= monster_attack + temp;
        if (weapons[0] && weapons[0]->whether_broken())
        {
            delete weapons[0];
            weapons[0] = NULL;
        }
        Mytime.get_time();
        info();
        cout << "attacked ";
        aim->info();
        printf("in city %d ", n);
        info_s();
        if (aim->monster_hp > 0)
        {
            aim->strike_back(this, n);
            loyalty -= K;
        }
        else
        {
            //杀死狮子
            if (aim->name[0] == 'l')
                monster_hp += aim_hp;
        }
    }

    void strike_back(monster *aim, int n)
    {
        int aim_hp = aim->monster_hp;
        int temp = 0;
        if (weapons[0])
        {
            temp += weapons[0]->get_attack();
        }
        aim->monster_hp -= monster_attack / 2 + temp;
        if (weapons[0] && weapons[0]->whether_broken())
        {
            delete weapons[0];
            weapons[0] = NULL;
        }
        Mytime.get_time();
        info();
        cout << "fought back against ";
        aim->info();
        printf("in city %d\n", n);
        //反击杀死狮子
        if (aim->whether_die() && aim->name[0] == 'l')
        {
            monster_hp += aim_hp;
        }
        if (!aim->whether_die())
        {
            loyalty -= K;
        }
    }
};

class iceman : public monster
{
  public:
    int step;
    iceman(bool _colour, int _monster_id) : monster(_colour, hp[2], _monster_id, strength[2])
    {
        name = "iceman";
        step = 1;
        int temp = _monster_id % 3;
        if (temp == 2)
        {
            //arrow temp_arrow(R);
            weapons[temp] = new arrow(R);
        }
        else if (temp == 1)
        {
            weapons[temp] = new bomb();
        }
        else
        {
            int temp_ = int(strength[2] * 0.2);
            if (temp_ != 0)
                weapons[temp] = new sword(temp_);
        }
        Mytime.get_time();
        info();
        cout << "born" << endl;
    }
    void add_step()
    {
        if ((++step) % 2)
        {
            if (monster_hp <= 9)
            {
                monster_hp = 1;
            }
            else
                monster_hp -= 9;

            monster_attack += 20;
        }
    }
};

class ninja : public monster
{
  public:
    ninja(bool _colour, int _monster_id) : monster(_colour, hp[1], _monster_id, strength[1])
    {
        name = "ninja";

        int temp1, temp2;
        temp1 = _monster_id % 3;
        temp2 = (_monster_id + 1) % 3;
        if (temp1 == 2)
            weapons[temp1] = new arrow(R);
        else if (temp1 == 1)
            weapons[temp1] = new bomb();
        else{
            int temp_ = int(strength[1] * 0.2);
            if (temp_ != 0)
                weapons[temp1] = new sword(temp_);
        }

        if (temp2 == 2)
            weapons[temp2] = new arrow(R);
        else if (temp2 == 1)
            weapons[temp2] = new bomb();
        else{
            int temp_ = int(strength[1] * 0.2);
            if (temp_ != 0)
                weapons[temp2] = new sword(temp_);
        }


        //输出生成信息
        Mytime.get_time();
        info();
        cout << "born" << endl;
    }

    void strike_back(monster *aim, int n)
    {
        return;
    }
};

class dragon : public monster
{
  public:
    double morale;
    dragon(bool _colour, int _monster_id) : monster(_colour, hp[0], _monster_id, strength[0])
    {
        name = "dragon";

        morale = (commands[_colour].command_hp) / double(hp[0] + 0.0);
        // if(_colour)
        //     morale = (commands[0].command_hp - hp[0]) / hp[0];
        // else
        //     morale = (commands[1].command_hp - hp[0]) / hp[0];

        int temp;
        temp = _monster_id % 3;
        if (temp == 2)
        {
            //arrow temp_arrow(R);
            weapons[temp] = new arrow(R);
        }
        else if (temp == 1)
        {
            weapons[temp] = new bomb();
        }
        else
        {
            int temp_ = int(strength[0] * 0.2);
            if(temp_ != 0)   
                weapons[temp] = new sword(temp_);
        }
        Mytime.get_time();
        info();
        cout << "born" << endl;
        printf("Its morale is %.2f\n", morale);
    }

    void strike(monster *aim, int n)
    {
        if (whether_die())
        {
            return;
        }

        if (!aim->whether_die())
        {
            int aim_hp = aim->monster_hp;
            int temp = 0;
            if (weapons[0])
            {
                temp += weapons[0]->get_attack();
            }
            aim->monster_hp -= monster_attack + temp;
            if (weapons[0] && weapons[0]->whether_broken())
            {
                delete weapons[0];
                weapons[0] = NULL;
            }
            Mytime.get_time();
            info();
            cout << "attacked ";
            aim->info();
            printf("in city %d ", n);
            info_s();
            if (aim->monster_hp > 0)
            {
                aim->strike_back(this, n);
                morale -= 0.2;
            }
            else
            {
                //杀死狮子
                if (aim->name[0] == 'l')
                    monster_hp += aim_hp;
                morale += 0.2;
            }
        }

        //判断是否欢呼（因为自己也有可能被反击致死
        
    }

    void strike_back(monster *aim, int n)
    {
        int aim_hp = aim->monster_hp;
        int temp = 0;
        if (weapons[0])
        {
            temp += weapons[0]->get_attack();
        }
        aim->monster_hp -= monster_attack / 2 + temp;
        if (weapons[0] && weapons[0]->whether_broken())
        {
            delete weapons[0];
            weapons[0] = NULL;
        }
        Mytime.get_time();
        info();
        cout << "fought back against ";
        aim->info();
        printf("in city %d\n", n);
        //反击杀死狮子
        if (aim->whether_die() && aim->name[0] == 'l')
        {
            monster_hp += aim_hp;
        }
        if (aim->whether_die())
        {
            morale += 0.2;
        }
        else
        {
            morale -= 0.2;
        }
    }

    void yell(int n){
        if (morale >= 0.8 && monster_hp >= 0)
        {
            Mytime.get_time();
            info();
            printf("yelled in city %d\n", n);
        }
    }
};

//城市类
class city
{
  public:
    int city_hp, last_win;
    bool flag; //true红， false 蓝，编号为奇数红旗，编号为偶数蓝旗
    monster *city_monsters[2];
    bool original_flag;
    int red_flag, blue_flag;
    int city_id;
    city()
    {
        city_hp = 0;
        city_monsters[0] = NULL;
        city_monsters[1] = NULL;
        red_flag = 0;
        blue_flag = 0;
        original_flag = true;
        last_win = -1;
        city_id = 0;
    }

    void march(int n)
    {
        if (city_monsters[1])
        {
            Mytime.get_time();
            city_monsters[1]->info();
            printf("marched to city %d ", n);
            city_monsters[1]->info_s();
        }
        if (city_monsters[0])
        {
            Mytime.get_time();
            city_monsters[0]->info();
            printf("marched to city %d ", n);
            city_monsters[0]->info_s();
        }
    }

    void get_flag(int i)
    {
        if (i % 2)
            flag = true;
        else
            flag = false;
        city_id = i;
    }
    
    void win(int n){
        city_monsters[1-n] -> die(city_id);
        if(city_monsters[n] -> name[0] == 'w'){
            for(int i = 0; i < 3; i++){
                if(city_monsters[n] -> weapons[i] == NULL && city_monsters[1-n] -> weapons[i] != NULL){
                    city_monsters[n] -> weapons[i] = city_monsters[1-n] -> weapons[i];
                    city_monsters[1-n] -> weapons[i] = NULL;
                }
            }
        }

        int flag_ = int(flag);
        if(city_monsters[n] -> name[0] == 'd' && n == flag_){
            city_monsters[n] -> yell(city_id);
        }
        city_monsters[n] -> win_hp(city_hp);
        if(last_win == n){
            if (n == 1 && (!flag || (flag && original_flag)))
            {
                Mytime.get_time();
                //blue_flag = 0;
                flag = true;
                printf("blue flag raised in city %d\n", city_id);
                original_flag = false;
            }
            if (n == 0 && (flag || (!flag && original_flag)))
            {
                    Mytime.get_time();
                    //citys[i].blue_flag = 0;
                    flag = false;
                    printf("blue flag raised in city %d\n", city_id);
                    original_flag = false;
            }
        }
    last_win = n;
    }
};

void create_monster()
{
    int temp = commands[1].next;
    if (temp == 0 && commands[1].command_hp >= hp[2])
    {
        //commands[1].next = (temp + 1) % 5;
        commands[1].command_hp -= hp[2];
        commands[1].temp = new iceman(1, commands[1].id++);
        commands[1].next = (temp + 1) % 5;
    }
    else if (temp == 1 && commands[1].command_hp >= hp[3])
    {
        //commands[1].next = (temp + 1) % 5;
        commands[1].command_hp -= hp[3];
        commands[1].temp = new lion(1, commands[1].id++);
        commands[1].next = (temp + 1) % 5;
    }
    else if (temp == 2 && commands[1].command_hp >= hp[4])
    {
        //commands[1].next = (temp + 1) % 5;
        commands[1].command_hp -= hp[4];
        commands[1].temp = new wolf(1, commands[1].id++);
        commands[1].next = (temp + 1) % 5;
    }
    else if (temp == 3 && commands[1].command_hp >= hp[1])
    {
        //commands[1].next = (temp + 1) % 5;
        commands[1].command_hp -= hp[1];
        commands[1].temp = new ninja(1, commands[1].id++);
        commands[1].next = (temp + 1) % 5;
    }
    else if (temp == 4 && commands[1].command_hp >= hp[0])
    {
        //commands[1].next = (temp + 1) % 5;
        commands[1].command_hp -= hp[0];
        commands[1].temp = new dragon(1, commands[1].id++);
        commands[1].next = (temp + 1) % 5;
    }

    temp = commands[0].next;
    if (temp == 0 && commands[0].command_hp >= hp[3])
    {
        commands[0].command_hp -= hp[3];
        commands[0].temp = new lion(0, commands[0].id++);
        commands[0].next = (temp + 1) % 5;
    }
    else if (temp == 1 && commands[0].command_hp >= hp[0])
    {
        commands[0].command_hp -= hp[0];
        commands[0].temp = new dragon(0, commands[0].id++);
        commands[0].next = (temp + 1) % 5;
    }
    else if (temp == 2 && commands[0].command_hp >= hp[1])
    {
        commands[0].command_hp -= hp[1];
        commands[0].temp = new ninja(0, commands[0].id++);
        commands[0].next = (temp + 1) % 5;
    }
    else if (temp == 3 && commands[0].command_hp >= hp[2])
    {
        commands[0].command_hp -= hp[2];
        commands[0].temp = new iceman(0, commands[0].id++);
        commands[0].next = (temp + 1) % 5;
    }
    else if (temp == 4 && commands[0].command_hp >= hp[4])
    {
        commands[0].command_hp -= hp[4];
        commands[0].temp = new wolf(0, commands[0].id++);
        commands[0].next = (temp + 1) % 5;
    }
    //commands[0].next = (temp + 1) % 5;
}

bool whether_run(monster *temp)
{
    if (!temp)
    {
        return false;
    }

    if (temp->name[0] == 'l' && temp->loyalty <= 0)
    {
        Mytime.get_time();
        temp->info();
        cout << "ran away" << endl;
        delete temp;
        return true;
    }
    return false;
}

void run_away(city *citys)
{
    if (whether_run(commands[1].temp))
        commands[1].temp = NULL;
    for (int i = 1; i != N + 1; i++)
    {

        if (whether_run(citys[i].city_monsters[1]))
            citys[i].city_monsters[1] = NULL;
        if (whether_run(citys[i].city_monsters[0]))
            citys[i].city_monsters[0] = NULL;
    }
    if (whether_run(commands[0].temp))
        commands[0].temp = NULL;
}

void forward(city *citys)
{
    bool flag1 = false, flag0 = false;
    for (int i = N; i > 0; i--)
    {
        if (citys[i].city_monsters[1] != NULL)
        {
            if (i == N)
            {
                if (commands[0].store)
                {
                    delete commands[0].store;
                }
                commands[0].store = citys[i].city_monsters[1];
                commands[0].num += 1;
                commands[0].store->add_step();
                flag0 = true;
            }
            else
            {
                citys[i + 1].city_monsters[1] = citys[i].city_monsters[1];
                citys[i + 1].city_monsters[1]->add_step();
            }
            citys[i].city_monsters[1] = NULL;
        }
    }

    if (commands[1].temp != NULL)
    {
        citys[1].city_monsters[1] = commands[1].temp;
        citys[1].city_monsters[1]->add_step();
        commands[1].temp = NULL;
    }

    //蓝色
    for (int i = 1; i <= N; i++)
    {
        if (citys[i].city_monsters[0])
        {
            if (i == 1)
            {
                if (commands[1].store)
                {
                    delete commands[1].store;
                }
                commands[1].store = citys[i].city_monsters[0];
                commands[1].store->add_step();
                commands[1].num += 1;
                flag1 = true;
            }
            else
            {
                citys[i - 1].city_monsters[0] = citys[i].city_monsters[0];
                citys[i - 1].city_monsters[0]->add_step();
            }
            citys[i].city_monsters[0] = NULL;
        }
    }

    if (commands[0].temp)
    {
        citys[N].city_monsters[0] = commands[0].temp;
        citys[N].city_monsters[0]->add_step();
        commands[0].temp = NULL;
    }

    //输出
    if (flag1)
    {
        Mytime.get_time();
        commands[1].store->info();
        cout << "reached red headquarter ";
        commands[1].store->info_s();
        // delete commands[1].store;
        // commands[1].store = NULL;
    }
    if (commands[1].num == 2)
    {
        Mytime.get_time();
        printf("red headquarter was taken\n");
    }

    for (int i = 1; i != N + 1; i++)
    {
        citys[i].march(i);
    }

    if (flag0)
    {
        Mytime.get_time();
        commands[0].store->info();
        cout << "reached blue headquarter ";
        commands[0].store->info_s();
        // delete commands[0].store;
        // commands[0].store = NULL;
    }
    if (commands[0].num == 2)
    {
        Mytime.get_time();
        printf("blue headquarter was taken\n");
    }
    //Mytime.add_time(10); 前进之后不能加上时间
}

//判断是否胜利
bool victory(bool _case = true)
{
    bool flag = false;
    if (commands[1].num == 2)
    {
        flag = true;
    }
    if (commands[0].num == 2)
    {
        flag = true;
    }
    return flag;
}

void create_hp(city *citys)
{
    for (int i = 1; i <= N; i++)
    {
        citys[i].city_hp += 10;
    }
}

void get_hp(city *citys)
{
    for (int i = 1; i <= N; i++)
    {
        if (citys[i].city_hp == 0)
            continue;
        if (citys[i].city_monsters[1] != NULL && citys[i].city_monsters[0] == NULL)
        {
            Mytime.get_time();
            citys[i].city_monsters[1]->info();
            printf("earned %d elements for his headquarter\n", citys[i].city_hp);
            commands[1].command_hp += citys[i].city_hp;
            citys[i].city_hp = 0;
        }
        if (citys[i].city_monsters[0] != NULL && citys[i].city_monsters[1] == NULL)
        {
            Mytime.get_time();
            citys[i].city_monsters[0]->info();
            printf("earned %d elements for his headquarter\n", citys[i].city_hp);
            commands[0].command_hp += citys[i].city_hp;
            citys[i].city_hp = 0;
        }
    }
}

void command_report()
{
    for (int i = 1; i >= 0; i--)
    {
        Mytime.get_time();
        printf("%d elements in ", commands[i].command_hp);
        if (i)
            cout << "red ";
        else
            cout << "blue ";
        cout << "headquarter" << endl;
    }
}

void monster_report(city *citys)
{
    for (int j = 1; j >= 0; j--)
    {
        if (j == 0 && commands[1].store)
        {
            commands[1].store->info_w();
        }
        for (int i = 1; i <= N; i++)
        {
            if (citys[i].city_monsters[j])
            {
                citys[i].city_monsters[j]->info_w();
            }
        }
        if (j == 1 && commands[0].store)
        {
            commands[0].store->info_w();
        }
    }
}

//放箭,下一步走到司令部的武士不需要放箭
void arrow_attack(city *citys)
{
    for (int i = 1; i <= N; i++)
    {
        if (citys[i].city_monsters[1])
            if (i != N && citys[i].city_monsters[1]->weapons[2] && citys[i + 1].city_monsters[0])
            {
                citys[i].city_monsters[1]->use_arrow(citys[i + 1].city_monsters[0]);
            }
        if (citys[i].city_monsters[0])
            if (i != 1 && citys[i].city_monsters[0]->weapons[2] && citys[i - 1].city_monsters[1])
            {
                citys[i].city_monsters[0]->use_arrow(citys[i - 1].city_monsters[1]);
            }
    }
}

void bombing(city *citys)
{
    for (int i = 1; i <= N; i++)
    {
        if(citys[i].city_monsters[0] == NULL || citys[i].city_monsters[1] == NULL)
            continue;
        bool flag = false;
        int city_flag = int(citys[i].flag);
        if (citys[i].city_monsters[0] -> monster_hp > 0 && citys[i].city_monsters[1] -> monster_hp > 0)
        {
            
            if (citys[i].city_monsters[city_flag]->weapons[1])
            {
                if (citys[i].city_monsters[city_flag]->use_bomb(citys[i].city_monsters[1 - city_flag], true))
                {
                    flag = true;
                }
            }
            if (!flag && citys[i].city_monsters[1 - city_flag]->weapons[1])
            {
                if (citys[i].city_monsters[1 - city_flag]->use_bomb(citys[i].city_monsters[city_flag], false))
                {
                    flag = true;
                }
            }
        }
            //收尸 😂
            if (flag)
            {
                delete citys[i].city_monsters[0];
                delete citys[i].city_monsters[1];
                citys[i].city_monsters[0] = NULL;
                citys[i].city_monsters[1] = NULL;
            }
        
    }
}

void fighting(city *citys)
{
    for (int i = 1; i <= N; i++)
    {
        if(citys[i].city_monsters[0] == NULL ||  citys[i].city_monsters[1] == NULL)
            continue;
        if (citys[i].city_monsters[1] -> monster_hp > 0 && citys[i].city_monsters[0] -> monster_hp > 0)
        {
            int city_flag = int(citys[i].flag);
            citys[i].city_monsters[city_flag]->strike(citys[i].city_monsters[1 - city_flag], i);
        }
        if(citys[i].city_monsters[1] -> monster_hp > 0 && citys[i].city_monsters[0] -> monster_hp <= 0){
            citys[i].win(1);
        }
        if(citys[i].city_monsters[0] -> monster_hp > 0 && citys[i].city_monsters[1] -> monster_hp <= 0){
            citys[i].win(0);
        }
        if (citys[i].city_monsters[1]->monster_hp > 0 && citys[i].city_monsters[0]->monster_hp > 0){
            citys[i].last_win = -1;
        }
    }
}




void reward(city *citys)
{
    for(int i = N; i > 0; i--){
        if(citys[i].city_monsters[1] == NULL || citys[i].city_monsters[0] == NULL)
            continue;
        if(citys[i].city_monsters[1] -> monster_hp > 0 && citys[i].city_monsters[0] -> monster_hp <= 0){
            if (commands[1].command_hp >= 8)
            {
                commands[1].command_hp -= 8;
                citys[i].city_monsters[1]->monster_hp += 8;
            }
        }
    }

    for(int i = 1; i <= N; i++){
        if (citys[i].city_monsters[1] == NULL || citys[i].city_monsters[0] == NULL)
            continue;
        if(citys[i].city_monsters[0] -> monster_hp > 0 && citys[i].city_monsters[1] -> monster_hp <= 0){
            if (commands[0].command_hp >= 8)
            {
                commands[0].command_hp -= 8;
                citys[i].city_monsters[0]->monster_hp += 8;
            }
        }
    }


    //夺取
    for (int i = 1; i <= N; i++)
    {
        for(int j = 0; j < 2; j++)
        if(citys[i].city_monsters[j] && citys[i].city_monsters[j] -> monster_hp <= 0){
            delete citys[i].city_monsters[j];
            citys[i].city_monsters[j] = NULL;
        }

        if (citys[i].city_hp == 0)
        {
            continue;
        }
        if (citys[i].city_monsters[0] == NULL && citys[i].city_monsters[1] != NULL)
        {
            commands[1].command_hp += citys[i].city_hp;
            citys[i].city_hp = 0;
        }
        if (citys[i].city_monsters[1] == NULL && citys[i].city_monsters[0] != NULL)
        {
            commands[0].command_hp += citys[i].city_hp;
            citys[i].city_hp = 0;
        }
    }
}


int main()
{
    cin >> t;
    for (int k = 1; k <= t; k++)
    {

        cin >> M >> N >> R >> K >> T;
        for (int i = 0; i < 5; i++)
            cin >> hp[i];
        for (int i = 0; i < 5; i++)
            cin >> strength[i];

        printf("Case %d:\n", k);
        Mytime.refresh();
        commands[0].refresh();
        commands[1].refresh();

        //本来的想法是用函数开辟，但是发现不能那样写
        city *citys;
        citys = new city[N + 1]; //申请N+1个空间，使城市从一开始排列
        for (int i = 1; i != N + 1; i++)
        {
            citys[i].get_flag(i);
        }

        while (1)
        {
            if (Mytime.is_end(0))
                break;
            create_monster();

            if (Mytime.is_end(5))
            {
                break;
            }
            run_away(citys);

            if (Mytime.is_end(5))
                break;

            forward(citys);

            if (victory(false) || Mytime.is_end(10))
            {
                break;
            }

            create_hp(citys);

            if (Mytime.is_end(10))
                break;
            get_hp(citys);

            if (Mytime.is_end(5))
                break;
            //放箭
            arrow_attack(citys);
            //cout<<"finish arrow_attack"<<endl;

            if (Mytime.is_end(3))
                break;
            //评估炸弹
            bombing(citys);
            //cout<<"finish bombing"<<endl;

            if (Mytime.is_end(2))
                break;
            //战斗
            fighting(citys);
            //cheer(citys);
            //cout<<"finish fighting"<<endl;

            //战后的奖励、
            reward(citys);
            //cout<<"finish reward"<<endl;

            //change_flag(citys);
            //cout<<"finfish change_flag"<<endl;

            if (Mytime.is_end(10))
                break;
            //司令部报告
            command_report();

            if (Mytime.is_end(5))
                break;
            //武士报告
            monster_report(citys);

            if (Mytime.is_end(5))
                break;
            //循环结束
        }
        delete citys;
    }
    return 0;
}

/*
一些问题，为什么在函数内部用new申请了空间外部无法访问

剩余工作：
欢呼
夺取
奖励
*/

/* 
1
10000 10 1 1 1000
80  80 80 80 80
1 1 1 1 1*/
