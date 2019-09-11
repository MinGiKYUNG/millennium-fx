/*
 ============================================================================
 Name        : imageProcessModule.h
 Author      : Team KKY
 Version     : 5.0.0(updated 2012.11.01)
 Copyright   : 2012 Embedded Test
 ============================================================================
 */

//	for multi-processing, multi-threading
struct pxa_video_buf*	vidbuf;
struct pxa_video_buf	vidbuf_overlay;
IplImage* frame;				// 3 channels image
IplImage* frame_out;	// 1 channel image

//	global variables
double cffn[] = { -1.0, 0.0, 1.0 };
CvMat kernel;
CvMemStorage* storage;
CvSeq* lines;
//	side lines(left, right)
Straight repLeft, repRight;	//	representative = average lines after Hough Transfer
Straight avgLeft, avgRight;	//	average(final result in LCD)

//	initiate imageProcessModule
void init_imageProcessModule() {
	pthread_mutex_init(&mutex, NULL);
	//	for IplImage Open CV
	frame = cvCreateImage(cvSize(160, 120), IPL_DEPTH_8U, 3);
	frame_out = cvCreateImage(cvSize(160, 120), IPL_DEPTH_8U, 1);
	//	for 2D Correlation Filter
	kernel = cvMat(1, 3, CV_64FC1, cffn);
	//	for Hough-Transform
	storage = cvCreateMemStorage(0);
	lines = NULL;
	//	for finding side lines
	initStraight(&repLeft);
	initStraight(&repRight);
	initStraight(&avgLeft);
	initStraight(&avgRight);
}

//	thread 0
void get_Image_Function() {
	pthread_mutex_lock(&mutex);
	//	vidbuf -> frame(3 channels), frame_out(1 channel)
	cvVidBuf2IplImage(vidbuf, frame, frame_out);
	cutIplImage(frame_out);
	//	2D Correlation Filter
	cvFilter2D(frame_out, frame_out, &kernel, cvPoint(-1, -1));
	//	Auto Threshold
	cvAdaptiveThreshold(frame_out, frame_out, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 3, 5);
	pthread_mutex_unlock(&mutex);
	usleep(1);
}

//	thread 0 : get_Image
void* get_Image(void* in) {
	int handle;
	struct pxacam_setting camset;

	handle = camera_open(NULL, 0);
	ASSERT(handle);
	system("echo b > /proc/invert/tb");

	memset(&camset, 0, sizeof(camset));
	camset.mode = CAM_MODE_VIDEO;
	camset.format = pxavid_ycbcr422;
	camset.width = 320;
	camset.height = 240;
	camera_config(handle, &camset);
	camera_start(handle);

	while (1) {
		vidbuf = camera_get_frame(handle);
		//	call image process function
		get_Image_Function();
		camera_release_frame(handle, vidbuf);
	}
	camera_stop(handle);
	camera_close(handle);
	return 0;
}

//	thread 1
void process_Image_Function() {
	pthread_mutex_lock(&mutex);
	
	//	Hough-Transform
	lines = cvHoughLines2(frame_out, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 44, 10, 50);
	printf("> the number of Hough lines = %d\n", lines->total);
	
	//	양 쪽 측면의 외곽 차선 대표값 추출(representative lines = average lines after Hough Transform)
	getRepresentLine(&repLeft, &repRight, lines);
	getAverageLine(&repLeft, &repRight, &avgLeft, &avgRight);
	//drawAverageLine(frame, &repLeft, &repRight);
	drawAverageLine(frame, &avgLeft, &avgRight);
	
	//	이미지의 한 가운데 중앙선 표시
	drawMidLine(frame);
	//	주행 방향을 나타내는 주행방향선 표시
	//drawDirLine(frame, &avgLeft, &avgRight);

	//	차량의 주행방향 설정
	//determineDirection();

	cvIplImage2VidBuf(frame, &vidbuf_overlay);
	//cvIplImage_Gray2VidBuf(frame_out, &vidbuf_overlay);
	pthread_mutex_unlock(&mutex);
	usleep(1);
}

//	thread 1 : process_Image
void* process_Image(void* in) {
	int handle_overlay2;
	handle_overlay2 = overlay2_open(NULL, pxavid_ycbcr422, NULL, 320, 240, 0, 0);
	overlay2_getbuf(handle_overlay2, &vidbuf_overlay);
	while (1) {
		//	call image process function
		process_Image_Function();
	}
	overlay2_close(handle_overlay2);
	return 0;
}

//	release all resources
void release_imageProcessModule() {
	cvReleaseImage(&frame);
	cvReleaseImage(&frame_out);
	pthread_mutex_destroy(&mutex);
}

