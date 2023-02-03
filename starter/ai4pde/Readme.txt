Data:
Eq: -div (\kappa grad p) = f
Mapping: 椭圆系数\kappa -> 解p
计算区域为[0,1]^2，求解方法为有限元，剖分区域为32*32的网格

LevRand_DarcyNN: K为32*32*2个三角形单元上的系数的值，方程的解同样为33*33个网格顶点(自由度)处的值,共有1000组数据，
        k_1, y\in [0,0.25]
kappa = k_2, y\in [0,25,0.75]
        k_3, y\in [0.75,1]
每个三角形单元上的值是相互独立的，服从正态分布
k_1\sim N(3,0.5)
k_2\sim N(2,0.5)
k_3\sim N(7,0.5)

Code:
模型参数：
f_d: Channel数量
c_grid_size: 粗网格尺寸
coarse_vel: 全联接层隐藏层宽度


2. Darcy_Rand.py
总数据1000，一般选择前80%数据为训练集，后20%数据为测试集，即N=0.8*1000
设置好训练集数量N，和网络参数f_d,c_grid_size,coarse_vel，epoch数量num_epochs,即可运行





