#include <iostream>  
#include <string.h>  
#include <fstream>  
#include <iterator>  
#include <algorithm>  
#include <limits.h>  
#include <math.h>  
#include <stdlib.h>  
#include <ctime>
#include <vector>
#include <map>

using namespace std;

#define N 150 // 城市数量

const int BEST = 6528;						// 最优解

double length_table[N][N];		// 存储各点间距离

const int group_size = 1000;					// 种群大小
const int breed_time = 8000;				// 繁殖代数
const double p_cross = 0.8;					// 交叉概率
double p_variation = 0.5;					// 变异概率

// 一个个体
class Path
{
public:
	// 计算路径的长度
	void get_len() {
		len = 0;
		for (int i = 1; i < N; i++) {
			len += length_table[path[i - 1] - 1][path[i] - 1];
		}
		len += length_table[path[N - 1] - 1][path[0] - 1];
	}

	// 随机生成一个解
	Path() {
		len = 0;
		generate_random();
		get_len();
	}

	void generate_random() {
		for (int i = 0; i < N; ++i)
		{
			path[i] = i + 1;
		}
		srand(time(0));
		/*for (int i = 0; i < N; ++i)
		{
			int i1 = rand() % (N - 1);
			int i2 = rand() % (N - 1);
			int temp = path[i1];
			path[i2] = path[i1];
			path[i1] = temp;
		}*/
	}

	void getNewSolution_cross(Path& t) {
		// 单点交叉
		int mark = rand() % (N - 2) + 1;//1 to nCities - 2
		for (int i = mark; i < N; i++) {
			int temp = path[i];
			path[i] = t.path[i];
			t.path[i] = temp;
		}
		// 判断解的合法性
		int i = 0; int j = 0;
		bool count_dup_1[N + 1] = { false };
		bool count_dup_2[N + 1] = { false };
		while (i < N && j < N) {
			if (count_dup_1[path[i]] && count_dup_2[t.path[j]]) {
				int temp = path[i];
				path[i] = t.path[j];
				t.path[j] = temp;
				i++;
				j++;
			}
			if (i >= N || j >= N)
				break;
			if (!count_dup_1[path[i]]) {
				count_dup_1[path[i]] = true;
				i++;
			}
			if (!count_dup_2[t.path[j]]) {
				count_dup_2[t.path[j]] = true;
				j++;
			}
		}
		get_len();
		t.get_len();
	}

	// 4种变异，在组内变化，不会出现非法解
	void getNewSolution_variation() {
		//生成0-1之间的一个随机数p
		//p≤0.25:Inversion Mutation
		//0.25<p≤0.50:Insertion Mutation
		//0.50<p≤0.75:Displacement Mutation
		//0.75<p:Swap Mutation
		double p = (double)rand() / (double)RAND_MAX;

		if (p <= 0.25) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//随机获得第一个点的位置
				index2 = (int)N * rand() / (RAND_MAX + 1);//随机获得第二个点的位置
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);

			//反转子序列
			for (int i = p1, j = p2; i < j; i++, j--) {
				int temp = path[i];
				path[i] = path[j];
				path[j] = temp;
			}
		}

		else if (p > 0.25 && p <= 0.50) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//随机获得一个点的位置
				index2 = (int)N * rand() / (RAND_MAX + 1);//随机获得该点要插入的位置
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);
			//插入
			int temp = path[p2];
			for (int i = p2 - 1;i >= p1;--i) {
				path[i + 1] = path[i];
			}
			path[p1] = temp;
		}

		else if (p > 0.50 && p <= 0.75) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//随机获得一个点的位置
				index2 = (int)N * rand() / (RAND_MAX + 1);//随机获得另一个点的位置
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);
			int* path_temp = new int[N];
			memcpy(path_temp, path, N * sizeof(int));
			path[0] = path_temp[p1];
			path[1] = path_temp[p2];
			int pos = 2;
			for (int i = 0; i < N; i++) {
				if (i == p1 || i == p2) continue;
				path[pos++] = path_temp[i];
			}
			free(path_temp);
		}

		else {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//随机获得第一个点的位置
				index2 = (int)N * rand() / (RAND_MAX + 1);//随机获得第二个的位置
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);
			int temp = path[p1];
			path[p1] = path[p2];
			path[p2] = temp;
		}
		get_len();
	}

	double getLength() {
		return len;
	}

	int* getPath() {
		return path;
	}

private:
	double len;//代价，总长度
	int path[N];//路径
};

class GA {
public:
	double city_pos[N][2];//存放城市的坐标

	//种群，大小为group_size
	vector<Path> group;

	GA();

	~GA();

	Path getPath();

private:
	int fit(Path p);//适应性函数

	void choose(vector<Path>& group);//选择

	void cross(vector<Path>& group);//交叉

	void variation(vector<Path>& group);//变异

	void judge(vector<Path>& old_group, vector<Path>& group);//// 决定子代是否能取代亲本

	void init();//初始化
};

GA::GA() {
	// 初始化种群
	init();
}

GA::~GA() {
	group.clear();
}

void GA::init() {
	//将城市的坐标从外部文件“tspdata.txt”中读入并存储，并生成初始种群

	int k = 0;//k表示当前是第k个城市
	double index, x, y;
	ifstream infile;
	infile.open("tspdata.txt");

	if (!infile.is_open()) {
		printf("Open file failed!\n");
	}
	//读入坐标并存储到city_pos[][]中
	while (!infile.eof()) {
		infile >> index >> x >> y;
		city_pos[k][0] = x;
		city_pos[k][1] = y;
		k++;
	}
	for (int i = 0; i < N; i++)
	{
		length_table[i][i] = (double)INT_MAX;
		for (int j = i + 1; j < N; j++)
		{
			length_table[i][j] = length_table[j][i] = sqrt(
				(city_pos[i][0] - city_pos[j][0]) * (city_pos[i][0] - city_pos[j][0]) +
				(city_pos[i][1] - city_pos[j][1]) * (city_pos[i][1] - city_pos[j][1]));
		}
	}

	/*for (int i = 0;i < group_size;i++) {
		Path p;
		group.push_back(p);
	}*/
	group.resize(group_size, Path());
	// 给种群一些 局部搜索出来的最优解
	Path new_path;
	Path copy = new_path;
	int j = 0;
	while (j < 50000) {
		new_path.getNewSolution_variation();
		if (copy.getLength() < new_path.getLength()) {
			copy = new_path;
		}
		j++;
	}
	for (int i = 0; i < 100; i++) {
		int num = rand() % group_size;
		group[num] = copy;
	}


}

//适应性函数
int GA::fit(Path p) {
	return 1 / p.getLength();
}

// 选择优秀的种群
void GA::choose(vector<Path>& group) {
	double sum_fitness = 0;
	double fitness[group_size];//适应性数组，用适应函数来计算
	double chance[group_size];//概率数组
	double pick;//用于轮盘赌的随机数
	vector<Path> next;

	//计算适应值
	for (int i = 0; i < group_size; i++) {
		fitness[i] = fit(group[i]);
		sum_fitness += fitness[i];
	}

	//计算概率
	for (int i = 0; i < group_size; i++) {
		chance[i] = fitness[i] / sum_fitness;
	}

	for (int i = 0; i < group_size; i++) {
		pick = ((double)rand()) / RAND_MAX;//0到1的随机数
		for (int j = 0; j < group_size; j++) {
			// 当pick<=chance就选该个体，chance越大越有机会
			if (pick <= chance[j]) {
				next.push_back(group[j]);
				break;
			}
			//仍未选中，但是已经到最后一个了
			if (j == group_size - 1) {
				next.push_back(group[j]);
			}
		}
	}
	group = next;
}

// 交叉
void GA::cross(vector<Path>& group) {
	int choice1, choice2;
	for (int i = 0;i < group_size;i += 2) {
		double pick = ((double)rand()) / RAND_MAX;//0到1的随机数
		if (pick > p_cross)//判断是否交叉
			continue;
		else {
			// 选择临近两个点来进行交叉
			choice1 = i;
			choice2 = i + 1;
			group[choice1].getNewSolution_cross(group[choice2]);//交叉
		}
	}
}

// 变异
void GA::variation(vector<Path>& group) {
	int point = 0;
	while (point < group_size) {
		double pick = ((double)rand()) / RAND_MAX;//0到1的随机数
		if (pick < p_variation) {
			group[point].getNewSolution_variation();
		}
		point++;
	}
}

// 决定子代是否能取代亲本，获取的优秀种群
void GA::judge(vector<Path>& old_group, vector<Path>& group) {
	int point = 0;
	while (point < group_size) {
		if (old_group[point].getLength() < group[point].getLength())
			group[point] = old_group[point];
		point++;
	}
}

Path GA::getPath() {
	// 初始化随机数种子
	srand((unsigned)time(NULL));

	ofstream outfile, outfile2;
	outfile2.open("result2.txt");
	outfile.open("result.txt");
	if (!outfile.is_open()) {
		printf("Open outfile failed!\n");
	}

	Path best;
	for (int i = 0; i < breed_time; i++) {//循环设置的繁衍代数次
		vector<Path> old_group = group;
		// 选择
		choose(group);
		// 交叉
		cross(group);
		// 变异次数为5
		for (int j = 0; j < 5; j++) {
			variation(group);
			judge(old_group, group);
		}
		// 找出种群中的最优解
		for (int j = 0; j < group_size; j++) {
			group[j].get_len();
			if (group[j].getLength() < best.getLength()) {
				best = group[j];
			}
		}
		cout << "当前最优解为：" << best.getLength() << "  当前迭代次数为：" << i << endl;

		if (i % 100 == 0) {
			for (int i = 0; i < N; i++) {
				outfile << (i + 1) << "\t" << best.getPath()[i] << endl;
			}
		}
		outfile2 << best.getLength() << endl;
	}
	outfile2.close();
	outfile.close();
	return best;
}

int main() {
	time_t start, finish;
	start = clock(); // 开始计时
	GA process;
	Path ans = process.getPath();
	bool count[N] = { false };
	int count2 = 0;
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			if (ans.getPath()[i] == j + 1)
			{
				count2++;
				break;
			}
		}
	}
	finish = clock(); //计时结束 
	if (count2 == N)
	{
		cout << endl << "Legal Solution!\n";
	}
	else {
		cout << endl << "Illegal Solution!\n";
		return 0;
	}
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
	cout << "Genetic Algorithm: " << endl;
	cout << "Best Path: " << endl;

	for (int i = 0; i < N - 1; i++) {
		cout << ans.getPath()[i] << " -> ";
	}
	cout << ans.getPath()[N - 1];

	cout << "\nRelative Error: " << (ans.getLength() - BEST) / BEST << endl;

	cout << "\nBest Path Length: " << ans.getLength() << endl;

	cout << "Time for Algorithm: " << duration << "sec" << endl;
	cout << endl;
}



