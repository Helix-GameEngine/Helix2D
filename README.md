# Helix2D v0.2.0
##### 一个轻量好用的C++图形库

Helix2D是一个基于Direct2D的Windows图形库，该图形库注重于给予用户*入门简单*，*编写迅速*的体验，整合了其它图形库或游戏引擎的操作，并拓展出许多自身的特色。

#### 示例代码1

下面是一个基础的Helix2D程序：

该程序会创建一个名为`Helix2D`的窗口，大小为600x600
```C++
using namespace helix2d;

int main()
{
    Window win;
    
    while (true)
    {
        Sleep(16);
    }
    return 0;
}

```


#### 示例代码2

下面是一个多窗口Helix2D程序：

该程序会创建两个分别名为`Window 1`和`Window 2`的窗口，大小分别为1000x1000，500x500
```C++
using namespace helix2d;

int main()
{
    Window win1{L"Window 1",1000,1000};
    Window win2{L"Window 2",500,500};

    while (true)
    {
        Sleep(16);
    }
    return 0;
}
```

#### 还有更多

关于Helix2D的网站已经搭建完毕~ 并提供以静态链接库(lib)的方式下载安装

**网站**：https://friespeter.github.io/helix2d.web/

B站(Bilibili)上有关于Helix2D的教程，

**个人空间**：https://space.bilibili.com/1755374307

**QQ交流群**：693409874