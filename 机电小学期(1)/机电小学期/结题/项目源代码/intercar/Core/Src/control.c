#include "control.h"
#include "servo.h"
/*
 * control.c
 *
 *  Created on: 2022年7月9日
 *      Author: 86152
 */

void setspeed(int targetspeed,int curvature)
{
	double omega = (float)targetspeed*curvature/10000;//计算角速度
	lefttarget = targetspeed - (int)omega*width/2;
	righttarget = targetspeed + (int)omega*width/2;
	if (lefttarget > maxtarget)
		{lefttarget = maxtarget;}
	if (righttarget > maxtarget)
		{righttarget = maxtarget;}
	if (lefttarget < -maxtarget)
		{lefttarget = -maxtarget;}
	if (righttarget < -maxtarget)
		{righttarget = -maxtarget;}
}
double roundPID(double targetposition,double position,double *error)
{
	double errorpos = targetposition - position;
	static double errorpos_last = 0,errorpos_prev = 0;
	double pos_pid =0;
	*error = errorpos;
	pos_pid = KPR*(errorpos - errorpos_last) + KIR*errorpos
			+ KDR*(errorpos-2*errorpos_last+errorpos_prev);
	errorpos_prev = errorpos_last;	  	    // 保存上上次误差
	errorpos_last = errorpos;	              // 保存上次偏差
	return pos_pid;
}
void spin(short spin_dir)//spin_dir=1:left   spin_dir=2:right
{
	if(spin_dir==1)
	{
		lefttarget = -spinspeed;
		righttarget = spinspeed;
	}
	if(spin_dir==2)
	{
		lefttarget = spinspeed;
		righttarget = -spinspeed;
	}
	if(spin_dir==0)
	{
		lefttarget = 0;
		righttarget =0;
	}
	if (lefttarget > maxtarget)
		{lefttarget = maxtarget;}
	if (righttarget > maxtarget)
		{righttarget = maxtarget;}
	if (lefttarget < -maxtarget)
		{lefttarget = -maxtarget;}
	if (righttarget < -maxtarget)
		{righttarget = -maxtarget;}
}
void groundturn(short spin_dir,short angle)
{
	avoid_stop();
	spin(spin_dir);
	is_groundturn=spin_dir;
	turn_target=(short)angle/6.8;
}
void fetchplace()
{
	if(is_fetch==0){
		is_down=0;
		is_shut=0;
		servo1_down();
		servo2_shut();
		is_down=1;
		is_shut=1;
		servo1_down();
		is_fetch=1;
		is_down=0;
	}
	else if(is_fetch==1){
		is_down=0;
		is_shut=1;
		servo1_down();
		servo2_shut();
		is_down=1;
		is_shut=0;
		servo1_down();
		is_fetch=0;
		is_down=0;
	}
}
void alongside(double deviation)
{
	double curvdifference = roundPID(0,deviation,&curv_error);
	curvature +=(short)curvdifference;
	int target=(int)(targetspeed-0.5*curvature);
	if(curvature>maxcurvature)
		{curvature = maxcurvature;}
	if(curvature<-maxcurvature)
		{curvature = -maxcurvature;}
	setspeed(target,curvature);
}
void avoidance()
{
	is_close1=get_distance1();
	if((is_close1<20)||(is_close1>1000)){is_close1=1000;}
	is_close2=get_distance2();
	if((is_close2<20)||(is_close2>1000)){is_close2=1000;}
	is_close3=get_distance3();
	if((is_close3<20)||(is_close3>1000)){is_close3=1000;}
	is_close4=get_distance4();
	if((is_close4<10)||(is_close4>1000)){is_close4=1000;}
	if (avoid_dir==0){
		if (((is_close2<260)&&(is_close1<220))||(is_close2<50)){
			avoid_dir = 2;
			groundturn(2,90);			//原地右转并前进
		}
		else if (((is_close3<260)&&(is_close1<220))||(is_close3<50)){
			avoid_dir = 1;
			groundturn(1,90);	     //原地左转并前进
		}
		else if ((is_close1<200)&&(is_close1>=60)){  //任意左右转
			avoid_comfirm= 1-avoid_comfirm;
			if(avoid_comfirm==0){
				avoid_dir=1;
				groundturn(1,90);
			}   //左转并前进
			else if(avoid_comfirm==1){
				avoid_dir=2;
				groundturn(2,90);
			}  //右转并前进
		}
		else if((is_close1<60)||(is_close2<40)||(is_close3<40)){
			if(back_flag==0){
			    avoid_stop();
			    setspeed(-targetspeed,0);
			    back_flag+=1;
			}
			else if((back_flag>0)&&(back_flag<10)){
				setspeed(-targetspeed,0);
				back_flag+=1;
			}
			else if(back_flag==10){
				avoid_stop();
				back_flag=0;
			}
		}
		else {
			if(check_flag==20){
			    if ((theta<320)&&(theta>180)){groundturn(1,360-theta);}//左转回正
				else if ((theta>40)&&(theta<=180)){groundturn(2,theta);}//右转回正
			    check_flag=0;
			}
			check_flag+=1;
			setspeed(targetspeed,0);
		}   //直行
	}
	else if (avoid_dir==1){
		if ((is_close3>230)&&(is_close4>400)&&(is_close4<=1000)&&(servo_angle==180)){
			avoid_dir= 0;
			groundturn(2,90);           //右转，即恢复向前走
		}
		else if(((is_close1<230)&&(is_close3<230))||(is_close1<100)){
			avoid_dir=3;
			avoid_stop();
			setspeed(-targetspeed,0);//后退，退退退！
			total_distance-=1;
		}
		else{
			setspeed(targetspeed,0);    //继续直走，即向左走
			total_distance+=1;
		}
	}
	else if (avoid_dir==2){
		if((is_close2>230)&&(is_close4>400)&&(is_close4<=1000)&&(0==servo_angle)){
			avoid_dir= 0;
			groundturn(1,90);	 		//左转，即恢复向前走
		}
		else if(((is_close1<230)&&(is_close2<230))||(is_close1<100)){
			avoid_dir=4;
			avoid_stop();
			setspeed(-targetspeed,0);		//后退，退退退！
			total_distance+=1;
		}
		else{
			setspeed(targetspeed,0);     //继续直走，即向右走
			total_distance-=1;
		}
	}
	else if (avoid_dir==3){
		if ((is_close3>230)&&(is_close4>300)&&(servo_angle==180)){
			avoid_dir= 0;
			groundturn(2,90);		//右转，即恢复向前走
		}
		else if (((is_close3<230)&&(is_close4<120)&&(servo_angle==90))||((is_close4<80)&&(servo_angle==90))){
			avoid_dir = 5;
			groundturn(1,90);      //左转，即向起点方向走
		}
	    else {
	    	setspeed(-targetspeed,0);   //继续退！
	    	total_distance-=1;
	    }
	}
	else if (avoid_dir==4){
		if ((is_close2>230)&&(is_close4>300)&&(0==servo_angle)){
			avoid_dir= 0;
			groundturn(1,90);		//左转，即恢复向前走
		}
		else if (((is_close2<230)&&(is_close4<120)&&(servo_angle==90))||((is_close4<80)&&(servo_angle==90))) {
			avoid_dir = 6;
			groundturn(2,90);      //右转，即向起点方向走
		}
		else {
			setspeed(-targetspeed,0);   //继续退！
			total_distance+=1;
		}
	}
	else if (avoid_dir == 5) {
		if ((is_close2 > 230) && (is_close4 > 400) && (servo_angle ==0) ) {
			avoid_dir = 2;
			groundturn(1,90);     //左转，即向右走
		}
		else if((is_close3 > 230) && (is_close4 > 400) && (servo_angle ==180)){
			avoid_dir = 1;
			groundturn(1,90);     //右转，即向左走
		}
		else {setspeed(targetspeed,0);}   //不改变方向
	}
	else if (avoid_dir == 6) {
		if ((is_close3 > 230) && (is_close4 > 700) && (servo_angle == 180) ) {
			avoid_dir = 1;
			groundturn(2,90);    //右转，即向左走
		}
		else if((is_close2 > 230) && (is_close4 > 700) && (servo_angle ==0)){
			avoid_dir = 2;
			groundturn(1,90);     //左转，即向右走
		}
		else {setspeed(targetspeed,0);}     //不改变方向
	}
}
void avoid_stop()
{
	is_stopping=1;
	setspeed(0,0);
}
