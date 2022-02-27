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

const int N = 150; 					// ��������  
const int BEST = 6528;						// ���Ž�

double length_table[N][N];		// �洢��������

const int group_size = 1000;					// ��Ⱥ��С
const int time_to_breed = 8000;				// ��ֳ����
const double p_cross = 0.8;					// �������
double p_variation = 0.5;					// �������

// һ������
class Path
{
public:
	// ����·���ĳ���
	void calculate_len() {
		len = 0;
		//����path
		for (int i = 1; i < N; i++) {
			len += length_table[path[i - 1] - 1][path[i] - 1];
		}
		len += length_table[path[N - 1] - 1][path[0] - 1];
	}

	// �������һ����
	Path() {
		len = 0;
		generate_random();
		calculate_len();
	}

	void generate_random() {
		for (int i = 0; i < N; ++i)
		{
			path[i] = i + 1;
		}
		srand(time(0));
		/*for (int i = 0; i < nCities; ++i)
		{
			int city1 = rand() % nCities;
			int city2 = rand() % nCities;
			int temp = path[city2];
			path[city2] = path[city1];
			path[city1] = temp;
		}*/
	}

	void get_cross(Path& t) {
		// ���㽻��
		int mark = rand() % (N - 2) + 1;//1 to nCities - 2
		for (int i = mark; i < N; i++) {
			int temp = path[i];
			path[i] = t.path[i];
			t.path[i] = temp;
		}
		// �жϽ�ĺϷ���
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
		calculate_len();
		t.calculate_len();
	}

	// ������������ڱ仯��������ַǷ���
	void get_variation() {
		//����0-1֮���һ�������p
			//p��0.25:Inversion Mutation
			//0.25<p��0.50:Insertion Mutation
			//0.50<p��0.75:Displacement Mutation
			//0.75<p:Swap Mutation
		double p = (double)rand() / (double)RAND_MAX;

		if (p <= 0.25) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//�����õ�һ�����λ��
				index2 = (int)N * rand() / (RAND_MAX + 1);//�����õڶ������λ��
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);

			//��ת������
			for (int i = p1, j = p2; i < j; i++, j--) {
				int temp = path[i];
				path[i] = path[j];
				path[j] = temp;
			}
		}

		else if (p > 0.25 && p <= 0.50) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//������һ�����λ��
				index2 = (int)N * rand() / (RAND_MAX + 1);//�����øõ�Ҫ�����λ��
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);
			//����
			int temp = path[p2];
			for (int i = p2 - 1;i >= p1;--i) {
				path[i + 1] = path[i];
			}
			path[p1] = temp;
		}

		else if (p > 0.50 && p <= 0.75) {
			int index1 = -1, index2 = -1;
			while (index1 == index2) {
				index1 = (int)N * rand() / (RAND_MAX + 1);//������һ�����λ��
				index2 = (int)N * rand() / (RAND_MAX + 1);//��������һ�����λ��
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
				index1 = (int)N * rand() / (RAND_MAX + 1);//�����õ�һ�����λ��
				index2 = (int)N * rand() / (RAND_MAX + 1);//�����õڶ�����λ��
			}
			int p1 = min(index1, index2);
			int p2 = max(index1, index2);
			int temp = path[p1];
			path[p1] = path[p2];
			path[p2] = temp;
		}
		calculate_len();
	}

	double get_len() {
		return len;
	}

	int* get_path() {
		return path;
	}

private:
	double len;//����
	int path[N];//·��
};

class GA {
public:
	struct node {
		int num;
		double x;
		double y;
	}nodes[N];

	//��Ⱥ����СΪgroup_size
	vector<Path> group;

	void read_file();

	GA();

	~GA();

	Path get_answer();

private:
	void choose(vector<Path>& group);

	void cross(vector<Path>& group);

	void variation(vector<Path>& group);

	void judge(vector<Path>& old_group, vector<Path>& group);

	void init();
};

GA::GA() {
}

GA::~GA() {
	group.clear();
}

void GA::init() {
	// ��ʼ����Ⱥ
	group.resize(group_size, Path());
	// ����ȺһЩ �ֲ��������������Ž�
	Path new_path;
	Path copy = new_path;
	int j = 0;
	while (j < 50000) {
		new_path.get_variation();
		if (copy.get_len() < new_path.get_len()) {
			copy = new_path;
		}
		j++;
	}
	for (int i = 0; i < 100; i++) {
		int num = rand() % group_size;
		group[num] = copy;
	}

}

// ��ȡtxt�ļ�����ȡ����
void GA::read_file()
{
	int i, j;
	ifstream in("tspdata.txt");
	for (i = 0; i < N; i++)
	{
		in >> nodes[i].num >> nodes[i].x >> nodes[i].y;
	}

	for (i = 0; i < N; i++)
	{
		length_table[i][i] = (double)INT_MAX;
		for (j = i + 1; j < N; j++)
		{
			length_table[i][j] = length_table[j][i] = sqrt(
				(nodes[i].x - nodes[j].x) * (nodes[i].x - nodes[j].x) +
				(nodes[i].y - nodes[j].y) * (nodes[i].y - nodes[j].y));
		}
	}
}


void GA::choose(vector<Path>& group) {
	double sum_fitness = 0;
	double fitness[group_size];//��Ӧ�����飬����Ӧ����������
	double chance[group_size];//��������
	double pick;//�������̶ĵ������
	vector<Path> next;

	for (int i = 0; i < group_size; i++) {
		fitness[i] = 1 / group[i].get_len();
		sum_fitness += fitness[i];
	}
	for (int i = 0; i < group_size; i++) {
		chance[i] = fitness[i] / sum_fitness;
	}
	//���̶Ĳ���
	for (int i = 0; i < group_size; i++) {
		pick = ((double)rand()) / RAND_MAX;//0��1�������
		for (int j = 0; j < group_size; j++) {
			pick -= chance[j];
			// ��������ѡ����pickС��0��ѡ����Ⱥ��chanceԽ��Խ�л���
			if (pick <= 0) {
				next.push_back(group[j]);
				break;
			}
			//��δѡ�У������Ѿ������һ����
			if (j == group_size - 1) {
				next.push_back(group[j]);
			}
		}
	}
	group = next;
}

// ����
void GA::cross(vector<Path>& group) {
	int point = 0;
	int choice1, choice2;
	while (point < group_size) {
		double pick = ((double)rand()) / RAND_MAX;//0��1�������
		if (pick > p_cross)
			continue;//�ж��Ƿ񽻲�
		else {
			choice1 = point;
			choice2 = point + 1;
			group[choice1].get_cross(group[choice2]);//����
		}
		point += 2;
	}
}

// ����
void GA::variation(vector<Path>& group) {
	int point = 0;
	while (point < group_size) {
		double pick = ((double)rand()) / RAND_MAX;//0��1�������
		if (pick < p_variation) {
			group[point].get_variation();
		}
		point++;
	}
}

// �����Ӵ��Ƿ���ȡ���ױ�����ȡ��������Ⱥ
void GA::judge(vector<Path>& old_group, vector<Path>& group) {
	int point = 0;
	while (point < group_size) {
		if (old_group[point].get_len() < group[point].get_len())
			group[point] = old_group[point];
		point++;
	}
}

Path GA::get_answer() {
	// ��ʼ�����������
	srand((unsigned)time(NULL));
	// ��ʼ����Ⱥ
	init();

	ofstream outfile, outfile2;
	outfile2.open("len.txt");
	outfile.open("path.txt");
	if (!outfile.is_open()) {
		printf("Open outfile failed!\n");
	}

	Path best;
	for (int i = 1; i <= time_to_breed; i++) {
		vector<Path> old_group = group;
		// ѡ��
		choose(group);
		// ����
		cross(group);
		// �������Ϊ5�������µĸ���
		for (int j = 0; j < 5; j++) {
			variation(group);
			judge(old_group, group);
		}
		if (i % 100 == 0) {
			for (int j = 0; j < group_size; j++) {
				group[j].calculate_len();
				if (group[j].get_len() < best.get_len()) {
					best = group[j];
				}
			}
			cout << "��ǰ���Ž�Ϊ��" << best.get_len() << "  ��ǰ��������Ϊ��" << i << endl;
			for (int i = 0; i < N; i++) {
				outfile << (i + 1) << "\t" << best.get_path()[i] << endl;
			}
		}
		outfile2 << best.get_len() << endl;
	}
	outfile2.close();
	outfile.close();
	return best;
}

int main() {
	time_t start, finish;
	start = clock(); // ��ʼ��ʱ
	GA process;
	process.read_file();
	Path ans = process.get_answer();
	bool count[N] = { false };
	finish = clock(); //��ʱ���� 
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
	cout << "���·��: " << endl;
	for (int i = 0; i < N - 1; i++) {
		cout << ans.get_path()[i] << " -> ";
	}
	cout << ans.get_path()[N - 1]<<endl;
	cout << "���: " << (ans.get_len() - BEST) / BEST << endl;

	cout << "���·������: " << ans.get_len() << endl;

	cout << "��ʱ: " << duration << "sec" << endl;
	cout << endl;
}



