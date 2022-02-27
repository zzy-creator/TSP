import pandas as pd
import matplotlib.pyplot as plt
import imageio
import os
import os.path

N = 150


def read_tsp_data(filePath):
    city = pd.read_table(filePath, sep='\n', header=None)  # 读入txt文件，分隔符为\n
    city.columns = ['x']
    city['y'] = None

    for i in range(len(city)):  # 遍历每一行
        coordinate = city['x'][i].split()  # 分开第i行，x列的数据。split()默认是以空格等符号来分割，返回一个列表
        city['x'][i] = float(coordinate[1])  # 分割形成的列表第2个数据给x列
        city['y'][i] = float(coordinate[2])  # 分割形成的列表第3个数据给y列

    # print(city)

    return city


def read_tsp_path(filePath):
    paths = []
    pathData = pd.read_table(filePath, sep='\n', header=None)
    pathData.columns = ['x']

    j = 0
    while j < len(pathData):
        path = []
        for i in range(j, j + N):  # 遍历某个解的路径
            coordinate = pathData['x'][i].split()
            path.append(int(coordinate[1]) - 1)  # 将该解的路径存在path中
        paths.append(path)  # 每获得一个路径，添加到paths中
        j += N

    return paths


def read_tsp_optimum(filePath):
    pathData = pd.read_table(filePath, sep='\n', header=None)
    pathData.columns = ['x']
    path = []
    i = 0
    while i < len(pathData):
        Str = pathData['x'][i]
        path.append(float(Str))
        i += 1
    return path


# Create the fig.
def create_figure(citys, paths):
    for j in range(len(paths)):
        path = paths[j]

        # 按路径获得依次存入每个城市的坐标至x[] y[]
        x = []
        y = []
        for i in range(len(citys)):
            x.append(citys['x'][path[i]])
            y.append(citys['y'][path[i]])
        x.append(citys['x'][path[0]])
        y.append(citys['y'][path[0]])

        # 绘图
        plt.plot(x, y, linewidth=1.0)  # 画线
        plt.scatter(x, y, s=10, color='g', marker=',')  # 画点

        # 存图
        plt.savefig("./image/" + str(j) + ".png")

        # 清理
        plt.clf()
        # plt.show()


def create_figure_optimum(optimum):
    x = []
    y = []
    for i in range(len(optimum)):
        x.append(optimum[i])
        y.append(i)
    plt.plot(y, x, linewidth=1.0)
    plt.xlabel("Annealing times")  # X轴标签
    plt.ylabel("Current optimal")  # Y轴标签
    plt.title("TSP simulated annealing")  # 标题
    plt.show()


def create_gif(gif_name, paths, duration=0.3):
    frames = []
    for j in range(len(paths)):
        # 读取 png 图像文件
        frames.append(imageio.imread("./image/" + str(j) + ".png"))
    # 保存为 gif
    imageio.mimsave(gif_name, frames, 'GIF', duration=duration)

    return


citys = read_tsp_data("tspdata.txt")
paths = read_tsp_path("path.txt")
optimum = read_tsp_optimum("len.txt")
create_figure_optimum(optimum)
create_figure(citys, paths)
gif_name = 'created_gif.gif'
path = './image'  # 指定文件路径
duration = 0.3
create_gif(gif_name, paths, duration)