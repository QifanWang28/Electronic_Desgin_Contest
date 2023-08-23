# Untitled - By: mengfan_zheng - 周二 2月 22 2022
import sensor, image, time,lcd
from pid import PID
from pyb import UART,Pin,Timer,Servo
import pyb


OBJ_REAL_LM=15   #物体真实宽度 mm

# 生成50HZ方波，使用TIM3，channels 1,2
#控制舵机需要输出2.5%-12.5%的PWM
#0度----0.5/20*100=2.5;(要看实际的舵机是否支持0度，一般运动不到0度，需要把这个值调大一点)
#45度----1.0/20*100=5;
#90度----1.5/20*100=7.5;
#135度----2.0/20*100=10;
#180度----2.5/20*100=12.5;
# 生成50HZ方波
#控制舵机需要输出2.5%-12.5%的PWM
pan_servo=Servo(1)    #左右控制
tilt_servo=Servo(2)  #上下控制

pan_angle=90    #上下运动控制，设置一个初始角度
titl_angle=90   #左右舵运动控制，初始角度

#定义机械爪舵机运动角度
#输入：舵机运动到目标角度
#输出：无

def servo_control_angle(servo,servo_angle):
    if servo_angle<=0:
        servo_angle=0
    if servo_angle>=180:
        servo_angle=180
    percent=(servo_angle+45)/18
    servo.pulse_width_percent(percent)


#功能：目标距离计算
#输入：目标宽度或直径，单位：像素值,real_Lm ，目标实际长度或直径，一般取长+宽/2
#输出：距离 mm
#note:不同的镜头焦距和摄像头芯片对应的距离参数不同，具体可配合超声波测距模块进行距离参数标定
def obj_distance(obj_Lm):
    distance= 1600*OBJ_REAL_LM/obj_Lm   #obj_Lm为目标长度，分子为距离系数，不同的摄像头需要测定一下系数
    #print(distance)
    return distance


#功能：寻找最大的色块，计算方式为像素面积排序
#输入： 色块链表blobs，中心坐标X(blob[0]),中心坐标y(blob[1]),（像素宽）blob[2],（像素高）blob[3],
#输出： 最大色块max_blob
def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

ball_s=0
power=0

#THRESHOLD  = (35, 51, 32, 57, 17, 127) #黄色乒乓球#
THRESHOLD  =(13, 66, 23, 44, 17, 120)
#red_threshold =(61, 23, 21, 67, -7, 41)
##红色


pan_pid = PID(p=0.12, i=0, imax=90) #脱机运行或者禁用图像传输，使用这个PID
tilt_pid = PID(p=0.05, i=0, imax=90) #脱机运行或者禁用图像传输，使用这个PID

sensor.reset() # Initialize the camera sensor.
sensor.__write_reg(0x11,0x80) #图像传感器的帧率控制，80为30帧，81为15帧
sensor.set_pixformat(sensor.RGB565) # use RGB565.彩色图像
sensor.set_framesize(sensor.QCIF) # use LCD分辨率160x128
sensor.set_vflip(1)
sensor.set_hmirror(1)
#sensor.set_hmirror(True) #水平镜像，暂时不用
#sensor.set_vflip(True)  #垂直镜像，根据摄像头的安装位置调整
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.颜色追踪需要关闭白平衡
clock = time.clock() # Tracks FPS. 设置一个定时器用来计算帧率
#lcd.init() # Initialize the lcd screen.初始化LCD显示屏
#lcd.set_direction(0)#设置显示屏的显示方向，0为纵向，1为横向
img = sensor.snapshot()  #获取一张照片
blobs = img.find_blobs([THRESHOLD],
                         area_threshold=3000) #先运行一次找色块的接口，获取一个色块的元组
last_blobs = blobs   #让上一个色块的元组等于本次获取的元组，因为第一次获取的元组是没有目标的

time_init = pyb.millis()
while(True):
    img.draw_rectangle((80,64,30,30))
    time_now = pyb.millis()
    time_diff = time_now-time_init
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.
    lcd.display(img) # Take a picture and display the image.
    if last_blobs:  #如果上一张图像中找到了色块，就在色块周边扩展，然后在扩展后的ROI区域进行寻找色块，这样有利于追踪
         for b in blobs:
            x1 = b[0]-7
            y1 = b[1]-7
            w1 = b[2]+12
            h1 = b[3]+12
            #print(b.cx(),b.cy())
         roi2 = (x1,y1,w1,h1)
        # print(roi2)
         blobs = img.find_blobs([THRESHOLD],
                                    roi = roi2,
                                    area_threshold=3000)
         last_blobs = blobs
    else:       #如果没有找到色块，就重新计算全图的色块，并将全图色块信息保存到上一张图像上，这样下一次就可以再进行ROI区域寻找色块
        blobs = img.find_blobs([THRESHOLD],
                                    area_threshold=3000)
        last_blobs = blobs
    if blobs:  #如果找到了色块，就计算最大的色块位置，并做舵机追踪
        max_blob = find_max(blobs)
        #色块的位置和大小需要满足图像本身大小的要求，以免越界
        if  max_blob.cx()>0 and max_blob.cx()<img.width() and max_blob.cy()>0 and max_blob.cy()<img.height() :

            ball_s=obj_distance((max_blob[2]+max_blob[3])/2) #计算距离
            pan_error = img.width()/2-max_blob.cx()   #左右控制的偏差
            tilt_error =img.height()/2-max_blob.cy() #上下控制的偏差
            tilt_error = -tilt_error
            #pan = -tilt_error
            if(time_diff//1000%60>=2):
              print("pan_error: ", pan_error)
              time_init = pyb.millis()

            img.draw_rectangle(max_blob.rect()) # rect 目标区域画矩形
            img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy画中心点
            img.draw_string(0, 0, "FPS:%.2f"%(clock.fps()))   #画出帧率
            img.draw_string(max_blob.cx(), max_blob.cy(), "%.2f mm"%(ball_s))  #显示目标的距离参数
            lcd.display(img) # Take a picture and display the image. #画完参数后再刷新一次图像
            pan_output=pan_pid.get_pid(pan_error,1)/2   #计算PID参数
            tilt_output=tilt_pid.get_pid(tilt_error,1)  #计算PID参数
            pan_angle=pan_servo.angle()+pan_output

            tilt_angle=tilt_servo.angle()-tilt_output
           # print("pan_output",pan_output)
            pan_servo.angle(pan_angle)
            tilt_servo.angle(tilt_angle)
    else :
        img.draw_string(0, 0, "FPS:%.2f"%(clock.fps()))  #没有找到对应的色块也显示出帧率
        lcd.display(img) # Take a picture and display the image.
   # print("fps:",clock.fps())
