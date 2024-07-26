# Untitled - By: 25138 - Thu Jul 25 2024
#可抓取坐标是（52，43）
import sensor, image, time
from pyb import UART,Timer,LED
uart = UART(3, 19200)
LED(3).on
flag=0
def tick1(timer):
    global flag
    #transmit="x"+str(sumDeriSpeed//cnt)+"o"
    flag=1
THRESHOLD=(64, 76, 11, 47, 33, 66)
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA)
sensor.skip_frames(time = 2000)
def find_max(blobs):
                max_size = 0
                for blob in blobs:
                    if blob.pixels() > max_size:
                        max_blob = blob
                        max_size = blob.pixels()
                return max_blob #寻找最大色块并返回最大色块的坐标
clock = time.clock()
xThre=[0,0]
yThre=[0,0]
transmit="shit"
tim1=Timer(4,freq=1)
tim1.callback(tick1)
while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(strength=1.8, zoom = 1.0)
    blobs=img.find_blobs([THRESHOLD])
    if blobs:
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob.rect())#框选最大色块
        img.draw_cross(max_blob.cx(), max_blob.cy())#在最大色块中心画十字
        #print('x: %f' %max_blob.cx())
        #print('y: %f' %max_blob.cy())
        if max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()>yThre[0] and max_blob.cy()<yThre[1]:
            transmit="win"
        elif  max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()<yThre[0]:
            transmit="w"
        elif max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()<yThre[1]:
            transmit="s"
        if max_blob.cx()<xThre[0] and max_blob.cy()>yThre[0] and max_blob.cy()<yThre[1]:
            transmit="d"
        elif max_blob.cx()>xThre[1] and max_blob.cy()>yThre[0] and max_blob.cy()<yThre[1]:
            transmit="a"

        LED(3).toggle()
        if flag==1:
            uart.write(transmit)
            print(transmit)
            flag=0






    #print(clock.fps())
