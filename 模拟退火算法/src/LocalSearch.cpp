#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
#define N 150 // ��������
#define LOOP_TIME 500000

int city_list[N]; // ���һ����
double city_pos[N][2];//��ų��е�����

double distance(double* city1, double* city2) {
	//������������֮���ֱ�߾���

	double x1 = city1[0];
	double y1 = city1[1];
	double x2 = city2[0];
	double y2 = city2[1];

	double dis = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return dis;
}

double path_len(int* list) {
	//������·������

	int index = list[0],s,e;
	double path = 0,dis;

	for (int i = 0; i < N - 1; i++) {
		s = list[i]; // ���
		e = list[i+1]; // �յ�
		dis = distance(city_pos[s - 1], city_pos[e - 1]);
		path += dis;
	}

	//�������һ�����лص����
	int last = list[N-1];
	int first = list[0];
	path += distance(city_pos[last - 1], city_pos[first - 1]);

	return path;
}

void init() {
	//ʵ�֣������е�������ⲿ�ļ���tspdata.txt���ж��벢�洢��������һ����ʼ��

	int k = 0;//k��ʾ��ǰ�ǵ�k������
	double index, x, y;
	ifstream infile;
	infile.open("tspdata.txt");

	if (!infile.is_open()) {
		printf("Open file failed!\n");
	}
	//�������겢�洢��city_pos[][]��
	while (!infile.eof()) {
		infile >> index >> x >> y;
		city_pos[k][0] = x;
		city_pos[k][1] = y;
		k++;
	}
	// ��ʼ��һ����
	for (int i = 0; i < N; i++) {
		city_list[i] = i + 1;
	}
}

void mutation() {
	//�ֲ��������������еĽ�

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
			int temp = city_list[i];
			city_list[i] = city_list[j];
			city_list[j] = temp;
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
		int temp = city_list[p2];
		for (int i = p2-1;i >= p1;--i) {
			city_list[i + 1] = city_list[i];
		}
		city_list[p1] = temp;
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
		memcpy(path_temp, city_list, N * sizeof(int));
		city_list[0] = path_temp[p1];
		city_list[1] = path_temp[p2];
		int pos = 2;
		for (int i = 0; i < N; i++) {
			if (i == p1 || i == p2) continue;
			city_list[pos++] = path_temp[i];
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
		int temp = city_list[p1];
		city_list[p1] = city_list[p2];
		city_list[p2] = temp;
	}
}

int main() {
	srand((unsigned)time(NULL)); // ��ʼ�����������

	time_t start, finish;
	start = clock(); // ��ʼ��ʱ
	double T;

	init();

	int city_list_prev[N]; // ���ڱ�����һ����

	ofstream outfile;
	outfile.open("path.txt");
	if (!outfile.is_open()) {
		printf("Open outfile failed!\n");
	}

	int count = 0;
	double f1, f2, df;
	while (count <= LOOP_TIME) {
		memcpy(city_list_prev, city_list, N * sizeof(int)); // ��������
		mutation(); // �����½�
		f1 = path_len(city_list_prev);
		f2 = path_len(city_list);

		// ���½���ţ������
		if (f2 < f1) {
			cout << "��ǰ���Ž�: " << path_len(city_list) << endl;
		}
		// ���򱣴�ɽ�
		else {
			memcpy(city_list, city_list_prev, N * sizeof(int));
		}
		if (count % 10000 == 0) {
			for (int i = 0; i < N; i++) {
				outfile << (i + 1) << "\t" << city_list[i] << endl;
			}
		}
		count++;
	}
	outfile.close();

	finish = clock();
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;

	cout << "���·����" << endl;
	for (int i = 0; i < N - 1; i++) {
		cout << city_list[i] << "-->";
	}
	cout << city_list[N - 1]<<endl;
	cout << "���·������: " << path_len(city_list) << endl;
	cout << "���: " << (path_len(city_list) - 6528) / 6528<< endl;
	cout << "��ʱ: " << duration << "sec" << endl;

	return 0;
}
