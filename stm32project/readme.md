## 思路

首先是读取MPU寄存器的值,通过加速度(三轴的加速度与重力加速度的比值,再用反三角函数求得夹角),通过夹角来判断是否水平.

使用了四个LED灯来表示,当水平时,四个LED都保持常亮状态,向哪个方向倾斜,哪个方向的灯就会熄灭.

接下来是读取HMC得到的三轴磁感应强度,同样通过反三角函数求解(只用到了x轴和y轴),就可以得到x轴指向的角度(0~360,北极时为0°),而当指向HMC上x轴指向北极的时候,红色的灯就会亮起,作为指示.

最后是目标点的功能(这个功能的LED不知道为啥亮不了,代码感觉没有问题(汗)),是用了两个串口,一个是ATGM336H连接到stm32上,一个是电脑连在stm32上,通过文本数据包,向stm32发送目标点的经度纬度

![image-20230826223645083](C:\Users\pwqc\AppData\Roaming\Typora\typora-user-images\image-20230826223645083.png)

(帧尾是回车,帧头是@)

对于W和S这两个前缀,都对相应的数取负数方便计算

然后是读取ATGM336H发来的数据,通过strtok对字符串进行裁剪得到相应的数据,再与目标点的数据结合进行计算,得到角度,最后与HMC的实时角度进行比对,当匹配成功时,同样是HMC芯片的x轴方向指向目标点方向时,此时,蓝色的灯亮起,从而表现目标点的方向.