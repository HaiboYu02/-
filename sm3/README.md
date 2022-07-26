# sm3的长度扩展攻击和生日攻击

Author：于海波

## 代码说明
  在头文件中通过两种方式实现sm3摘要计算方法，分别为调用openssl库的sm3_hash方法，第二种为根据算法迭代过程用c语言实现完整的sm3过程；
  
  在cpp文件中可通过调控参数选择执行长度扩展攻击或者生日攻击，实现整个摘要值的哈希碰撞所需的时间复杂度较高，所以选择和给定消息的哈希值前16bit完全一致就认为找到了一个碰撞，
并打印出穷举次数，计算哈希时使用openssl方法；

  选择长度扩展攻击时，由于需要用到压缩函数，在本项目中为_CF，在输出中将由已知消息和哈希值构造另一对通过验证的消息和哈希值的构造过程给出了展示。
  
## 运行指导
  #define Birthday_Attack 时执行生日攻击
  
  #define Length_Extension_Attack 时执行长度扩展攻击
  
## 运行截图
  长度扩展攻击
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic1.png)
  
  生日攻击
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic2.png)
  
