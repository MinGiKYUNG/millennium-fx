/*
 ============================================================================
 Name        : change_line.h
 Author      : Jincheol Kim
 Version     : 1.0.0(updated 2012.11.01)
 Copyright   : 2012 Embedded Test
 ============================================================================
 */

//	method for changing line
//void CHANGE_LINE() {
//}

//	detect dot line
void detect_DotLine() {
	int i, tmp;
	for (i = 0; i < lines->total; i++) {
		line = (CvPoint*) cvGetSeqElem(lines, i);
		p1 = line[0];
		p2 = line[1];
		tmp = (3 * p1.x + 480 * p1.y);
		cvLine(frame, p1, p2, CV_RED, 1, 8, 0);
		printf("%d, %d, %d\n", frame->imageData[tmp], frame->imageData[tmp+1],frame->imageData[tmp+2]);
	}
}
