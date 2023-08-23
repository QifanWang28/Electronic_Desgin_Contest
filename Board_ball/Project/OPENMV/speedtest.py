import sensor, image, time, ustruct
from pyb import UART
uart = UART(3, 115200)#串口通信
#**********颜色/ROI阈值**********#
white = (42, 100, -128, 127, -128, 127)
#white = (55, 100, -128, 127, -128, 127)

roi1=(15,10,250,225) #水平、竖直、长、宽
#**********摄像头参数**********#
sensor.reset()
sensor.set_vflip(False)   #垂直翻转、水平翻转
sensor.set_hmirror(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # 320x240
#sensor.set_windowing([0,20,80,40])
sensor.set_auto_whitebal(True)     #自动白平衡
sensor.skip_frames(time = 2000)     # 设置生效
clock = time.clock()
a=0
b1=0
c=0
def center():
    white_blobs = img.find_blobs([white],roi=roi1)
    for blob in white_blobs:
        w=blob.w()
        h=blob.h()
        if((3<w<10)&(3<h<10)):
            img.draw_rectangle(blob.rect(), color=(255,0,0))
            a=blob.cx()
            b=blob.cy()
            return a,b
while(True):
    clock.tick()
    img=sensor.snapshot().binary([white])
    img.draw_rectangle(roi1, color=(51,204,204))

    pl=center()
    c=pl[0]
    r=pl[1]
    if(pl):
        #d=(int)((c-b1)*clock.fps())#初始有一个b1=0异常值要排除
        print(c)#原像素位置
        print(r)
        FH1 = bytearray([0x2C,0x12,c,r,0x5B])
        uart.write(FH1)
        if(-255<d<255):
            print(d)
            if(d>=0):
                e=0
                FH = bytearray([0x2C,0x12,c-80,e,d,0x5B])
                #uart.write(FH)
            else:
                e=1
                FH = bytearray([0x2C,0x12,c-80,e,d,0x5B])
                #uart.write(FH)
        b1=c

