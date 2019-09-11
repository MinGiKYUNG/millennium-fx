/*
 ============================================================================
 Name        : straight.h
 Author      : Team KKY
 Copyright   : 2012 Embedded Contest
 ============================================================================
 */

#define CV_RED		CV_RGB(0, 255, 0)
#define CV_BLUE		CV_RGB(255, 0, 0)
#define CV_LIME		CV_RGB(0, 0, 255)

//	average weight for side line
#define weight 0.7

//	struct for detectiong straight
typedef struct _STRAIGHT {
	int count;	// accumulated count of average straight
	double m;		// gradient
	int isCatchLine;	// detecting side line[0 : not find, 1 : find]
	CvPoint p1, p2;			// end point of the line
} Straight;

//	initiate Straight
void initStraight(Straight* line) {
	line->count = 0;
	line->m = 0.0;
	line->p1.x = 0;
	line->p1.y = 0;
	line->p2.x = 0;
	line->p2.y = 0;
	line->isCatchLine = 0;
}

void addCount(Straight* line) {
	line->count = line->count + 1;
}

//	set gradient of the line
void setM(Straight* line, CvPoint p1, CvPoint p2) {
	double temp;
	if (p2.x - p1.x != 0) {
		temp = (double) (p2.y - p1.y) / (double) (p2.x - p1.x);
		line->m = (line->m * (line->count - 1) + temp) / line->count;
	}
}

void setP1(Straight* line, CvPoint p1) {
	CvPoint temp = p1;
	line->p1.x = (int) ((p1.x * (line->count - 1) + temp.x) / line->count);
	line->p1.y = (int) ((p1.y * (line->count - 1) + temp.y) / line->count);
}

void setP2(Straight* line, CvPoint p2) {
	CvPoint temp = p2;
	line->p2.x = (int) ((p2.x * (line->count - 1) + temp.x) / line->count);
	line->p2.y = (int) ((p2.y * (line->count - 1) + temp.y) / line->count);
}

//	get gradient of the line
double getM(Straight* line) {
	return line->m;
}

//	get [x point] by using [line] and [y point]
int getX(Straight* line, int y) {
	double n = line->p2.y - line->m * line->p2.x;
	return (int) ((y - n) / line->m);
}

CvPoint getP1(Straight* line) {
	return line->p1;
}

CvPoint getP2(Straight* line) {
	return line->p2;
}

CvPoint getTop(Straight* line) {
	if (line->p1.y < line->p2.y)
		return line->p2;
	else return line->p1;
}

CvPoint getBottom(Straight* line) {
	if (line->p1.y < line->p2.y)
		return line->p1;
	else return line->p2;
}

//	if all points are 0(init state, empty) then return 1, else(not init state, not empty) then return 0;
int isEmpty(Straight* line) {
	if ((line->p1.x == 0) && (line->p1.y == 0) && (line->p2.x == 0) && (line->p2.y == 0))
		return 1;
	return 0;
}

//	initiate average value by measuring now value
//	-	avgLeft, avgRight : initate average measurement value
//	-	nowLeft, nowRight : first, now measurement value
void initAverageValue(Straight* avgLeft, Straight* avgRight, Straight* nowLeft, Straight* nowRight) {
	avgLeft->p1.x = nowLeft->p1.x;
	avgLeft->p1.y = nowLeft->p1.y;
	avgLeft->p2.x = nowLeft->p2.x;
	avgLeft->p2.y = nowLeft->p2.y;
	avgRight->p1.x = nowRight->p1.x;
	avgRight->p1.y = nowRight->p1.y;
	avgRight->p2.x = nowRight->p2.x;
	avgRight->p2.y = nowRight->p2.y;
}

//	calculate average lines
void calculateAverage(Straight* srcLine, Straight* dstLine) {
	dstLine->p1.x = weight * dstLine->p1.x + (1 - weight) * srcLine->p1.x;
	dstLine->p1.y = weight * dstLine->p1.y + (1 - weight) * srcLine->p1.y;
	dstLine->p2.x = weight * dstLine->p2.x + (1 - weight) * srcLine->p2.x;
	dstLine->p2.y = weight * dstLine->p2.y + (1 - weight) * srcLine->p2.y;
}

