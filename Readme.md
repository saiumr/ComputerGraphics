## ComputerGraphics

### 介绍

计算机图形学光栅图形学算法，绘制基本图形的算法实现（直线、圆之类的）  

### 环境配置：cmake make SDL2 mingw64

配置好环境之后使用下面命令编译运行（注意将CMakeLists.txt文件中SDL2的路径修改为自己的路径）  

```bash
$ make cmake
$ make
$ .\gal.exe
$ make clean
```

### 操作

#### 全局操作

+ q：退出    
+ c：清屏  
+ a：转换功能（比如从绘制直线转换为区域填充）  

#### 局部操作

class ALGOLine：绘制直线的算法类  

+ s：转换绘制直线的算法  
+ 鼠标单击：每有两个单击点绘制一条直线段  

注：需要在全局转换到相应的功能下才能使用局部操作  



### 完成情况

#### ALGOLine：直线绘制算法

+ DDA直线算法（Red Line）  
+ 中点画线算法（Green Line）  
+ Bresenham直线算法（Blue Line）  
