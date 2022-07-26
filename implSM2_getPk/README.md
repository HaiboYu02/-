# 从SM2签名中获取公钥

  Author：于海波

## 代码说明
  在以太坊中不会为每个签名值单独开辟一段空间存储公钥值，但是从签名中获取公钥Pk。
  
  调用密码学库gmssl可以完成国密算法sm2的签名验证过程，但需要编写不分函数实现sm2的密钥生成过程，这一部分关键在于定义椭圆曲线上的加法运算和点与数字的乘法运算。
  
  根据SM2签名的特定表达式可以从签名(r,s)中推导出Pk的表达式。
  
## 运行指导
  修改宏定义的变量version的值为1或2一次运行main函数，得到关于相同明文的密文。将注释部分取消注释也可对比二者运行速度。
  
## 运行截图
  SM4 Ttable实现方法：
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic7.png)
  
  原始SM4实现方法：
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic8.png)
