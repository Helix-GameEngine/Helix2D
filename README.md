# Helix2D 0.0.1
##### 一个轻量好用的C++图形库

Helix2D是一个基于Direct2D的Windows图形库，该图形库注重于给予用户*入门简单*，*编写迅速*的体验，整合了其它图形库或游戏引擎的操作，并拓展出许多自身的特色。

#### 示例代码1

下面是一个基础的Helix2D程序：

该程序会创建一个名为`Helix2D`的窗口，大小为600*600
```C++
using namespace helix2d;

int main()
{
    Window win;
    
    while (true)
    {

    }
    return 0;
}

```


#### 示例代码2

下面是一个多窗口Helix2D程序：

该程序会创建两个分别名为`Window 1`和`Window 2`的窗口，大小分别为1000*1000，500*500
```C++
using namespace helix2d;

int main()
{
    Window win1{L"Window 1",1000,1000};
    Window win2{L"Window 2",500,500};

    while (true)
    {

    }
    return 0;
}
```

#### 还有更多

关于Helix2D的更多教程今后我会自己搭建一个网站，并提供以lib文件的形式下载安装，大家还请期待

目前Helix2D还只具备一些基础的小程序开发，但将来会根据用户的建议以及我自身的想法去更新，我也只是个新手，所以还请各位大佬多多关照！

**QQ交流群**：693409874