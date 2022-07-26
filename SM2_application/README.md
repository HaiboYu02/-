# SM2应用实现：ECMH集合的哈希，PGP方案，两方签名

  Author：于海波

## 代码说明
  
### ECMH集合上的哈希
  
  该项目成功实现了集合上的哈希,生成椭圆曲线的公私钥对。主函数中定义了椭圆曲线的相关参数，前面部分定义了椭圆曲线中加法函数，乘以常数函数以及求取集合哈希的函数，
关键在于将调用SM3得到的哈希值作为椭圆曲线的横坐标，利用Tonelli-Shanks算法求二次剩余得到纵坐标值，充分利用了椭圆曲线上加法可逆的性质，并调用求集合哈希的函数验证，
符合椭圆曲线加法运算可逆的性质。该项目在ECMH_project.py中实现。

### PGP消息发送方案

  通话双方提前协商好一对SM2公私钥对（pk_r,sk_r），消息发送方生成一个随机AES对称加密密钥K，用K加密消息M，同时用pk_r加密K，和加密后的消息一并发送给接收方。 
接收方收到后，用sk_r解密得到K后，再解密得到消息M。其中（pk_r,sk_r）的生成直接调用ECMH_project.py中的定义。该项目在PGP_project.py中实现。

### 2p_sign/2p_decrypt两方签名两方解密

  SM2两方签名与传统的SM2签名相比，完成签名的双方各自掌握一部分信息，但都无法得出签名私钥d的值，SM2两方签名需要签名双方共计交互五次，最后得到消息的签名值(r,s),
其中椭圆曲线上相关运算的实现 仍需调用ECMH_projecy.py。该项目在2p_sign.py中实现。
  
  
## 运行指导
  运行各自对应的py文件即可。
  
## 运行截图
  ECMH集合哈希
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic10.png)
  
  PGP方案
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic11.png)
  
  2psign两方签名
  
  ![image](https://github.com/HaiboYu02/img-storage/blob/main/pic12.png)
  
