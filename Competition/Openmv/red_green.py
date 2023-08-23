import sensor, image, time
import pyb
import math
from pyb import UART

sensor.reset()  # 初始化摄像头模块.
sensor.set_pixformat(sensor.RGB565)  # 或者使用 sensor.RGB565 彩色
sensor.set_framesize(sensor.QVGA)  # 或者使用 sensor.QVGA (or others)
sensor.set_auto_gain(False)
sensor.skip_frames(20)
#sensor.set_auto_exposure(False, exposure_us=1400)
sensor.set_auto_whitebal(False)
clock = time.clock()
uart = UART(3, 115200)  # 串口通信
red_threshold = (30, 100, 15, 127, -40, 127)  # 红色激光笔的颜色阈值
green_threshold = (0, 82, -128, 7, -109, 27)  # 绿色十字的颜色阈值
enable1 = 0
enable2 = 0
errx = 500
erry = 500
red_x = 500
red_y = 500
green_x = 500
green_y = 500

def find_blob_center(threshold,judge):
    blobs = img.find_blobs([threshold],roi =(86,17,206,201),invert = judge )
    if blobs:
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        return cx, cy
    else:
        return None, None

while True:
    clock.tick()
    img = sensor.snapshot().lens_corr(strength=1.05, zoom=1.6)

    # 检测红色激光笔的位置
    red_x, red_y = find_blob_center(red_threshold,False)
    #green_x,green_y = find_blob_center(green_threshold,True)
    enable1 = 0
    enable2 = 0
    if red_x is not None and red_y is not None:
        # 绘制绿色十字来表示激光笔的位置
        img.draw_cross(red_x, red_y, color=(0, 255, 128))
        enable1 = 1
    #if green_x is not None and green_y is not None:
        #img.draw_cross(green_x, green_y, color=(255, 0, 0))
        #enable2 = 1
    #if(enable1 and enable2):
        #errx = red_x - green_x
        #erry = red_y - green_y

    # 显示图像
    #img.draw_cross(80, 60, color=(255, 0, 0))  # 中心位置绘制红色十字
    #img = img.to_rgb565()
        print(red_x, red_y,green_x,green_y,errx,erry)
        FH = bytearray([0x2C,0x12,errx,erry,0x5B])
        uart.write(FH)
    #print(clock.fps())
