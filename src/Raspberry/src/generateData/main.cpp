#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <cstdlib>

#include "pixy.h"
#include <wiringPi.h>
#include <wiringSerial.h>
using namespace std;

#define BLOCK_BUFFER_SIZE    25
// Pixy Block buffer
struct Block blocks[BLOCK_BUFFER_SIZE];
static bool run_flag = true;
void handle_SIGINT(int unused){run_flag = false;}

typedef struct _point{
	int x;
	int y;
} point;

float distToCenter(point p);
void swapPoints(point *p1, point *p2);

int main(int argc, char * argv[])
{
	float delay_val;
	if (argc == 1)
	{
		delay_val = 100;
		printf("Using default delay value of %f milisecond(s) !\n", delay_val);
	}else
	{
		delay_val = atof(argv[1]);
		printf("Using default delay of %f milisecond(s) !\n", delay_val);
	}

	int blocks_copied, pixy_init_status;
	point robot,light;
	//---------- Initiate Serial ----------//
	int fd;
	if((fd = serialOpen ("/dev/ttyS0", 115200)) < 0 )
	/*device must be replaced by the serial port in the RPi*/
		perror("device not opened \n");
	if ( wiringPiSetup () < 0 )
		perror("WiringPiSetup problem \n ");

	//---------- Initiate Pixy ----------//
	signal(SIGINT, handle_SIGINT);// To catch CTRL+C
	printf("libpixyusb Version: %s\n", __LIBPIXY_VERSION__);
	pixy_init_status = pixy_init();// Connect to Pixy

	// Error initializing Pixy
	if(!pixy_init_status == 0)
	{
	printf("pixy_init(): ");
	pixy_error(pixy_init_status);
	return pixy_init_status;
	}

	printf("Detecting blocks...\n");

	
	while(run_flag)
	{
		if(getchar()){
		// Wait for new blocks to be available
		while(!pixy_blocks_are_new() && run_flag); 

		// Get blocks from Pixy
		blocks_copied = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);

		if(blocks_copied < 0) {
		  // Error: pixy_get_blocks
		  printf("pixy_get_blocks(): ");
		  pixy_error(blocks_copied);
		}

		// Calculate robot and light absolute (x,y)
		if(blocks_copied==2){
			robot.x = blocks[0].x-160;
			robot.y = blocks[0].y-100;
			light.x = blocks[1].x-160;
			light.y = blocks[1].y-100;
			if(distToCenter(light)<distToCenter(robot)){
				swapPoints(&light, &robot);
			};
			// Print positions
			/*printf("Robot: x:%d y:%d\n", robot.x, robot.y);
			printf("Light: x:%d y:%d\n", light.x, light.y);*/
			printf("\n\n\n");
		}

		int camMaxX = 320;
		int camMaxY = 200;
		int resolution = 7;
		// Print camera image
		/*for(	int y=-camMaxY/2 ; y<camMaxY/2 ; y+=resolution){
			for(int x=-camMaxX/2 ; x<camMaxX/2 ; x+=resolution){
				if(abs(robot.x-x)<=resolution*1.5 && abs(robot.y-y)<=resolution*1.5){
					cout<<"*";
				}else if(abs(light.x-x)<=resolution*1.5 && abs(light.y-y)<=resolution*1.5){
					cout<<"o";
				}else{
					cout<<".";
				}
			}
		cout<<endl;
		}*/
		//serialPutchar(fd, '.');
		//delay(delay_val);
		//printf("Robot: (%d, %d)\n",0,0);
		//serialPrintf(fd, "Robot: (%d, %d)\n",0,0);
		printf("Light: (%d, %d)\n",light.x,light.y);
		serialPrintf(fd, "(%d, %d)\n",light.x,light.y);
		//serialPrintf(fd, "%d %d\n",light.x,light.y);
		}
	}
	pixy_close();
}

float distToCenter(point p){
	return sqrt(p.x*p.x + p.y*p.y);
}

void swapPoints(point *p1, point *p2){
	point temp = {p2->x,p2->y};
	p2->x = p1->x;
	p2->y = p1->y;
	p1->x = temp.x;
	p1->y = temp.y;
}
