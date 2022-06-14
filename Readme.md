## ComputerGraphics
  
### 介绍  
计算机图形学光栅图形学算法，绘制基本图形的算法实现（直线、圆之类的）  
  
### cmake make SDL2 mingw64  
配置好环境之后使用下面命令编译运行（注意将CMakeLists.txt文件中SDL2的路径修改为自己的路径）  
```bash  
$ make cmake
$ make
$ .\gal.exe
$ make clean
```  

### 操作  
任意单击两个位置绘制直线，c键清屏，s键转换绘制直线的算法（各个算法绘制的直线颜色不同），q键退出程序  

### 完成情况  
+ DDA直线算法（Red Line）  
+ 中点画线算法（Green Line）  
+ Bresenham直线算法（Blue Line）  

