# include <bits/stdc++.h>
# define MAXN 100000
# define inf 1e9+10
# include <unistd.h>
# define PATH_SIZE 255
using namespace std;

map<string, int> linestring2id; //线路到id的映射 
map<int, string> id2linestring; //id到线路的映射 
map<string, int> stationstring2id; //站点到id的映射 
map<int, string> id2stationstring; //id到站点的映射 

struct Node{ //用于存储地铁站信息 
	int lines[10]; //经过站点线路的列表 
	int numOfLines; //经过站点的线路数 
}stations[MAXN];

int lineNum; //线路总数 
int stationNum; //站点总数 
int pairNum; //边总数 
int lineSpeed[MAXN]; //每条线路的最高时速，单位为公里每小时 
int visLine[MAXN]; //用于查找两个站所在的地铁线 
int rawFir[MAXN]; //用于存储原始站与站之间关系，表示第一个站 
int rawSec[MAXN]; //用于存储原始站与站之间关系，表示第二个站 
int rawVal[MAXN]; //用于存储原始站与站之间关系，表示两站之间的距离（单位为米） 

int head[MAXN]; //Dijkstra算法存储图信息 
int next_[2 * MAXN]; //Dijkstra算法存储图信息 
int val[2 * MAXN]; //Dijkstra算法存储图中距离信息 
int goal[2 * MAXN]; //Dijkstra算法存储图信息 
int dis[MAXN]; //Dijkstra算法存储图中最短距离信息 
bool vis[MAXN]; //Dijkstra算法记录点是否经过 
int pre[MAXN]; //Dijkstra算法存储图中站点信息 
int totans[MAXN]; //Dijkstra算法存储图中站点信息 

void buildlines(); //建立铁路线到id的映射 
void buildstations(); //建立站点到id的映射 
void stationinform(); //记录每一站的信息 
void distOfStations(); //记录相邻两站的距离信息 
void readLineSpeed(); //记录每条线路的最高时速 
void cleaner(); //清空最短路算法中使用过的数组 
string id2line(int id); //通过id返回对应铁路线 
string id2station(int id); //通过id返回对应站点 
int station2id(string s); //通过站点名称返回对应id 
int findLine(int st1, int st2); //通过站点id返回对应线路 
int navigation(int st, int ed, int pattern); //三合一计算最优路线，patter为0时表示距离最短，返回最短距离；pattern为1时表示时间最快，返回最快时间；pattern为1时表示换乘次数最少，返回最少换乘次数 
int culcost(int meters); //根据路程计算车费，路程单位为米 
int main(){
	buildlines();
	buildstations();
	stationinform();
	distOfStations();
	readLineSpeed();
	char path[PATH_SIZE];
	if(!getcwd(path,PATH_SIZE)){
        cout<<"Get path fail!"<<endl;
        return 0;
    }
    string v = "\\dist\\visualize.exe";
    string ne = path + v;
    const char* p = ne.data();
	string STOP = "y";
	while(STOP != "n"){
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "                                     缺心眼导航                                     " << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "请输入出发地铁站名称（例：积水潭）：" << endl;
		cout << "起始点："; 
	//	string start_station = "积水潭";
		string start_station;
		cin >> start_station; 
		cout << "请输入目的地地铁站名称（例：国贸）：" << endl;
		cout << "目的地："; 
	//	string end_station = "国贸";
		string end_station;
		cin >> end_station;
		cout << "请输入想要查询的导航方案（0代表查询最短路径，1代表查询最快路径，2代表查询转站次数最少路径）：";
		int mod = 0;	
		cin >> mod; 
		switch(mod){
			case 0:{
				int mindistance = navigation(station2id(start_station), station2id(end_station), 0); 
				cout << "从"<< start_station <<"到"<< end_station <<"的最短距离为：" << mindistance * 1.0 / 1000 << "公里" << endl;
				cout << "地铁票价为"<< culcost(mindistance) << "元" << endl;
				break;
			}
			case 1:{
				int mintime = navigation(station2id(start_station), station2id(end_station), 1);
				cout << "从"<< start_station <<"到"<< end_station <<"的最短时间为（不考虑换乘和等车时间）：" << mintime * 1.0 / 60 << "分钟" << endl;
				break;
			}
			case 2:{
				int minchange = navigation(station2id(start_station), station2id(end_station), 2);
				cout << "从"<< start_station <<"到"<< end_station <<"的最少换乘次数为：" << minchange - 1 << "次" << endl;
				break;
			}
		}
		puts(""); 
		cout << "即将呈现可视化路径，点击“Esc”键即可关闭图片。" << endl; 
		system(p);
//		system("pause");
//		getchar();getchar();
		cout << "是否继续查询？（输入y继续，输入n退出)" << endl;
		cin >> STOP;
	}
	cout << "------------------------------------------------------------------------------------" << endl;
	cout << "                            缺心眼导航期待您的下一次使用！                          " << endl;
	cout << "------------------------------------------------------------------------------------" << endl;
	return 0;	
}
void buildlines(){
	ifstream fin("line_list.in");
	string lineName;
	fin >> lineNum;
	for(int i = 1;i <= lineNum;i++){
		fin >> lineName;
		linestring2id.insert(pair<string, int>(lineName, i - 1));
		id2linestring.insert(pair<int, string>(i - 1, lineName));
	}
	printf("@>>>---finish building the dictionary of the lines' name.\n");
}
string id2line(int id){
	map<int, string>::iterator it;
	it = id2linestring.find(id);
	return it->second;
}
void buildstations(){
	ifstream fin("station_list.in");
	string stationName;
	fin >> stationNum;
	for(int i = 1;i <= stationNum;i++){
		fin >> stationName;
		stationstring2id.insert(pair<string, int>(stationName, i - 1));
		id2stationstring.insert(pair<int, string>(i - 1, stationName));
	}
	printf("@>>>---finish building the dictionary of the stations' name.\n");
}
string id2station(int id){
	map<int, string>::iterator it;
	it = id2stationstring.find(id);
	return it->second;
}
int station2id(string s){
	map<string, int>::iterator it;
	it = stationstring2id.find(s);
	return it->second;
}
void stationinform(){
	ifstream fin("station_line_tags.in");
	int k, id;
	
	for(int i = 1;i <= stationNum;i++){
		fin >> k >> id;
		stations[id].numOfLines = k;
		
		for(int j = 1;j <= k;j++){
			fin >> stations[id].lines[j];
		}
	}
	printf("@>>>---finish recording the stations' information.\n");
}
void distOfStations(){ 
	ifstream fin("distances.in");
	fin >> pairNum;
	for(int i = 1;i <= pairNum;i++){
		int i1, i2, i3;
		fin >> i1 >> i2 >> i3;
		rawFir[i] = i1;
		rawSec[i] = i2;
		rawVal[i] = i3;
	}
	printf("@>>>---finish recording the distances between stations.\n");
} 
void readLineSpeed(){
	ifstream fin("lineSpeed.in");
	for(int i = 1;i <= lineNum;i++){
		fin >> lineSpeed[i - 1];
	}
	printf("@>>>---finish recording the speed of lines.\n");
} 
int findLine(int st1, int st2){
	for(int i = 0;i <= lineNum;i++){
		visLine[i] = 0;
	}
	for(int i = 1;i <= stations[st1].numOfLines;i++){
//		cout << stations[st1].lines[i] << endl;
		visLine[stations[st1].lines[i]] = 1;	
	} 
	for(int i = 1;i <= stations[st2].numOfLines;i++){
		if(visLine[stations[st2].lines[i]] == 1)return stations[st2].lines[i];
	}
	return -1;
}

void add(int a,int b,int c,int e)//建边 
{
	next_[e]=head[a];
	head[a]=e;
	goal[e]=b;
	val[e]=c;
	return;
}

struct node{
	int num;
	int mindis;
};
 
bool operator < (const node x,const node y)//重载运算符 
{
	return x.mindis > y.mindis;
}
priority_queue <node> q;


void dijkstra(int s, int n)
{
	node a;
	while(!q.empty())q.pop();//清空队列 
	for(int k=0;k<=n;k++)//初始化 
	{
		dis[k]=inf;
		vis[k]=false;
	}
	q.push((node){s,0});//出发点入队列 
	dis[s]=0;
	pre[s]=-1;
	
	while(true)
	{
		
		while((!q.empty()) && vis[q.top().num])q.pop();
		if(q.empty())break;
		a=q.top(); q.pop();
		vis[a.num]=true;
		for(int i=head[a.num];i!=-1;i=next_[i])
		{
			if(vis[goal[i]])continue;//如果已经访问过则不再访问 
			if(dis[a.num]+val[i]<dis[goal[i]])//边的松弛操作 
			{
				dis[goal[i]]=dis[a.num]+val[i];
				pre[goal[i]]=a.num;
				q.push((node){goal[i],dis[a.num]+val[i]});//新节点入队列 
			}
			
		}
	}
	return;
}

int navigation(int st, int ed, int pattern){//st表示出发点对应的id，ed表示目的地对应的id 
	int cnt = 0; 
	memset(head,-1,sizeof(head));
	int cnt_e=0;
	if(pattern == 0 || pattern == 1)
		for(int i = 1;i <= pairNum;i++){
			if(!pattern){//第一种建边方式 
				add(rawFir[i],rawSec[i],rawVal[i],cnt_e++);
				add(rawSec[i],rawFir[i],rawVal[i],cnt_e++);	
			}
			else{//第二种建边方式 
				add(rawFir[i],rawSec[i],rawVal[i] / (5 * lineSpeed[findLine(rawFir[i], rawSec[i])] / 36),cnt_e++);
				add(rawSec[i],rawFir[i],rawVal[i] / (5 * lineSpeed[findLine(rawFir[i], rawSec[i])] / 36),cnt_e++);
			} 
			
		}
	else{//第三种建边方式 
		for(int i = 0;i < stationNum;i++){
			for(int j = 0;j < stationNum;j++){
				if(findLine(i,j) != -1){
					add(i,j,1,cnt_e++);
				}
			}
		}
	}
	dijkstra(st, stationNum);//dijkstra算法 
	for(int k=ed;k!=-1;k=pre[k])totans[cnt++]=k;
	cout << "您的乘车路径为：" << endl;
	for(int k=cnt-1;k>0;k--)cout << id2station(totans[k]) << "->";
	cout << id2station(totans[0]);
	printf("\n");
	ofstream fout("route.out");//乘车路径写入文件 
	for(int k=cnt-1;k>0;k--)fout << id2station(totans[k]) << " ";
	fout << id2station(totans[0]);
	puts("");
	int ansdist = dis[ed];
	cleaner();//初始化参数 
	return ansdist;//返回最短距离 
}
void cleaner(){
	for(int i = 0;i <= MAXN - 1;i++){
		totans[i] = dis[i] = pre[i] = head[i] = next_[2 * i] = next_[2 * i + 1] = val[2 * i] = val[2 * i + 1] = goal[2 * i] = goal[2 * i + 1] = 0;
	}
} 
int culcost(int meters){
/*
	轨道交通价格为：6公里(含)内3元;6公里至12公里(含)4元;12公里至22公里(含)5元;22公里至32公里(含)6元;32公里以上部分，每增加1元可乘坐20公里。
*/
	if(meters <= 6000)return 3;
	else if(meters <= 12000)return 4;
	else if(meters <= 22000)return 5;
	else if(meters <= 32000)return 6;
	else if (((meters - 32000) / 20000) * 20000 + 32000 != meters)return ((meters - 32000) / 20000) + 7;
	else return (meters - 32000) / 20000 + 6;
}


 
