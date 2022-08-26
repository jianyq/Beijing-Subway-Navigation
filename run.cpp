# include <bits/stdc++.h>
# define MAXN 100000
# define inf 1e9+10
# include <unistd.h>
# define PATH_SIZE 255
using namespace std;

map<string, int> linestring2id; //��·��id��ӳ�� 
map<int, string> id2linestring; //id����·��ӳ�� 
map<string, int> stationstring2id; //վ�㵽id��ӳ�� 
map<int, string> id2stationstring; //id��վ���ӳ�� 

struct Node{ //���ڴ洢����վ��Ϣ 
	int lines[10]; //����վ����·���б� 
	int numOfLines; //����վ�����·�� 
}stations[MAXN];

int lineNum; //��·���� 
int stationNum; //վ������ 
int pairNum; //������ 
int lineSpeed[MAXN]; //ÿ����·�����ʱ�٣���λΪ����ÿСʱ 
int visLine[MAXN]; //���ڲ�������վ���ڵĵ����� 
int rawFir[MAXN]; //���ڴ洢ԭʼվ��վ֮���ϵ����ʾ��һ��վ 
int rawSec[MAXN]; //���ڴ洢ԭʼվ��վ֮���ϵ����ʾ�ڶ���վ 
int rawVal[MAXN]; //���ڴ洢ԭʼվ��վ֮���ϵ����ʾ��վ֮��ľ��루��λΪ�ף� 

int head[MAXN]; //Dijkstra�㷨�洢ͼ��Ϣ 
int next_[2 * MAXN]; //Dijkstra�㷨�洢ͼ��Ϣ 
int val[2 * MAXN]; //Dijkstra�㷨�洢ͼ�о�����Ϣ 
int goal[2 * MAXN]; //Dijkstra�㷨�洢ͼ��Ϣ 
int dis[MAXN]; //Dijkstra�㷨�洢ͼ����̾�����Ϣ 
bool vis[MAXN]; //Dijkstra�㷨��¼���Ƿ񾭹� 
int pre[MAXN]; //Dijkstra�㷨�洢ͼ��վ����Ϣ 
int totans[MAXN]; //Dijkstra�㷨�洢ͼ��վ����Ϣ 

void buildlines(); //������·�ߵ�id��ӳ�� 
void buildstations(); //����վ�㵽id��ӳ�� 
void stationinform(); //��¼ÿһվ����Ϣ 
void distOfStations(); //��¼������վ�ľ�����Ϣ 
void readLineSpeed(); //��¼ÿ����·�����ʱ�� 
void cleaner(); //������·�㷨��ʹ�ù������� 
string id2line(int id); //ͨ��id���ض�Ӧ��·�� 
string id2station(int id); //ͨ��id���ض�Ӧվ�� 
int station2id(string s); //ͨ��վ�����Ʒ��ض�Ӧid 
int findLine(int st1, int st2); //ͨ��վ��id���ض�Ӧ��· 
int navigation(int st, int ed, int pattern); //����һ��������·�ߣ�patterΪ0ʱ��ʾ������̣�������̾��룻patternΪ1ʱ��ʾʱ����죬�������ʱ�䣻patternΪ1ʱ��ʾ���˴������٣��������ٻ��˴��� 
int culcost(int meters); //����·�̼��㳵�ѣ�·�̵�λΪ�� 
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
		cout << "                                     ȱ���۵���                                     " << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "�������������վ���ƣ�������ˮ̶����" << endl;
		cout << "��ʼ�㣺"; 
	//	string start_station = "��ˮ̶";
		string start_station;
		cin >> start_station; 
		cout << "������Ŀ�ĵص���վ���ƣ�������ó����" << endl;
		cout << "Ŀ�ĵأ�"; 
	//	string end_station = "��ó";
		string end_station;
		cin >> end_station;
		cout << "��������Ҫ��ѯ�ĵ���������0�����ѯ���·����1�����ѯ���·����2�����ѯתվ��������·������";
		int mod = 0;	
		cin >> mod; 
		switch(mod){
			case 0:{
				int mindistance = navigation(station2id(start_station), station2id(end_station), 0); 
				cout << "��"<< start_station <<"��"<< end_station <<"����̾���Ϊ��" << mindistance * 1.0 / 1000 << "����" << endl;
				cout << "����Ʊ��Ϊ"<< culcost(mindistance) << "Ԫ" << endl;
				break;
			}
			case 1:{
				int mintime = navigation(station2id(start_station), station2id(end_station), 1);
				cout << "��"<< start_station <<"��"<< end_station <<"�����ʱ��Ϊ�������ǻ��˺͵ȳ�ʱ�䣩��" << mintime * 1.0 / 60 << "����" << endl;
				break;
			}
			case 2:{
				int minchange = navigation(station2id(start_station), station2id(end_station), 2);
				cout << "��"<< start_station <<"��"<< end_station <<"�����ٻ��˴���Ϊ��" << minchange - 1 << "��" << endl;
				break;
			}
		}
		puts(""); 
		cout << "�������ֿ��ӻ�·���������Esc�������ɹر�ͼƬ��" << endl; 
		system(p);
//		system("pause");
//		getchar();getchar();
		cout << "�Ƿ������ѯ��������y����������n�˳�)" << endl;
		cin >> STOP;
	}
	cout << "------------------------------------------------------------------------------------" << endl;
	cout << "                            ȱ���۵����ڴ�������һ��ʹ�ã�                          " << endl;
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

void add(int a,int b,int c,int e)//���� 
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
 
bool operator < (const node x,const node y)//��������� 
{
	return x.mindis > y.mindis;
}
priority_queue <node> q;


void dijkstra(int s, int n)
{
	node a;
	while(!q.empty())q.pop();//��ն��� 
	for(int k=0;k<=n;k++)//��ʼ�� 
	{
		dis[k]=inf;
		vis[k]=false;
	}
	q.push((node){s,0});//����������� 
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
			if(vis[goal[i]])continue;//����Ѿ����ʹ����ٷ��� 
			if(dis[a.num]+val[i]<dis[goal[i]])//�ߵ��ɳڲ��� 
			{
				dis[goal[i]]=dis[a.num]+val[i];
				pre[goal[i]]=a.num;
				q.push((node){goal[i],dis[a.num]+val[i]});//�½ڵ������ 
			}
			
		}
	}
	return;
}

int navigation(int st, int ed, int pattern){//st��ʾ�������Ӧ��id��ed��ʾĿ�ĵض�Ӧ��id 
	int cnt = 0; 
	memset(head,-1,sizeof(head));
	int cnt_e=0;
	if(pattern == 0 || pattern == 1)
		for(int i = 1;i <= pairNum;i++){
			if(!pattern){//��һ�ֽ��߷�ʽ 
				add(rawFir[i],rawSec[i],rawVal[i],cnt_e++);
				add(rawSec[i],rawFir[i],rawVal[i],cnt_e++);	
			}
			else{//�ڶ��ֽ��߷�ʽ 
				add(rawFir[i],rawSec[i],rawVal[i] / (5 * lineSpeed[findLine(rawFir[i], rawSec[i])] / 36),cnt_e++);
				add(rawSec[i],rawFir[i],rawVal[i] / (5 * lineSpeed[findLine(rawFir[i], rawSec[i])] / 36),cnt_e++);
			} 
			
		}
	else{//�����ֽ��߷�ʽ 
		for(int i = 0;i < stationNum;i++){
			for(int j = 0;j < stationNum;j++){
				if(findLine(i,j) != -1){
					add(i,j,1,cnt_e++);
				}
			}
		}
	}
	dijkstra(st, stationNum);//dijkstra�㷨 
	for(int k=ed;k!=-1;k=pre[k])totans[cnt++]=k;
	cout << "���ĳ˳�·��Ϊ��" << endl;
	for(int k=cnt-1;k>0;k--)cout << id2station(totans[k]) << "->";
	cout << id2station(totans[0]);
	printf("\n");
	ofstream fout("route.out");//�˳�·��д���ļ� 
	for(int k=cnt-1;k>0;k--)fout << id2station(totans[k]) << " ";
	fout << id2station(totans[0]);
	puts("");
	int ansdist = dis[ed];
	cleaner();//��ʼ������ 
	return ansdist;//������̾��� 
}
void cleaner(){
	for(int i = 0;i <= MAXN - 1;i++){
		totans[i] = dis[i] = pre[i] = head[i] = next_[2 * i] = next_[2 * i + 1] = val[2 * i] = val[2 * i + 1] = goal[2 * i] = goal[2 * i + 1] = 0;
	}
} 
int culcost(int meters){
/*
	�����ͨ�۸�Ϊ��6����(��)��3Ԫ;6������12����(��)4Ԫ;12������22����(��)5Ԫ;22������32����(��)6Ԫ;32�������ϲ��֣�ÿ����1Ԫ�ɳ���20���
*/
	if(meters <= 6000)return 3;
	else if(meters <= 12000)return 4;
	else if(meters <= 22000)return 5;
	else if(meters <= 32000)return 6;
	else if (((meters - 32000) / 20000) * 20000 + 32000 != meters)return ((meters - 32000) / 20000) + 7;
	else return (meters - 32000) / 20000 + 6;
}


 
