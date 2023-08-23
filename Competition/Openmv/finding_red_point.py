import sensor, image, time
from pyb import UART,LED
from pyb import millis
sensor.reset() # 初始化摄像头模块.
sensor.set_pixformat(sensor.RGB565) # 或者使用 sensor.RGB565 彩色
sensor.set_framesize(sensor.QVGA) # 或者使用 sensor.QVGA (or others)
sensor.set_auto_gain(False)
sensor.skip_frames(20)
sensor.set_auto_whitebal(False)
red_threshold = (30, 100, 15, 127, -40, 127)
clock = time.clock()

time_init=millis()

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
while True:
    clock.tick()
    img = sensor.snapshot().lens_corr(strength=1.6, zoom=1.0)
    time_now = millis()
    time_diff = time_now-time_init
    second = time_diff//1000%60
    blobs = img.find_blobs([red_threshold])
    if blobs:
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        if cx is not None and cy is not None:
        # 绘制绿色十字来表示激光笔的位置
          img.draw_cross(cx, cy, color=(0, 255, 128))
    img = img.to_rgb565()
    if(blobs and second==0.1):
        time_init = millis()
        second=0
        uart.write([(cx,cy)])
   # print(clock.fps())
