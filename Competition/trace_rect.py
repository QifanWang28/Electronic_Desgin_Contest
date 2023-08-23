import sensor, image, lcd
import pyb

#初始化摄像头
sensor.reset() # 初始化摄像头模块.
sensor.set_pixformat(sensor.GRAYSCALE) # 或者使用 sensor.RGB565 彩色
sensor.set_framesize(sensor.QVGA) # 或者使用 sensor.QVGA (or others)
sensor.skip_frames(time = 2000) #延时让摄像头文稳定.
sensor.set_gainceiling(8) #设置增益，这是官方推荐的参数
time_init= pyb.millis()
while(True):
    time_now = pyb.millis()
    time_diff = time_now-time_init
    second = time_diff//1000%60
    img = sensor.snapshot() # 拍摄并返回图像.
    #使用 Canny 边缘检测器
    img.find_edges(image.EDGE_CANNY, threshold=((14, 98)))
    rect = img.find_rects( threshold=240000)
    if(second>=0.02):
      if(rect):
       time_init=pyb.millis()
       second = 0
       if(rect):
        if(rect[0][4]<=260000):
         img.draw_rectangle(rect[0][0],rect[0][1],rect[0][2],rect[0][3],fill = True)
         print(rect[0][4])

