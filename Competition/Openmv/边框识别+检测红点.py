import sensor, image, time
import pyb

# 设置相机
sensor.reset() # 初始化摄像头模块.
sensor.set_pixformat(sensor.RGB565) # 或者使用 sensor.RGB565 彩色
sensor.set_framesize(sensor.QVGA) # 或者使用 sensor.QVGA (or others)
sensor.set_auto_gain(False)
sensor.skip_frames(20)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time = 333 )
red_threshold = (30, 100, 15, 127, -40, 127)
clock = time.clock()
roi1=(80,28,240,83)#水平、竖直、长、宽


#uart = UART(3,115200)   #定义串口3变量
#uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
while True:
 clock.tick()
 img = sensor.snapshot().lens_corr(strength=1.05, zoom=1.5)
 blobs = img.find_blobs([red_threshold])
 if blobs:
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        if cx is not None and cy is not None:
        # 绘制绿色十字来表示激光笔的位置
          img.draw_cross(cx, cy, color=(0, 255, 128))
          img = img.to_rgb565()
 img.to_grayscale(copy = True)
#sensor.reset()
#sensor.set_pixformat(sensor.GRAYSCALE)
#sensor.set_framesize(sensor.QVGA) # 320x240
#sensor.skip_frames(time = 333 )#跳过100张图片
#sensor.set_auto_gain(False) # must be turned off for color tracking
#sensor.set_auto_whitebal(False) # must be turned off for color tracking
#sensor.set_hmirror(True)
 regulate=8
 clock = time.clock()
 roi1 = [70,50,180,130]       #巡线敏感区
 time_init = pyb.millis()

 # 定义黑框颜色的阈值
 thresholds = ((48, 0))
 thresholds_white = ((0, 48))



 time_now = pyb.millis()
 time_diff = pyb.millis()
    # 拍摄图像
 img = sensor.snapshot()
    # roi1 = (0,0,img.width(),img.height())

    # 找到黑框区域
 blobs = img.find_blobs([thresholds], roi=roi1, pixels_threshold=100, area_threshold=5000,merge=True)
    # 如果找到黑框
 if blobs:
        shang_zuo = (blobs[0].min_corners()[2][0] ,blobs[0].min_corners()[2][1] )#左上角
        shang_you = ( blobs[0].min_corners()[3][0] , blobs[0].min_corners()[3][1] )#右上角
        xia_you = ( blobs[0].min_corners()[0][0] , blobs[0].min_corners()[0][1] )#右下角
        xia_zuo = ( blobs[0].min_corners()[1][0] , blobs[0].min_corners()[1][1] )#左下角

        #if(blobs[][])
        # 绘制黑框的边界
        img.draw_edges(blobs[0].min_corners(), thickness=2,color = (187,29,97))
        #for i in range(len(edges)):
        #   for j in range(len(edges)):
        #    if(blobs[0].corners()[i][1]>blobs[0].corners()[j][1]):
        #        edges_max = corners()[i][1]
        #        num_max = i

        #for i in range(len(edges)):
        #  for j in range(len(edges)):
        #     if(blobs[0].corners()[i][1]<blobs[0].corners()[j][1]):
        #       edges_min = corners()[i][1]
        #       num_min = i
        edges = blobs[0].min_corners()


        sorted_tru = sorted(edges, key=lambda item: item[1], reverse=False)
        x1 = sorted_tru[0][0]
        y1 = sorted_tru[0][1]
        x2 = sorted_tru[1][0]
        y2 = sorted_tru[1][1]
        x3 = sorted_tru[2][0]
        y3 = sorted_tru[2][1]
        x4 = sorted_tru[3][0]
        y4 = sorted_tru[3][1]
        #print(sorted_tru[0])
        #print(sorted_tru)
        #print((x1,y1))
        if(abs(y1-y2)<=2): # 检测垂直
            sorted_tru1 = sorted((sorted_tru[0],sorted_tru[1]), key=lambda item: item[0], reverse=False)
            sorted_tru2 = sorted((sorted_tru[2],sorted_tru[3]), key=lambda item: item[0], reverse=False)
            sorted_final = sorted_tru1+sorted_tru2
            print(sorted_final)
            x1 = sorted_final[0][0]
            y1 = sorted_final[0][1]
            x2 = sorted_final[1][0]
            y2 = sorted_final[1][1]
            x3 = sorted_final[2][0]
            y3 = sorted_final[2][1]
            x4 = sorted_final[3][0]
            y4 = sorted_final[3][1]



            x1_new = (x1+regulate)
            y1_new = (y1+regulate)
            x2_new = (x2-regulate)
            y2_new = (y2+regulate)



            x3_new = (x3+regulate)
            y3_new = (y3-regulate)
            x4_new = (x4-regulate)
            y4_new = (y4-regulate)
            img.draw_line(x1_new,y1_new,x2_new,y2_new)
            img.draw_line(x2_new,y2_new,x4_new,y4_new)
            img.draw_line(x1_new,y1_new,x3_new,y3_new)
            img.draw_line(x3_new,y3_new,x4_new,y4_new)


        #else:
        '''
        #print(blob.w(),blob.h())


        #blobs_white = img.find_blobs([thresholds_white],invert=True, roi=roi_white, pixels_threshold=300, area_threshold=6000,merge=True,merge_cb = function(if_hebing,blobs_white))
       # if blobs_white:

        #        img.draw_edges(blobs_white[0].corners(),thickness=2)
        #        shang_zuo_white = (blobs_white[0].corners()[2][0] ,blobs_white[0].corners()[2][1] )#左上角
        #        shang_you_white = ( blobs_white[0].corners()[3][0] , blobs_white[0].corners()[3][1] )#右上角

        #        xia_you_white = (blobs_white[0].corners()[0][0] , blobs_white[0].corners()[0][1] )#右下角
        #        xia_zuo_white = ( blobs_white[0].corners()[1][0] , blobs_white[0].corners()[1][1] )#左下角
        #print(blobs[0]'''
        #print("上左:", shang_zuo)
        #print("上右:", shang_you)
        #print("下左:",xia_you)
        #print("下右:",xia_zuo)
        #print(blobs[0].min_corners())
        if(time_diff>=1000):
         time_init = pyb.millis()



