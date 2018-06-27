# 同济大学软件学院C++期末项目
## 自选项目——弓箭手大作战
> 小组成员: 张喆(组长), 黄安宁, 王菲凡

### 游戏介绍
> 随着等级的升高

> 你会获得更多的技能

>  好好利用它们

> 去给你的对手一击痛击

> 欢迎来到——弓箭手大作战!

### 基础功能
- 地图绘制, 人物绘制, 道具绘制, 技能绘制
- 支持鼠标和键盘交互
- 支持障碍物
- 支持HP道具和EXP道具在地图随机产生
- 实现服务端：⽀持互联网联机对战（⾃由对抗模式），且⽀持双人模式, 可在同⼀个地图游戏
- ⽀持动画效果 (包括人物走动动画, 子弹发射动画, 人物受伤的动画, 场景切换的动画)
- 支持 UserDefault 储存部分玩家选定的游戏参数
### 扩展功能
- 支持三张不同地图的绘制, 四个不同人物的绘制, 由于多张地图的存在, 默认支持房间列表(但没有显示表现)
- 支持两种游戏模式 (玩家可自由切换)
     1. 键盘wasd控制人物八个方向自由移动, 鼠标点击发射子弹
     2. 人物自动朝鼠标所在方向移动, 空格用来发射子弹
- 支持19种道具 (具体功能请参考 *游戏帮助*)
- 支持4种武器
- 支持中英两种系统语言
- 支持屏幕尺寸自由切换 (三种尺寸随时切换)
- 支持地图缩放(发布版本暂未提供,答辩中将展示)
- 支持小地图,且可自由开启和关闭
- 支持滑动屏幕查看帮助信息

### 游戏玩法
1.开始游戏之前，选择你喜欢的地图和人物（联网模式下，地图选择相同的玩家会被安排到同一个房间，目前房间上限人数2人，请不要超出房间容量）
2.进入游戏之后可以选择单机模式或联网模式，默认是单机模式，如果想切换到联网模式，请点击右上角的WIFI按钮
> 请注意：刚连接好网络的时候不要急着移动人物开始游戏，等到你的朋友也进入房间之后再开始双人模式（你可以在左上角的小地图里看到当前所有玩家的情况）

3.地图上会随机产生回血道具和能量道具，吃到更多的能量道具来提升等级
4.没升一级会获得一个道具，充分利用他们来给你的对手一击痛击（19种道具的详细技能请运行游戏并查看游戏帮助）
5.每个玩家有3条生命，留到最后的玩家获得最终的胜利

### C++特性使用情况
- Lambda表达式
- 类型自动判断
- 范围for的使用
- 初始化列表
- vector map 等多种STL容器和相应方法的使用
- 迭代器

### 服务器实现的功能：
1. 对客户端消息进行定向转发
2. 在客户端非正常退出后自动判定离开游戏，并广播其他用户
3. 多人游戏的房主轮换机制

### 网络的搭建
1. 利用基于 Chrome V8 引擎的 JavaScript 运行环境Node.js 搭建服务器端，实现异步数据传输。
2. 封装C++模块进入node.js，提高多人游戏时服务器判定效率，后来功能改变遂移除。
3. 客户端利用Cocos2d-x中封装的SocketIO协议与服务器进行通信。
4. 研究库文件，实现在场景切换后保持网络连接。为解决客户端联网后与单机模式冲突问题，与WebSocket不同，因SocketIO库中的类大量使用“private”，不得不在库文件中增添或略微修改内容以解决判定BUG。*（尝试新建类继承Socket::Delegate并储存数据来实现跨场景，因SocketIO.h中大量private定义，出现无法修复的BUG）*
5. 服务器的启动: 运行js脚本
6. 客户端的连接采用 SocketIO::connect, 提供给用户的借口请详见游戏里的帮助界面

### 部分游戏截图
![](http://a4.qpic.cn/psb?/V11JkC4u1YdnGY/ZjqFAKvvGkaAMpVjiMoy1XhkU0lnne8zeVoNd9y62VE!/c/dN8AAAAAAAAA&ek=1&kp=1&pt=0&bo=aAFoAWgBaAEDGTw!&tl=1&vuin=2871811733&tm=1529924400&sce=60-2-2&rf=0-0)

![](http://a2.qpic.cn/psb?/V11JkC4u03NHtr/usMG3YXyVAzJUyKoc5mjieUDyRouNn7kRvXOh5Ci34Q!/m/dIkBAAAAAAAA&ek=1&kp=1&pt=0&bo=AQXSAgEF0gIDSWw!&tl=1&vuin=2871811733&tm=1529856000&sce=60-4-3&rf=0-0)

![](http://a4.qpic.cn/psb?/V11JkC4u03NHtr/kNlaTlpYyYSd39kSRsTVfOsDA4IreYPSPfVVJNTqG3U!/m/dNsAAAAAAAAA&ek=1&kp=1&pt=0&bo=AgXOAgIFzgIDGTw!&tl=1&vuin=2871811733&tm=1529856000&sce=60-4-3&rf=0-0)

![](http://a3.qpic.cn/psb?/V11JkC4u03NHtr/OvDutxGhrpua.Y5kHLh6Ti7e6sWqkVh9iDZRQ3zVhhE!/m/dOYAAAAAAAAA&ek=1&kp=1&pt=0&bo=AAXSAgAF0gIDKQw!&tl=1&vuin=2871811733&tm=1529856000&sce=60-4-3&rf=0-0)

![](http://a1.qpic.cn/psb?/V11JkC4u03NHtr/by*.KGURojnQI3CToWRGuXZMIr15E.iKwbAHu9mripw!/m/dPQAAAAAAAAA&ek=1&kp=1&pt=0&bo=AgXSAgIF0gIDKQw!&tl=1&vuin=2871811733&tm=1529856000&sce=60-4-3&rf=0-0)

![](http://a2.qpic.cn/psb?/V11JkC4u03NHtr/eAleq20QE9Ks9TMhQItR4fQAujOjXRHs9ZFHLX8GluE!/m/dA0BAAAAAAAA&ek=1&kp=1&pt=0&bo=BgbgAQYG4AEDKQw!&tl=1&vuin=2871811733&tm=1529856000&sce=60-4-3&rf=0-0)
