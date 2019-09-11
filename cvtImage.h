/*
 ============================================================================
 Name        : cvtImage.h
 Author      : Jincheol Kim
 Version     : 5.0.0(updated 2012.10.31, optimized)
 Copyright   : 2012 Embedded Contest
 ============================================================================
 */
//	size of pxa_video_buf
#define VIDBUF_WIDTH	320
#define VIDBUF_HEIGHT	240
//	cut line
#define TOPLINE			88
#define BOTTOMLINE	32
//	YCrCb	data(output)
unsigned char buf_y[76800];
unsigned char buf_cr[38400];
unsigned char buf_cb[38400];

//	input data																	output data
//	Pxa_Video_Buf(YCrCb, 320 * 240)	->	IplImage(3 channels, YCrCb, 160 * 120)
//																						->	IplImage(1 channel, Y, 160 * 120)
void cvVidBuf2IplImage(struct pxa_video_buf* vidbuf, IplImage* frame, IplImage* frame_gray) {
	int i, j, tmp_f, tmp_y;
	for (j = 0; j < VIDBUF_HEIGHT; j = j + 2) {
		for (i = 0; i < VIDBUF_WIDTH; i = i + 2) {
			tmp_f = (3 * i + 480 * j) >> 1;
			tmp_y = i + 320 * j;
			//	imageData[channel * x + channel * width * y + n]
			frame->imageData[tmp_f] = vidbuf->ycbcr.y[tmp_y];
			frame->imageData[tmp_f + 1] = vidbuf->ycbcr.cr[tmp_y >> 1];
			frame->imageData[tmp_f + 2] = vidbuf->ycbcr.cb[tmp_y >> 1];
			//	gray scale
			frame_gray->imageData[(i + 160 * j) >> 1] = vidbuf->ycbcr.y[tmp_y];
		}
	}
}

//	input data																						output data
//	IplImage(3 channels, YCrCb, 160 * 120)	->	Pxa_Video_Buf(YCrCb, 320 * 240)
void cvIplImage2VidBuf(IplImage* frame, struct pxa_video_buf* vidbuf_overlay) {
	int i, j, tmp_f, tmp_y, tmp_z;
	for (j = 0; j < VIDBUF_HEIGHT; j = j + 2) {
		for (i = 0; i < VIDBUF_WIDTH; i = i + 2) {
			tmp_f = (3 * i + 480 * j) >> 1;
			tmp_y = i + 320 * j;
			tmp_z = i + 1 + 320 * (j + 1);
			buf_y[tmp_y] = frame->imageData[tmp_f];
			buf_cr[tmp_y >> 1] = frame->imageData[tmp_f + 1];
			buf_cb[tmp_y >> 1] = frame->imageData[tmp_f + 2];
			buf_y[tmp_z] = frame->imageData[tmp_f];
			buf_cr[tmp_z >> 1] = frame->imageData[tmp_f + 1];
			buf_cb[tmp_z >> 1] = frame->imageData[tmp_f + 2];
		}
	}
	memcpy(vidbuf_overlay->ycbcr.y, buf_y, 76800);
	memcpy(vidbuf_overlay->ycbcr.cr, buf_cr, 38400);
	memcpy(vidbuf_overlay->ycbcr.cb, buf_cb, 38400);
}

//	input data																			output data
//	IplImage(1 channel, Y, 160 * 120)	->	Pxa_Video_Buf(YCrCb, 320 * 240)
void cvIplImage_Gray2VidBuf(IplImage* frame_gray, struct pxa_video_buf* vidbuf_overlay) {
	int i, j, tmp_f;
	for (j = 0; j < VIDBUF_HEIGHT; j = j + 2) {
		for (i = 0; i < VIDBUF_WIDTH; i = i + 2) {
			tmp_f = (i + 160 * j) >> 1;
			buf_y[i + 320 * j] = frame_gray->imageData[tmp_f];
			buf_y[i + 1 + 320 * (j + 1)] = frame_gray->imageData[tmp_f];
		}
	}
	memcpy(vidbuf_overlay->ycbcr.y, buf_y, 76800);
}

//	remove top and bottom of the image
void cutIplImage(IplImage* frame) {
	int i, j, tmp;
	for (j = 0; j < BOTTOMLINE; j++) {
		for (i = 0; i < 160; i++) {
			tmp = i + 160 * j;
			frame->imageData[tmp] = 255;
		}
	}
	for (j = TOPLINE; j < 120; j++) {
		for (i = 0; i < 160; i++) {
			tmp = i + 160 * j;
			frame->imageData[tmp] = 255;
		}
	}
}

