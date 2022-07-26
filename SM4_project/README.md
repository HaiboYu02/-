# 根据对称加密方案SM4实现完整的加密算法，并利用Ttable技术对s盒替换、移位和异或阶段进行优化，构造4个8进32bit出的大S盒

Author：于海波

## 代码说明
  代码共包含1个头文件和一个cpp文件：
  
  在define.h文件中声明了SM4算法所需的所有函数；
  
  在SM4_project.cpp中对头文件中的函数依次实现，t函数会因宏定义的version变量的值的不同而选择执行标准的SM4算法还是优化后的Ttable方式。
  
## 运行指导
  修改宏定义的变量version的值为1或2一次运行main函数，得到关于相同明文的密文。将注释部分取消注释也可对比二者运行速度。
  
## 运行截图
  SM4 Ttable实现方法：
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic7.png)
  
  原始SM4实现方法：
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic8.png)
