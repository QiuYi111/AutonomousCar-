THRESHOLD = (41, 100, -128, 127, -128, 127)
greyThre=(122, 255)# Grayscale threshold for dark things...
import sensor, image, time
from pyb import UART,Timer
uart = UART(3, 19200)
flag=0
'''
from pid import PID

 #from pyb import uart
positionPid = PID(p=1,i=1,d=1,t=30)
angelPid = PID(p=1,i=1,d=1,t=0)
'''
def tick(timer):
    global flag
    flag=1

tim=Timer(2,freq=10)

tim.callback(tick)
positionError=positionErrorLast=positionErrorBefore=0
positionKp=1
positionKi=1
positionKd=0
angelError=angelErrorLast=angelErrorBefore=0
angelKp=1
angelKi=1
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
deriSpeed=0
sensor.reset()
sensor.set_vflip(False)
sensor.set_hmirror(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds
clock = time.clock()                # to process a frame sometimes.
while(True):
    clock.tick()

    img = sensor.snapshot()#.binary([greyThre])

    #img.invert()
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
    line1 = img.get_regression([(100, 100)], roi=(0, 0, img.width() // 2, img.height()), robust=True)
    line2 = img.get_regression([(100, 100)], roi=(img.width() // 2, 0, img.width() // 2, img.height()), robust=True)

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
           transmit="x"+str(deriSpeed)+"o"
           if flag==1:
               uart.write(transmit)
               flag=0
               print(transmit)



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
