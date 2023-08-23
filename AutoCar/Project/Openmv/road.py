import sensor, image, time, ustruct
from pyb import UART
uart = UART(3, 115200)#串口通信
#**********颜色/ROI阈值**********#
THRESHOLD = (0, 62, 26, 127, -128, 127) # Grayscale threshold for dark things...
roi1=(32,100,10,10)#水平、竖直、长、宽
roi2=(90,100,10,10)
roi3=(132,100,10,10)
roi4=(160,100,10,10)
roi5=(192,100,10,10)
roi6=(224,100,10,10)
roi7=(288,100,10,10)
#************变量************#
a_1=a_2=a_3=a_4=a_5=0
#**********摄像头参数**********#
sensor.reset()
sensor.set_vflip(True)   #垂直翻转、水平翻转
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # 320x240
#sensor.set_windowing([0,20,80,40])
sensor.set_auto_whitebal(True)     #自动白平衡
sensor.skip_frames(time = 2000)     # 设置生效

#**********检测前方横线子函数**********#
def tongji():
  global a_1
  global a_2
  global a_3
  global a_4
  global a_5
  global a_6
  global a_7
  #**********1**********#
  statistics1=img.get_statistics(roi=(32,100,5,5))  # 区域：前两个是起点，最后一个是长宽
  color_l1=statistics1.l_mode()
  print("color_l1:",color_l1)
  if color_l1==100:
    a_1=1
  else:
    a_1=0
  #**********2**********#
  statistics2=img.get_statistics(roi=(96,100,5,5))
  color_l2=statistics2.l_mode()
  print("color_l2:",color_l2)
  if color_l2==100:
    a_2=1
  else:
    a_2=0
  #**********3**********#
  statistics3=img.get_statistics(roi=(132,100,5,5))
  color_l3=statistics3.l_mode()
  print("color_l3:",color_l3)
  if color_l3==100:
    a_3=1
  else:
    a_3=0
  #**********4**********#
  statistics4=img.get_statistics(roi=(157,100,5,5))
  color_l4=statistics4.l_mode()
  print("color_l4:",color_l4)
  if color_l4==100:
    a_4=1
  else:
    a_4=0
  #**********5**********#
  statistics5=img.get_statistics(roi=(192,100,5,5))
  color_l5=statistics5.l_mode()
  print("color_l5:",color_l5)
  if color_l5==100:
    a_5=1
  else:
    a_5=0
  #**********6**********#
  statistics6=img.get_statistics(roi=(224,100,5,5))
  color_l6=statistics6.l_mode()
  print("color_l6:",color_l6)
  if color_l6==100:
    a_6=1
  else:
    a_6=0
  #**********7**********#
  statistics7=img.get_statistics(roi=(288,100,5,5))
  color_l7=statistics7.l_mode()
  print("color_l7:",color_l7)
  if color_l7==100:
    a_7=1
  else:
    a_7=0
  #*****发送的是7个点的数值*****#类似于7个传感器
  #time.sleep_ms(1000)现在是无延时不间断发送，是否要加延时等调试阶段定

  print(str(a_1)+'\x20'+str(a_2)+'\x20'+str(a_3)+'\x20'+str(a_4)+'\x20'+str(a_5)+'\x20'+str(a_6)+'\x20'+str(a_7))
  FH = bytearray([0x2C,0x12,a_1,a_2,a_3,a_4,a_5,a_6,a_7,0x5B])
  uart.write(FH)
while(True):
    img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0).binary([THRESHOLD])
    img.draw_rectangle(roi1)
    img.draw_rectangle(roi2)
    img.draw_rectangle(roi3)
    img.draw_rectangle(roi4)
    img.draw_rectangle(roi5)
    img.draw_rectangle(roi6)
    img.draw_rectangle(roi7)
    tongji()
