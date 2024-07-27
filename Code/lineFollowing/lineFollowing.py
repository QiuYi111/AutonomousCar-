binTHRESHOLD = (41, 100, -128, 127, -128, 127)
orangeTHRESHOLD=(26, 74, -6, 55, 17, 54)
greyThre=(122, 255)# Grayscale threshold for dark things...
import sensor, image, time
from pyb import UART,Timer,LED
#可抓取坐标是（52，43）
uart = UART(3, 19200)
cnt=0
xFound=0
sumDeriSpeed=0
flag=0
flag_x=0
mode=0
xThre=[45,55]
yThre=[35,45]
transmit="S"

'''
from pid import PID

 #from pyb import uart
positionPid = PID(p=1,i=1,d=1,t=30)
angelPid = PID(p=1,i=1,d=1,t=0)
'''
def tick1(timer):
    global cnt,flag,sumDeriSpeed,transmit
    #transmit="x"+str(sumDeriSpeed//cnt)+"o"
    flag=1
    cnt=1
    sumDeriSpeed=0
def tick2(timer):
    global cnt,sumDeriSpeed,deriSpeed,flag_x
    flag_x=1
    sumDeriSpeed+=deriSpeed
    cnt+=1
tim1=Timer(4,freq=100)
tim1.callback(tick1)
tim2=Timer(2,freq=1000)
tim2.callback(tick2)
positionError=positionErrorLast=positionErrorBefore=0
positionKp=100
positionKi=0
positionKd=0
angelError=angelErrorLast=angelErrorBefore=0
angelKp=100
angelKi=0
angelKd=0
def positionPid(present):
    global positionError,positionErrorLast,positionErrorBefore,positionKp,positionKi,positionKd
    positionError=40-present
    output=positionKp*(positionError-positionErrorLast)+positionKi*(positionError)+positionKd*(positionError-2*positionErrorLast+positionErrorBefore)
    positionErrorBefore=positionErrorLast
    positionErrorLast=positionError
    return output

def angelPid(present):
    global angelError,angelErrorLast,angelErrorBefore,angelKp,angelKi,angelKd
    angelError=0-present
    output=angelKp*(angelError-angelErrorLast)+angelKi*(angelError)+angelKd*(angelError-2*angelErrorLast+angelErrorBefore)
    angelErrorBefore=angelErrorLast
    angelErrorLast=angelError
    return output

def find_max(blobs):
                max_size = 0
                for blob in blobs:
                    if blob.pixels() > max_size:
                        max_blob = blob
                        max_size = blob.pixels()
                return max_blob #寻找最大色块并返回最大色块的坐标
deriSpeed=0
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds
clock = time.clock()# to process a frame sometimes.
mode=0
LED(2).on()
LED(3).on()
while(True):
    if mode==0:
        clock.tick()
        img = sensor.snapshot().lens_corr(strength=1.8, zoom = 1.0)
        blobs=img.find_blobs([orangeTHRESHOLD])
        transmit="S"
        #isFind=0
        if blobs:
            #isFind=1
            max_blob = find_max(blobs)
            img.draw_rectangle(max_blob.rect())#框选最大色块
            img.draw_cross(max_blob.cx(), max_blob.cy())#在最大色块中心画十字
            if max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1]:
                transmit="x"
            if xFound==1:
                if max_blob.cy()<yThre[0]:
                    transmit="w"
                elif max_blob.cy()>yThre[1]:
                    transmit="s"
                elif max_blob.cy()<yThre[1] and max_blob.cy()>yThre[0]:
                    transmit="y"

        if flag_x==1:
            uart.write(transmit)
            if transmit=="x":
                xFound=1
            flag_x=0
        if transmit=="y":
            mode=1
            LED(3).off()

            '''
            if isLookingY==1 and max_blob.cy()<yThre[0]:
                transmit="w"
            elif isLookingY==1 and max_blob.cy()>yThre[1]:
                transmit="s"
            elif isLookingY==1 and max_blob.cy()<yThre[1] and max_blob.cy()>yThre[0]:
                transmit="x"
            '''
            '''
            if isFind<10:
                transmit="x"
            if isFind==10:
                if max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()>yThre[0] and max_blob.cy()<yThre[1]:
                    transmit="cool"
                    mode=1
                elif  max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()<yThre[0]:
                    transmit="w"
                elif max_blob.cx()>xThre[0] and max_blob.cx()<xThre[1] and max_blob.cy()>yThre[1]:
                    transmit="s"
                elif max_blob.cx()<xThre[0] :
                    transmit="a"
                elif max_blob.cx()>xThre[1]:
                    transmit="d"
'''



            #LED(3).toggle()
    elif mode==1:
        clock.tick()
        img = sensor.snapshot().binary([binTHRESHOLD ]).lens_corr(strength = 1.8, zoom = 1.0)
        img.invert()
        '''
        lines=img.find_lines()

        if len(lines)>=2:
            line1,line2=find_two_mainLines(lines)
            if line1.magnitude()>8 and line2.magnitude()>8:
                img.draw_line(line1.line(), color = 10)
                img.draw_line(line2.line(), color = 127)
                x1, y1, x2, y2 = line1.line()
                x3, y3, x4, y4 = line2.line()

                mid_x1 = (x1 + x3) // 2
                mid_y1 = (y1 + y3) // 2
                mid_x2 = (x2 + x4) // 2
                mid_y2 = (y2 + y4) // 2
                img.draw_line((mid_x1, mid_y1, mid_x2, mid_y2), color=127)


        line1 = img.get_regression([(100,100)], robust = True)
        line2=img.get_regression([(100,100)], robust = True)
    '''
        line1 = img.get_regression([(100, 100)], roi=(0, img.height()//2, img.width() // 2, img.height()), robust=True)
        line2 = img.get_regression([(100, 100)], roi=(img.width() // 2, img.height()//2, img.width() // 2, img.height()), robust=True)

        if line1 and line2 and line1.magnitude() > 8 and line2.magnitude() > 8:
               # 计算两条线的中点
               x1, y1, x2, y2 = line1.line()
               x3, y3, x4, y4 = line2.line()

               mid_x1 = (x1 + x3) // 2
               mid_y1 = (y1 + y3) // 2
               mid_x2 = (x2 + x4) // 2
               mid_y2 = (y2 + y4) // 2
               img.draw_line(line1.line(), color = 10)
               img.draw_line(line2.line(), color = 10)
               img.draw_line((mid_x1, mid_y1, mid_x2, mid_y2), color=127)
               position=(mid_x1+mid_x2)//2
               angelMeas=mid_x1-mid_x2
               deriSpeed=positionPid(position)+angelPid(angelMeas)

               #print(position,angelMeas)
               #print(positionError,angelError)
              # print(deriSpeed)

               if flag==1:
                   transmit="x"+str(sumDeriSpeed//cnt)+"o"
                   uart.write(transmit)
                   flag=0
                   LED(2).toggle()
                   #print(sumDeriSpeed,deriSpeed,cnt)
                   #print(transmit)
        elif line1 and line1.magnitude()>8 and not line2:
            uart.write("x40o")
        elif line2 and line2.magnitude()>8 and not line1:
            uart.write("x-40o")







           #print(positionPid.get_pid(position),angelPid.get_pid(angelMeas))
          # deriSpeed=positionPid.get_pid(position)+angelPid.get_pid(angelMeas)

           #img.draw_line((40, mid_y1, 40, mid_y2), color=10)

'''

           print("rho of line1:",line1.rho())
           print("rho of line2:",line2.rho())
           print("rho of mid:", rho)
           print("theta of line1:", line1.theta())
           print("theta of line2:", line2.theta())
           print("theta of mid:", theta)


    if (line):
        rho_err = abs(line.rho())-img.width()/2
        if line.theta()>90:
            theta_err = line.theta()-180
        else:
            theta_err = line.theta()
        img.draw_line(line.line(), color = 127)
        #print(rho_err,line.magnitude(),rho_err)
        if line.magnitude()>8:
            #if -40<b_err<40 and -30<t_err<30:
            rho_output = rho_pid.get_pid(rho_err,1)
            theta_output = theta_pid.get_pid(theta_err,1)
            output = rho_output+theta_output
        else:
            output=-1

    else:
        output=0
        pass
    #print(clock.fps())
'''
