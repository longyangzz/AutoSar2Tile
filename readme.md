# 根据监视的目录对点云数据切片输出

1、setting中存储的监视目录和数据输出目录

2、log文件为日志输出

3、切片方法为使用gdal 和 entwine切片

4、原始数据为指定兰州区域的4列xyzs的txt格式数据

5、已知的点云数据坐标系为EPSG::4542

6、文件名称不能包含空格、文件夹目录不能包含空格



## 如何使用

1、配置setting.xml中的本地文件目录；

2、配置gdal环境变量

3、使用例子数据放入到监控目录里，输出目录下有数据生成，即配置成功，可正常运行。



## 切片结果pnts如何可视化

1、发布iis服务：F:\IISDataCenter\China\gansu\baigedem【http://localhost:8062/baige/】

2、基于cesium以http服务形式加载3dtiles：

​     个人电脑上使用[H:\2.code\github\Cesium-1.70\Apps\Sandcastle\demo\CZML Polygon.html]例子改写测试，并将该html存放到本项目下名称为[pntsDEMO.html]