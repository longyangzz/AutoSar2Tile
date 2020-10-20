# 根据监视的目录对点云数据切片输出

1、setting中存储的监视目录和数据输出目录

2、log文件为日志输出

3、切片方法为使用gdal 和 entwine切片

4、原始数据为指定兰州区域的4列xyzs的txt格式数据

5、已知的点云数据坐标系为EPSG::4542

6、文件名称不能包含空格、文件夹目录不能包含空格



ctb：需要使用4326的坐标系，3857的不识别

gdal2tiles切片xyz：可以使用3857，不识别CGCS



## 1.1点云切片如何使用

1、配置setting.xml中的本地文件目录；

2、配置gdal环境变量

3、使用例子数据放入到监控目录里，输出目录下有数据生成，即配置成功，可正常运行。



## 1.2切片结果pnts如何可视化

1、发布iis服务：F:\IISDataCenter\China\gansu\baigedem【http://localhost:8062/baige/】

2、基于cesium以http服务形式加载3dtiles：

​     个人电脑上使用[H:\2.code\github\Cesium-1.70\Apps\Sandcastle\demo\CZML Polygon.html]例子改写测试，并将该html存放到本项目下名称为[pntsDEMO.html]

## 2.1tiff切片如何使用

1、配置setting.xml中的本地文件目录；

2、setting.xml中配置

<image zoom="10-18" python="C:\Users\wly\AppData\Local\Programs\Python\Python37\python.exe">
    </image>

3、配置gdal环境变量

4、安装gdal的python绑定  

···

5、使用例子数据放入到监控目录里，输出目录下有数据生成，即配置成功，可正常运行。

## 2.2 tif切片后的tile img怎么在cesium可视化

![image-20200928201232223](H:\2.code\github\AutoSar2Tile\screenshot\image-openlayers.png)

1、输出的切片目录建立iis服务

2、在cesium中使用xyz格式调用

## 2.3 使用的gdal2tiles细节说明

1、使用的脚本存放于scirpts目录下；

2、gdal2tiles需要使用像素深度为8位的数据，使用镶嵌到新栅格可以处理；

3、测试成功的例子使用的原始坐标为4326

4、测试数据及命令为：

python H:\2.code\github\AutoSar2Tile\AutoSar2Tile\x64\AutoSar2Tile\scripts\gdal2tiles.py D:\data\fsdsar\test\tongluclip8.tif D:\output\tonglu --zoom=0-18 --s_srs=EPSG:4326 --xyz

5、使用的python下载地址为：https://github.com/CjS77/gdal2tiles https://github.com/longyangzz/gdal2tiles-1

6、切片结果发布为iis服务，设置跨域http响应头，添加Access-Control-Allow-Origin 值为*即可。

7、在cesium中调用示例为：

```
var xyz2 = new Cesium.UrlTemplateImageryProvider({
  url: 'http://localhost:8063/{z}/{x}/{y}.png', //服务地址
  //tilingScheme: new Cesium.GeographicTilingScheme()
})

var viewer = new Cesium.Viewer("cesiumContainer", {
  imageryProvider: xyz2
});

viewer.camera.flyTo({
          destination : Cesium.Cartesian3.fromDegrees(119.18871379902474,29.75981714010474, 150.0)
        });
```

8、测试成功的数据及调用html存放于data目录下的test子目录

## 3.1 csv中点坐标及标量色识别切片

原始数据格式

![image-20201013114532650](H:\2.code\github\AutoSar2Tile\screenshot\image-csvdata.png)

提取有效数据列：EFG列是坐标，J列是位移



颜色条范围：Blue>Green>Yellow>Red

颜色阶数：50

![image-20201013171439449](H:\2.code\github\AutoSar2Tile\screenshot\image-colorscale.png)

切片结果目录：

![image-20201013171516227](H:\2.code\github\AutoSar2Tile\screenshot\image-csvcloud.png)