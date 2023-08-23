import sensor, image, time
import pyb
import math
from pyb import UART

# 设置相机

sensor.reset()  # 初始化摄像头模块.
sensor.set_pixformat(sensor.RGB565)  # 或者使用 sensor.RGB565 彩色
sensor.set_framesize(sensor.QVGA)  # 或者使用 sensor.QVGA (or others)
sensor.set_auto_gain(False)
sensor.skip_frames(20)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time=2000)
red_threshold = (30, 100, 15, 127, -40, 127)
clock = time.clock()
uart = UART(3, 115200)  # 串口通信
process = 0
cx = 0
cy = 0
errx = 500
erry = 500
count = 0

def cj(cj):
    if cj >= 0:
        return 0
    else:
        return 1


def point_calculating(x1, x2, y1, y2, xo, yo):
    point_x = 0.5 * (x1 + x2) + xo
    point_y = 0.5 * (y1 + y2) + yo
    return (point_x, point_y)


def calculating_error(cx, cy, x_target, y_target):
    errx = cx - x_target
    erry = cy - y_target
    return errx, erry


def if_finished(errx, erry):
    if (abs(errx) <= 2 and abs(erry) <= 2):
        return True
    else:
        return False
def if_black(x,y):
    (x1,x2,x3) = img.get_pixel(int(x),int(y))
    if (x1<=250 and x2<=250 and x3<=250):
        return True
    else:
        return False


# uart = UART(3,115200)   #定义串口3变量
# uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
# time_init = pyb.millis()
# time_init2 = pyb.millis()
# img = sensor.snapshot()


roi_red = [48, 0, 245, 240]
roi_edge = [81, 28, 175, 165]
while True:
    #    time_now = pyb.millis()
    #    time_diff = time_now - time_init

    img = sensor.snapshot().lens_corr(strength=1.05, zoom=1.6)
    blobs = img.find_blobs([red_threshold], roi=roi_red)
    if blobs:
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        if cx is not None and cy is not None:
            # 绘制绿色十字来表示激光笔的位置
            img.draw_cross(cx, cy, color=(0, 255, 128))
            # img = img.to_rgb565()
    img.to_grayscale(copy=True)
    img.erode(2, threshold = 5) #腐蚀膨胀
    # sensor.reset()
    # sensor.set_pixformat(sensor.GRAYSCALE)
    # sensor.set_framesize(sensor.QVGA) # 320x240
    # sensor.skip_frames(time = 333 )#跳过100张图片
    # sensor.set_auto_gain(False) # must be turned off for color tracking
    # sensor.set_auto_whitebal(False) # must be turned off for color tracking
    # sensor.set_hmirror(True)
    regulate = 5
    h_prep_w = 1.418
    perp_h = 0.02
    perp_w = perp_h * h_prep_w

    clock = time.clock()
      # 巡线敏感区Q
    # time_init = pyb.millis()

    # 定义黑框颜色的阈值
    thresholds = ((48, 0))
    thresholds_white = ((0, 48))
    # print(cx)

    # time_diff2 = time_init2 - time_now
    # 拍摄图像
    #img = sensor.snapshot().lens_corr(strength=1.05, zoom=1.5)
    # roi1 = (0,0,img.width(),img.height())
    if cx is not None and cy is not None:
        # 找到黑框区域
        blobs = img.find_blobs([thresholds], roi=roi_edge, pixels_threshold=100, area_threshold=5000, merge=True)
        # 如果找到黑框
        if blobs:
            for blob in blobs:
                if (blob.area() <= 20000):

                    img.draw_edges(blobs[0].min_corners(), thickness=2)
                    # for i in range(len(edges)):
                    #   for j in range(len(edges)):
                    #    if(blobs[0].corners()[i][1]>blobs[0].corners()[j][1]):
                    #        edges_max = corners()[i][1]
                    #        num_max = i

                    # for i in range(len(edges)):
                    #  for j in range(len(edges)):
                    #     if(blobs[0].corners()[i][1]<blobs[0].corners()[j][1]):
                    #       edges_min = corners()[i][1]
                    #       num_min = i
                    edges = blobs[0].min_corners()

                    sorted_tru = sorted(edges, key=lambda item: item[1], reverse=False)
                    if(count<=100):
                     x1 = sorted_tru[0][0]
                     y1 = sorted_tru[0][1]

                     x2 = sorted_tru[1][0]
                     y2 = sorted_tru[1][1]

                     x3 = sorted_tru[2][0]
                     y3 = sorted_tru[2][1]

                     x4 = sorted_tru[3][0]
                     y4 = sorted_tru[3][1]

                    if(count>=100):
                      if(!if_black(sorted_tru[0][0],sorted_tru[0][1]) and x1<x2):

                           y1 = y1+1
                           x1 = x1+1

                      if(if_black(sorted_tru[1][0],sorted_tru[1][1])):
                        x2 = sorted_tru[1][0]
                        y2 = sorted_tru[1][1]
                        print(img.get_pixel(int(x1),int(y1)))
                      if(if_black(sorted_tru[2][0],sorted_tru[2][1])):
                       x3 = sorted_tru[2][0]
                       y3 = sorted_tru[2][1]
                      if(if_black(sorted_tru[3][0],sorted_tru[3][1])):
                       x4 = sorted_tru[3][0]
                       y4 = sorted_tru[3][1]

                    # print(sorted_tru[0])
                    # print(sorted_tru)
                    # print((x1,y1))
                    if (abs(y1 - y2) == 0):  # 检测垂直
                        version = 0
                        sorted_tru1 = sorted((sorted_tru[0], sorted_tru[1]), key=lambda item: item[0], reverse=False)
                        sorted_tru2 = sorted((sorted_tru[2], sorted_tru[3]), key=lambda item: item[0], reverse=False)
                        sorted_final = sorted_tru1 + sorted_tru2
                        # print(sorted_final)
                        # print(blob[2],blob[3])
                        x1 = sorted_final[0][0]
                        y1 = sorted_final[0][1]
                        x2 = sorted_final[1][0]
                        y2 = sorted_final[1][1]
                        x3 = sorted_final[2][0]
                        y3 = sorted_final[2][1]
                        x4 = sorted_final[3][0]
                        y4 = sorted_final[3][1]




                        if (x4 - x3 >= y3 - y1):
                            x1_new = (x1 + (x2 - x1) * perp_h)
                            y1_new = (y1 + (y3 - y1) * perp_w)
                            x2_new = (x2 - (x2 - x1) * perp_h)
                            y2_new = (y2 + (y4 - y2) * perp_w)
                            x3_new = (x3 + (x4 - x3) * perp_h)
                            y3_new = (y3 - (y3 - y1) * perp_w)
                            x4_new = (x4 - (x4 - x3) * perp_h)
                            y4_new = (y4 - (y4 - y2) * perp_w)
                        elif (x4 - x3 < y3 - y1):
                            x1_new = (x1 + (x2 - x1) * perp_w)
                            y1_new = (y1 + (y3 - y1) * perp_h)
                            x2_new = (x2 - (x2 - x1) * perp_w)
                            y2_new = (y2 + (y4 - y2) * perp_h)
                            x3_new = (x3 + (x4 - x3) * perp_w)
                            y3_new = (y3 - (y3 - y1) * perp_h)
                            x4_new = (x4 - (x4 - x3) * perp_w)
                            y4_new = (y4 - (y4 - y2) * perp_h)
                        img.draw_cross((int(x1_new), int(y1_new)), color=(255, 0, 0))
                        img.draw_cross((int(x2_new), int(y2_new)), color=(255, 0, 0))
                        img.draw_cross((int(x3_new), int(y3_new)), color=(255, 0, 0))
                        img.draw_cross((int(x4_new), int(y4_new)), color=(255, 0, 0))
                        # print(1)
                    elif (abs(y1 - y2)>0):
                        version = 1
                        # print(sorted_tru[1])
                        sorted1 = sorted((sorted_tru[1], sorted_tru[2]), key=lambda item: item[0], reverse=True)
                        # sorted_final =(sorted_tru[0][0],sorted_tru[0][1])+sorted1+(sorted_tru[3][0],sorted_tru[3][1])

                        # print(sorted_final)
                        # print(blob[2],blob[3])
                        # print(sorted1)
                        x2 = sorted1[0][0]
                        y2 = sorted1[0][1]
                        x3 = sorted1[1][0]
                        y3 = sorted1[1][1]






                        # print(x1,y1,x2,y2)
                        if (y2 - y4 > abs(x3 - x4)):
                            (x1_new, y1_new) = point_calculating((x2 - x1) * perp_w, (x3 - x1) * perp_h,
                                                                 (y2 - y1) * perp_w, (y3 - y1) * perp_h, x1, y1)
                            (x2_new, y2_new) = point_calculating((x1 - x2) * perp_w, (x4 - x2) * perp_h,
                                                                 (y1 - y2) * perp_w, (y4 - y2) * perp_h, x2, y2)
                            (x3_new, y3_new) = point_calculating((x4 - x3) * perp_w, (x1 - x3) * perp_h,
                                                                 (y4 - y3) * perp_w, (y1 - y3) * perp_h, x3, y3)
                            (x4_new, y4_new) = point_calculating((x3 - x4) * perp_w, (x2 - x4) * perp_h,
                                                                 -(y4 - y3) * perp_w, (y2 - y4) * perp_h, x4, y4)
                        elif (y2 - y4 < abs(x3 - x4)):
                            (x1_new, y1_new) = point_calculating((x2 - x1) * perp_h, (x3 - x1) * perp_w,
                                                                 (y2 - y1) * perp_h, (y3 - y1) * perp_w, x1, y1)
                            (x2_new, y2_new) = point_calculating(-(x2 - x1) * perp_h, (x4 - x2) * perp_w,
                                                                 (y1 - y2) * perp_h, (y4 - y2) * perp_w, x2, y2)
                            (x3_new, y3_new) = point_calculating((x4 - x3) * perp_h, (x1 - x3) * perp_w,
                                                                 (y4 - y3) * perp_h, (y1 - y3) * perp_w, x3, y3)
                            (x4_new, y4_new) = point_calculating(-(x4 - x3) * perp_h, (x2 - x4) * perp_w,
                                                                 -(y4 - y3) * perp_h, (y2 - y4) * perp_w, x4, y4)
                        img.draw_cross((int(x1_new), int(y1_new)), color=(255, 0, 0))
                        img.draw_cross((int(x2_new), int(y2_new)), color=(255, 0, 0))
                        img.draw_cross((int(x3_new), int(y3_new)), color=(255, 0, 0))
                        img.draw_cross((int(x4_new), int(y4_new)), color=(255, 0, 0))
                        # print((x2_new,y2_new))

                    if (version == 0):
                        if (process == 0):
                            errx, erry = calculating_error(cx, cy, x1_new, y1_new)
                            img.draw_cross((int(x1_new), int(y1_new)))
                            #print(x1_new, y1_new)
                            if (if_finished(errx, erry)):
                                process = 1
                        if (process == 1):
                            errx, erry = calculating_error(cx, cy, x2_new, y2_new)
                            if (if_finished(errx, erry)):
                                process = 2
                        if (process == 2):
                            errx, erry = calculating_error(cx, cy, x4_new, y4_new)
                            if (if_finished(errx, erry)):
                                process = 3

                        if (process == 3):
                            errx, erry = calculating_error(cx, cy, x3_new, y3_new)
                            if (if_finished(errx, erry)):
                                finished = 1
                                process = 0
                    if (version == 1):
                        if (process == 0):
                            errx, erry = calculating_error(cx, cy, x1_new, y1_new)
                            if (if_finished(errx, erry)):
                                process = 1
                        if (process == 1):
                            errx, erry = calculating_error(cx, cy, x2_new, y2_new)
                            if (if_finished(errx, erry)):
                                process = 2
                        if (process == 2):
                            errx, erry = calculating_error(cx, cy, x4_new, y4_new)
                            if (if_finished(errx, erry)):
                                process = 3

                        if (process == 3):
                            errx, erry = calculating_error(cx, cy, x3_new, y3_new)
                            if (if_finished(errx, erry)):
                                finished = 1
                                process = 0

    # print(clock.fps())
    # if(time_diff>=800):
    # time_init = pyb.millis()
    exc = cj(errx)
    eyc = cj(erry)
   # print(cx, cy, exc, errx, eyc, erry)
    #print(x1_new,y1_new)
   # print(process)
    #print(version)
    FH = bytearray([0x2C, 0x12, cx, cy, exc, int(abs(errx)), eyc, int(abs(erry)), 0x5B])
    #print(img.get_pixel(int(173),int(126)))
    uart.write(FH)
    count = count+1
    if(count>=10000):
        count = 100


    # if (time_diff2 >= 1000):
    #     time_init2 = pyb.millis()
    # finished = 0
