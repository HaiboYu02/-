# sm3的长度扩展攻击、生日攻击和rho算法寻找碰撞

Author：于海波

## 代码说明
  在头文件中通过两种方式实现sm3摘要计算方法，分别为调用openssl库的sm3_hash方法，第二种为根据算法迭代过程用c语言实现完整的sm3过程；
  
  在cpp文件中可通过调控参数选择执行长度扩展攻击或者生日攻击，实现整个摘要值的哈希碰撞所需的时间复杂度较高，所以选择和给定消息的哈希值前16bit完全一致就认为找到了一个碰撞，
并打印出穷举次数，计算哈希时使用openssl方法；

  选择长度扩展攻击时，由于需要用到压缩函数，在本项目中为_CF，在输出中将由已知消息和哈希值构造另一对通过验证的消息和哈希值的构造过程给出了展示。
  
  rho算法寻找一个sm3碰撞的实现在单独的文件夹sm3_rho中，为了提高运行效率，哈希的结果取高32bit，取一个初始字符串作为sm3哈希摘要算法的初始输入，用得到的32bit结果替代开始的输入值，并把值通过stl向量容器vector的insert方法插入到容器头部，每插入一个哈希结果前都用find方法从容器索引0开始查找是否已经存在要插入的哈希值，如果有就找到了一个环并进入反复循环。
  
## 运行指导
  #define Birthday_Attack 时执行生日攻击
  
  #define Length_Extension_Attack 时执行长度扩展攻击
  
  直接运行整个sm3_rho文件实现 rho算法寻找一个sm3碰撞
  
## 运行截图
  长度扩展攻击
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic1.png)
  
  生日攻击
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic2.png)
  
  rho算法
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic15.png)
