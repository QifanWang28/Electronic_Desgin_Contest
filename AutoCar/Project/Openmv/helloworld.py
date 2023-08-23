
# Hello World 示例
#
# 欢迎使用OpenMV IDE!点击下面绿色的运行箭头按钮运行脚本!
#用usb线与电脑连接后，打开 文件——examples——01Basic——helloworld.py例程，点击上面的齿轮按钮运行。

import sensor, image, time
#引入此例程依赖的模块，sensor是与摄像头参数设置相关的模块，image是图像处理相关的模块，
#time时钟控制相关的模块。import相当于c语言的#include <>,模块相当于c语言的库。

sensor.reset()                      # 复位并初始化传感器。
#初始化摄像头，reset()是sensor模块里面的函数

sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种

sensor.set_framesize(sensor.QVGA)   # 将图像大小设置为QVGA (320x240)
#设置图像像素大小，sensor.QQVGA: 160x120，sensor.QQVGA2: 128x160 (一般用于LCD
#扩展板)，sensor.QVGA: 320x240，sensor.VGA: 640x480, sensor.QQCIF: 88x72，sensor.QCIF: 176x144，sensor.CIF: 352x288

sensor.skip_frames(time = 2000)     # 等待设置生效。
clock = time.clock()                # 创建一个时钟对象来跟踪FPS帧率。
#初始化时钟

while(True):
    #python while循环，一定不要忘记加冒号“：”
    clock.tick()                    # 更新FPS帧率时钟。
    img = sensor.snapshot()         # 拍一张照片并返回图像。
     #截取当前图像，存放于变量img中。注意python中的变量是动态类型，不需要声明定义，直接用即可。

    print(clock.fps())
    # 注意: 当连接电脑后，OpenMV会变成一半的速度。当不连接电脑，帧率会增加。
    #打印当前的帧率。
