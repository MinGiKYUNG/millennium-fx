/*
 ============================================================================
 Name        : drawDirectionLines.h
 Author      : Team KKY
 Version     : 4.0.0(updated 2012.09.23)
 Copyright   : 2012 Embedded Contest
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//	for excepting Horizontal Lines(p2.y - p1.y)
#define DIFF_Y	6
#define DIFF_X	7

CvPoint p1, p2;
CvPoint* line;

CvPoint dir_top, dir_bottom;

//	get representative outline(side) of the road(left, right)
void getRepresentLine(Straight* repLeft, Straight* repRight, CvSeq* lines) {
	int i, diff_x, diff_y;

	for (i = 0; i < lines->total; i++) {
		line = (CvPoint*) cvGetSeqElem(lines, i);
		p1 = line[0];
		p2 = line[1];

		diff_x = p2.x - p1.x;
		diff_y = p2.y - p1.y;
		//	except divide 0 and horizontal lines
		if (diff_x != 0 && (diff_y > DIFF_Y || diff_y < -DIFF_Y)) {
			if ((double) diff_y / (double) diff_x < 0) {	//	- : left side line
				//	���� ī��Ʈ ����
				addCount(repLeft);
				//	������ ��հ��� ����
				setM(repLeft, p1, p2);
				setP1(repLeft, p1);
				setP2(repLeft, p2);
			} else {	//	+ : right side line
				//	���� ī��Ʈ ����
				addCount(repRight);
				//	������ ��հ��� ����
				setM(repRight, p1, p2);
				setP1(repRight, p1);
				setP2(repRight, p2);
			}
		}
	}
}

//	�ܰ��� ��ǥ������ ���� ��� �ܰ��� ���� ����(by using average value filter)
void getAverageLine(Straight* repLeft, Straight* repRight, Straight* avgLeft, Straight* avgRight) {
	//	������ ��Ȳ���� average ���� �����Ƿ�, empty�� ��쿡 ��ǥ������ �ʱ�ȭ
	if ((isEmpty(avgLeft) == 1) && (isEmpty(avgRight) == 1)) {
		initAverageValue(avgLeft, avgRight, repLeft, repRight);
	}
	//	���� ������ ���� ���� ���� ���ٸ� ��հ��� ���� ���� ������ ����
	if (isEmpty(repLeft) == 1)
		memcpy(repLeft, avgLeft, sizeof(Straight));
	else calculateAverage(avgLeft, repLeft);
	//	���� ������ ������ ���� ���� ���ٸ� ��հ��� ������ ���� ������ ����
	if (isEmpty(repRight) == 1)
		memcpy(repRight, avgRight, sizeof(Straight));
	else calculateAverage(avgRight, repRight);
	memcpy(avgLeft, repLeft, sizeof(Straight));
	memcpy(avgRight, repRight, sizeof(Straight));
	initStraight(repLeft);
	initStraight(repRight);
}

//	��� �ܰ��� ���� ȭ�鿡 ���
void drawAverageLine(IplImage* frame, Straight* avgLeft, Straight* avgRight) {
	//	draw average left and right lines of the road
	cvLine(frame, getP1(avgLeft), getP2(avgLeft), CV_RED, 1, 8, 0);
	cvLine(frame, getP1(avgRight), getP2(avgRight), CV_RED, 1, 8, 0);
}

//	�̹����� �� ��� �߾� ���ؼ� ǥ��
void drawMidLine(IplImage* frame) {
	cvLine(frame, cvPoint(80, 0), cvPoint(80, 120), CV_BLUE, 1, 8, 0);
}

//	���� ������ ��Ÿ���� ������⼱ ǥ��
void drawDirLine(IplImage* frame, Straight* avgLeft, Straight* avgRight) {
	double n1, n2;
	n1 = getTop(avgLeft).y - getM(avgLeft) * getTop(avgLeft).x;
	n2 = getTop(avgRight).y - getM(avgRight) * getTop(avgRight).x;
	dir_top.x = (int) ((n2 - n1) / (getM(avgLeft) - getM(avgRight)));
	dir_top.y = (int) (getM(avgLeft) * dir_top.x + n1);
	dir_bottom.x = 80;
	dir_bottom.y = 0;
	//	�ҽ��� ���
	if (dir_top.y > 86 && (dir_top.x - 80 > DIFF_X))
		dir_top.x = dir_top.x - (dir_top.x - 80);
	else if (dir_top.y > 86 && (80 - dir_top.x > DIFF_X))
		dir_top.x = dir_top.x + (80 - dir_top.x);
	//	�̹����� �� ��� ������⼱ ǥ��
	cvLine(frame, dir_top, dir_bottom, CV_LIME, 1, 8, 0);
}

//	���� ������ ����
void determineDirection() {
	//	����
	if (dir_top.x - 80 > DIFF_X) 
		setting_hl(0xF4, 1600);
	//	������
	else if (80 - dir_top.x > DIFF_X)
		setting_hl(0xF4, 1400);
	else setting_hl(0xF4, 1500);
}

