#include "camera.h"

static Point tmp1 = { -1, -1 };
static Point tmp2 = { -1, -1 };



/* 点击鼠标执行 */
void OnMouseCallBack(int event, int x, int y, int flags, void *param)
{
	static bool click_flag = false;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		if (!click_flag)
		{
			tmp1.x = x;
			tmp1.y = y;
			click_flag = true;
		}
		else
		{
			tmp2.x = x;
			tmp2.y = y;
			click_flag = false;
		}
	}
}

MyCamera::MyCamera(int cap)
{
	int i = 0;
	/* 打开摄像头 */
loop:
	capture = VideoCapture(CAP_DSHOW);
	capture.open(cap);
	cout << "起始0" << endl;
	/* 设置摄像头 */
	capture.set(CAP_PROP_FOURCC, CAP_OPENCV_MJPEG);//设置为MJPG格式  CAP_OPENCV_MJPEG   VideoWriter::fourcc('M','J', 'P', 'G')
	//capture.set(CAP_PROP_FOURCC, CAP_ANY('M', 'J', 'P', 'G'));//设置为MJPG格式
	capture.set(CAP_PROP_FRAME_WIDTH, CAP_WIDTH);//宽度
	capture.set(CAP_PROP_FRAME_HEIGHT, CAP_HEIGHT);//高度
	capture.set(CAP_PROP_FPS, CAP_FPS);//帧率 帧/秒
	capture.set(CAP_PROP_BRIGHTNESS, -10);//亮度 
	capture.set(CAP_PROP_CONTRAST, 40);//对比度 40
	capture.set(CAP_PROP_SATURATION, 50);//饱和度 50
	capture.set(CAP_PROP_HUE, 50);//色调 50
	capture.set(CAP_PROP_EXPOSURE,-5);// -6.15);//曝光
	cout << "起始1" << endl;
	/* 获取可识别范围 */
	Mat frame;
	while (1)
	{
		capture >> frame;
		imshow("Vidio", frame);
		waitKey(20);
		i++;
		if (i == 100)
			break;
	}
	cout << "起始1.5" << endl;
	cvSetMouseCallback("Vidio", OnMouseCallBack, &frame);
	cout << "起始2" << endl;
	while (tmp2.y == -1) { waitKey(10); }   //出现一帧画面，点击不同的两点两次
	destroyWindow("Vidio");
	if (tmp1.x > tmp2.x || tmp1.y > tmp2.y)
	{
		i = 0;
		tmp1.x = -1;
		tmp1.y = -1;
		tmp2.x = -1;
		tmp2.y = -1;
		goto loop;
	}
	broad_position1.x = tmp1.x;
	broad_position1.y = tmp1.y;
	broad_position2.x = tmp2.x;
	broad_position2.y = tmp2.y;
	cout << "起始3" << endl;
	/* 初始化平板 */
	exchange_value_x = REAL_HEIGHT / (broad_position2.x - broad_position1.x);
	exchange_value_y = REAL_WIDTH / (broad_position2.y - broad_position1.y);

	cout << "平板起始： " << broad_position1.x << "平板起始：" << broad_position1.y << " "<<endl;
	cout << "平板结束： " << broad_position2.x << "平板结束：" << broad_position2.y << " "<<endl;
	cout << "交换宽： " << exchange_value_x << "交换长 " << exchange_value_y << " "<<endl;

	redpos = Point(200, 200);
}

bool MyCamera::IsOpen()
{
	if (!capture.isOpened())
	{
		return false;
	}
	return true;
}


void MyCamera::GetImgData(Mat &frame)
{
	capture >> frame;		//将摄像头的视频流转换成Mat格式的图像
	frame = frame(Range(broad_position1.y, broad_position2.y - 1), Range(broad_position1.x, broad_position2.x - 1));  //裁剪,保留鼠标点击的两点之间的图像
	ScanfRedPos(frame);  //将图像流实时画上红点的位置，且将红点涂为黑色
}
/*判断是否为红点*/
bool ifRed(uchar blue, uchar green, uchar red)
{
	if (red - blue > 80 && red - green > 80&& green<100&&blue<100)
		return true;
	else
		return false;
}

void MyCamera::ScanfRedPos(Mat &frame)
{
	int width = frame.cols;
	int height = frame.rows;
	uchar maxred = 50;
	redbegin = Point(frame.cols, frame.rows);
	redend =Point(0, 0);
	for (int i = 0; i < height; i++)
	{
		uchar* p = frame.ptr(i);
		for (int j = 0; j < width; j++)
		{
			int q = *(p + j * 3);
			if (ifRed(*(p + j * 3 + 0), *(p + j * 3 + 1), *(p + j * 3 + 2)))  //得到红点的起始和结束位置
			{
				if (i < redbegin.y)redbegin.y = i;
				if (j < redbegin.x)redbegin.x = j;
				if (i > redend.y)redend.y = i;
				if (j > redend.x)redend.x = j;
			}
		}
	}
	if (redbegin != Point(frame.cols, frame.rows) && redend != Point(0, 0))  //
	{

		circle(frame, Point((redbegin.x + redend.x) / 2, (redbegin.y + redend.y) / 2), 5, CV_RGB(0, 0, 0), 5, 8, 0);//把红点在图像上涂黑，避免影响小球识别
		redpos.x = (int)((redbegin.x + redend.x) / 2 * exchange_value_x);
		redpos.y = (int)((redbegin.y + redend.y) / 2 * exchange_value_y);
	}
	if (redpos.x >= REAL_HEIGHT - Redpos_limt)redpos.x = REAL_HEIGHT - Redpos_limt;
	if (redpos.x <= Redpos_limt)redpos.x = Redpos_limt;
	if (redpos.y >= REAL_HEIGHT - Redpos_limt)redpos.y = REAL_HEIGHT - Redpos_limt;
	if (redpos.y <= Redpos_limt)redpos.y = Redpos_limt;
}

void MyCamera::GetRedPos(Point & pos)
{
	pos = redpos;
}

void MyCamera::GetExchangeValue(double &k1, double &k2)
{
	k1 = exchange_value_x;
	k2 = exchange_value_y;
}


void MyCamera::PrintBroadPosition()
{
	cout << "The broad_position1 is:(" << broad_position1.x << "," << broad_position1.y << ")"; //木板位置
	cout << "(" << broad_position2.x << "," << broad_position2.y << ")" << endl;
}


void MyCamera::PrintExchangeValue()
{
	cout << "The exchange_value_x is: " << exchange_value_x << endl; //交换值
	cout << "The exchange_value_y is: " << exchange_value_y << endl;
}
