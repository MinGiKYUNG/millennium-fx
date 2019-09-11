/*
 *(C) Copyright 2006 Marvell International Ltd.  
 * All Rights Reserved 
 *
 * Author:     Neo Li
 * Created:    March 07, 2007
 * Copyright:  Marvell Semiconductor Inc. 
 *
 * Sample code interface header for PXA Linux release. All interfaces
 * introduced in this file is not multiple thread safe unless it's 
 * specified explicitly. 
 *
 */



#include <stdio.h>
#include <signal.h>

#include <pxa_lib.h>
#include <pxa_camera_zl.h>

// VIDEOBUF_COUNT must be larger than STILLBUF_COUNT
#define VIDEOBUF_COUNT  3
#define STILLBUF_COUNT  2

#define CAM_STATUS_INIT     0
#define CAM_STATUS_READY    1
#define CAM_STATUS_BUSY     2

struct pxa_camera{
    int handle;
    int status;
    int mode;
    int sensor;
    int ref_count;

    // Video Buffer
    int width;
    int height;
    enum    pxavid_format format;
};

int fd;
struct sigaction act;
void sighandler(int signo)
{
	close(fd);
	exit(0);
}

int main(int argc, char* argv[])
{
	int handle;
	int handle_overlay2;
	struct pxa_video_buf* vidbuf;
	struct pxacam_setting camset;
	struct pxa_video_buf vidbuf_overlay;
	int len;
/*
    fd = open("/dev/input/event0", O_RDWR | O_NONBLOCK);

	if(fd < 0 ) {
		printf("touch open error!!!\n");
		return -1;
	}

    fcntl(fd, F_SETOWN, getpid());
  fcntl(fd, F_SETFL, FASYNC | FNONBLOCK);

//    sigaddset (&act.sa_mask, SIGIO);
    sigaction (SIGIO, &act, 0);
    act.sa_handler = sighandler;
*/
	handle = camera_open(NULL,0);
	ASSERT(handle);

	memset(&camset,0,sizeof(camset));
	camset.mode = CAM_MODE_VIDEO;
	camset.format = pxavid_ycbcr422;
	
	camset.width = 320;
	camset.height = 240;

	camera_config(handle,&camset);

	camera_start(handle);

    handle_overlay2 = overlay2_open(NULL,pxavid_ycbcr422,NULL, 320, 240, 0 , 0);

    overlay2_getbuf(handle_overlay2,&vidbuf_overlay);
    len = vidbuf_overlay.width*vidbuf_overlay.height;

	while(1){
		vidbuf = camera_get_frame(handle);
	
//		display_write(handle_overlay2,&vidbuf_overlay);

    	memcpy(vidbuf_overlay.ycbcr.y,vidbuf->ycbcr.y,len);
	    memcpy(vidbuf_overlay.ycbcr.cb,vidbuf->ycbcr.cb,len/2);
	    memcpy(vidbuf_overlay.ycbcr.cr,vidbuf->ycbcr.cr,len/2);
		
		camera_release_frame(handle,vidbuf);
	}

	camera_stop(handle);
	camera_close(handle);

	return 0;
}
