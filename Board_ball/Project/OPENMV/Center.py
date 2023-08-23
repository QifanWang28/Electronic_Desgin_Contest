import sensor, image, time, ustruct
from pyb import UART
uart = UART(3, 115200)#串口通信
#**********颜色/ROI阈值**********#
white = (25, 100, -128, 127, -128, 127)
#roi1=(100,52,200,70)#水平、竖直、长、宽
roi1=(80,45,250,90)#水平、竖直、长、宽
#**********摄像头参数**********#
sensor.reset()
sensor.set_vflip(False)   #垂直翻转、水平翻转
sensor.set_hmirror(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # 320x240
#sensor.set_windowing([0,20,80,40])
sensor.set_auto_whitebal(True)     #自动白平衡
sensor.skip_frames(time = 2000)     # 设置生效
#roi=

def center():

    white_blobs = img.find_blobs([white],roi=roi1,y_stride=5)
    for blob in white_blobs:
        img.draw_rectangle(blob.rect(), color=(255,0,0))
        a=blob.cx()
        b=0
        if (a!=198)&(a!=197):
            print(a)
            b=a

        if(b!=0):
            print(b)
            FH = bytearray([0x2C,0x12,b,0x5B])
            uart.write(FH)
while(True):

    img=sensor.snapshot().binary([white])
    img.draw_rectangle(roi1)
    center()
