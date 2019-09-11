//============================================================================
//	Name        : main.c
//	Author      : Team KKY
//	Copyright   : 2012 World Embedded Test
//============================================================================

///////////////		Drive mode		///////////////
//
#define NOTHING					0
#define SUDDEN_STOP		1		//	돌발 정지
#define SHUT_OFF				2		//	차단기 구간
#define TRAFFIC_LIGHT	3		//	신호등 구간
#define CHANGE_LINE		4		//	차선변경
#define PARKING 				5		//	주차
#define SPECIAL					6		//	특수구간
#define SLOPE						7		//	언덕주행
#define SHAKING					8		//	방지턱
#define CURVE						9		//	곡선주행
#define GO_STRAIGHT		10	//	직진주행
#define EXIT_						11
//
//////////////////////////////////////////////////

//	car library
#include "car_lib.h"
#include <pxa_lib.h>
#include <pxa_camera_zl.h>
//	opencv library
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
//	c library
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//	global thread
pthread_t get_Image_t;			// 이미지 읽기 쓰레드
pthread_t process_Image_t;	// 이미지 처리 쓰레드
pthread_mutex_t mutex;

//	global variable
int MODE = NOTHING;	// drive mode , Initializaing as NOTHING Default Mode

void NOTHING_FUNC(); //mode 0 function

//	user library
#include "serial.h"
#include "cvtImage.h"
#include "straight.h"
#include "drawDirectionLines.h"
#include "imageProcessModule.h"
#include "park.h"
#include "change_line.h"

//	initiate car before running
void init_car() {
	setting_hl(DM_ANGLE, 1500);	// 조향 모터 각도 제어
	setting_hl(DM_SPEED, 100);		// 조향 모터 속도 제어
	setting_hl(CM_ANGLE, 1520);	// 카메라 모터 각도 제어
	setting_hl(CM_SPEED, 100);		// 카메라 모터 속도 제어
}

void run_Modules() {
	//	initiate imageProcessModule
	init_imageProcessModule();
	//	create threads
	pthread_create(&get_Image_t, NULL, get_Image, NULL );
	pthread_create(&process_Image_t, NULL, process_Image, NULL );
	pthread_join(get_Image, NULL );
	pthread_join(process_Image, NULL );
}

int main() {
	car_connect();
	open_serial();

	//	initiate car before running
	init_car();
	init_car();
	
	run_Modules();
	
	//	start unmanned drive
	sound_play("start.mp3");

	while (1) {
		if (MODE == SUDDEN_STOP) {
			// SUDDEN_STOP();
			MODE = NOTHING;
		} else if (MODE == SHUT_OFF) {
			//SHUT_OFF();
			MODE = NOTHING;
		} else if (MODE == TRAFFIC_LIGHT) {
			//TRAFFIC_LIGHT();
			MODE = NOTHING;
		} else if (MODE == CHANGE_LINE) {
			//CHANGE_LINE();
			MODE = NOTHING;
		} else if (MODE == PARKING) {
			//PARKING_FUNC();
			MODE = NOTHING;
		} else if (MODE == SPECIAL) {
			//SPECIAL();
			MODE = NOTHING;
		} else if (MODE == SLOPE) {
			//SLOPE();
			MODE = NOTHING;
		} else if (MODE == SHAKING) {
			//SHAKING();
			MODE = NOTHING;
		} else if (MODE == CURVE) {
			//CURVE();
			MODE = NOTHING;
		} else if (MODE == GO_STRAIGHT) {
			//GO_STRAIGHT();
			MODE = NOTHING;
		} else if (MODE == EXIT_) {
			//EXIT();
			MODE = NOTHING;
			break;	//The End.... Out from While.....
		} else NOTHING_FUNC(); //Default mode
	}

	release_imageProcessModule();

	return 0;
}

void NOTHING_FUNC() {
	//	Please write Conditions for Other 11 Entire modes.....
	//	Write Standard Straight Moving Mode
	//		(example)
	//		if(sensor1 > 100)
	//			MODE = SUDDEN_STOP;
	//		else if(sensor2 > 200)
	//			MODE = SHUT_OFF; ..... etc...

	detect_DotLine();

	//	Parking Mode
	if (check_distance(3) > 60) {
		sleep(1);
		if (check_distance(3) > 60)
			MODE = PARKING;
	}
}
