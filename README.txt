模拟退火：
src：
	LocalSearch.cpp 使用局部搜索实现
	SimulatedAnnealing.cpp 在局部搜索的基础上使用模拟退火实现
	visualization.py 实现可视化

tc：
	运行结果：
		path_LS.txt 局部搜索获得的path
		path_SA.txt 模拟退火获得的path
		len.txt 模拟退火获得的每次温度下的最优解（路径长度）

	可视化结果：
		image_局部搜索 中存放了局部搜索的路径变化的过程图
		gif_局部搜索 为局部搜索的路径变化的动图
		image_模拟退火 中存放了模拟退火的路径变化的过程图
		gif_模拟退火 为模拟退火的路径变化的动图

遗传算法：
src：
	GA.cpp 遗传算法
	visualization.py 实现可视化

tc：
	运行结果：
		path.txt 每100次迭代获得的最优解path
		len.txt 每100次迭代获得的最优解路径长度

	可视化结果：
		image 中存放了路径变化的过程图
		gif 为路径变化的动图




