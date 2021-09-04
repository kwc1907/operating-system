#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#define MAX 1111
#define INF 0x3f3f3f3f         //代表无穷大
using namespace std;
typedef struct Disk_Scheduling
{
	int Trackhao;
	int Trackshu;
}DKSG;

int track[MAX];  
int mark[MAX];
double t;            //单位寻道时间
int n;
int now;             //当前磁道
int flag=1;          //向里向外扫描
//输入
int input(int& n, int& now)
{
	int im=0;
	int Min = INF;
	cout<<"请输入所求磁道号个数，开始磁道号，单位寻道时间以及磁头方向(0-向里  1-向外):"<<endl;
	cin >> n >> now>>t>>flag;
	cout<<"请输入"<<n<<"个磁道号:"<<endl;
	for (int i = 0; i < n; i++) {
		cin>>track[i];
		int temp = fabs(track[i] - now);
		if (temp < Min) {
			Min = temp;                               //获得离初始磁道最近的磁道号
			im = i;
		}
	}
	return im;
}
//输出
void output(int n, DKSG ds[MAX])
{
	int sum = 0;
	cout<<"被访问的下一个磁道号         横跨磁道数"<<endl;
	for (int i = 0; i < n; i++) {
		cout<<"       " << ds[i].Trackhao << "                     " << ds[i].Trackshu << endl;
		sum += ds[i].Trackshu;
	}
	cout << "横跨磁道总数：" << sum << endl;                         //输出横跨磁道总数
	cout << "平均寻道时间：" << (sum * t) / n << endl;              //输出平均寻道时间
}
//先来先服务
void FCFS(int now)
{
	DKSG ds[MAX];
	for (int i = 0; i < n; i++) {
		ds[i].Trackhao = track[i];
		ds[i].Trackshu = fabs(track[i] - now);
		now = track[i];
	}
	cout<<"先来先服务:"<<endl;
	output(n,ds);
}
//最短路径
void SSTF(int now,int im)
{
	DKSG ds[MAX];
	memset(mark, 0, sizeof(mark));
	for (int i = 0; i < n; i++) {
		int Min = INF;
		ds[i].Trackhao = track[im];
		ds[i].Trackshu = fabs(track[im] - now);
		now = track[im];
		mark[im] = 1;                                 //标记此位是否检查过
		for (int j = 0; j < n; j++) {
			if (!mark[j]) {
				double temp = fabs(track[j] - now);
				if (temp < Min) {
					Min = temp;
					im = j;                           //每轮循环选出最小的
				}
			}
		}
	}
	cout << "最短寻道时间优先:" << endl;
	output(n,ds);
}
//扫描
void SCAN(int now)
{
	DKSG ds[MAX];
	double retrack[MAX];
	if (flag)                                                        //向外扫描
	{
		sort(track, track + n);                                           //从小到大排序
		int locate = upper_bound(track, track + n, now) - track;         //二分查找第一个大于now的数字
		int t = locate;                                            //记录其坐标
		int k, r;
		for (k = 0; k < n - locate; k++) {
			retrack[k] = track[k + locate];
		}
		for (r = k; r < n; r++) {
			retrack[r] = track[--t];
		}
		for (int i = 0; i < n; i++) {
			ds[i].Trackhao = retrack[i];                               //将最终结果存入ds数组中
			ds[i].Trackshu = fabs(retrack[i] - now);
			now = retrack[i];
		}
	}
	else
	{                                                            //往里扫描
		sort(track, track + n, greater<int>());                  //从大往小排序       
		int locate = upper_bound(track, track + n, now, greater<int>()) - track;     //二分查找第一个大于now的数字         
		int t = locate;                                      //记录其坐标
		int k, r;
		for (k = 0; k < n - locate; k++) {
			retrack[k] = track[k + locate];
		}
		for (r = k; r < n; r++) {
			retrack[r] = track[--t];
		}                                                         //在retrack数组中记录扫描结果
		for (int i = 0; i < n; i++) {
			ds[i].Trackhao = retrack[i];
			ds[i].Trackshu = fabs(retrack[i] - now);                 //将最终结果存入ds数组中
			now = retrack[i];
		}
	}
	printf("\n扫描调度算法:\n");
	output(n,ds);
}

//循环扫描
void C_SCAN(int now)
{
	DKSG ds[MAX];
	double retrack[MAX];
	if (flag)                                                     //向外扫描
	{
		sort(track, track + n);                                    //从小到大排序
		int locate = upper_bound(track, track + n, now) - track;
		int im;
		for (int i = 0; i < n; i++) {
			im = (i + locate) % n;
			ds[i].Trackhao = track[im];
			ds[i].Trackshu = fabs(track[im] - now);                       //将最终结果存入ds数组中
			now = track[im];
		}
	}
	else                                                                  //往里扫描
	{
		sort(track, track + n, greater<int>());                             //从大往小排序  
		int locate = upper_bound(track, track + n, now, greater<int>()) - track;       //二分查找第一个大于now的数字 
		int t = locate;                                                     //记录其坐标
		int k, r,l=0; 
		for (k = 0; k < n - locate; k++) {
			retrack[k] = track[k + locate];
		}
		for (r = k; r < n; r++) {
			retrack[r] = track[l++];                                            //在retrack数组中记录扫描结果
		}          
		for (int i = 0; i < n; i++) {
			ds[i].Trackhao = retrack[i];
			ds[i].Trackshu = fabs(retrack[i] - now);
			now = retrack[i];                                        //将最终结果存入ds数组中
		}
	}
	printf("\n循环扫描调度算法:\n"); 
	output(n, ds);                                               //输出最终答案
}
int main()
{
	char ch;
	do {
		int i = input(n,now);
		FCFS(now);
		SSTF(now, i);
		SCAN(now);
		C_SCAN(now);
		cout << "是否继续:";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
	return 0;
}