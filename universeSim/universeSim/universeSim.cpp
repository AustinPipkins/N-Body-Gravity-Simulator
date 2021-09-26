#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <cmath>

#define float double

using namespace cv;
using namespace std;

const float pi = 3.14159;
const short WIN_WIDTH = 1100;
const long MAX_DIST_FROM_ORIGIN = 150000;
const short WIN_HEIGHT = 750;
const short NUM_GRID_LAYERS = 3;
const float ZOOM_MULT = 1.2;
const float G = 200 * 100000;
const float TIME_STEP = .001;

const string WIN_NAME = "Space Simulator";



const float NUM_PIX_BETWEEN_LAYER_1 = 500;
const float NUM_PIX_BETWEEN_LAYER_2 = NUM_PIX_BETWEEN_LAYER_1 / 4.0;
const float NUM_PIX_BETWEEN_LAYER_3 = (NUM_PIX_BETWEEN_LAYER_1) / 16.0;

const short STATS_WIN_WIDTH = 250;
//   \/  /\ these two should be the same!!!
const short LOGOW = 250;
const short LOGOH = 200;

const short STOP_SHOW = 10;







struct mouseBox;


//NOTES
//if logo fails, change path owo


struct cord
{
	float x;
	float y;
	cord() { x = -1; y = -1; }
	cord(float x1, float y1) { x = x1; y = y1; }
};


class object;





class object
{
private:
	cord position;
	float mass;
	cord velocity;
	cord acceleration; //not acctually acceleration, is force, dont kill me mccoy please!


public:

	object(float x, float y, float m, cord vel);
	cord getPos() const;
	float getMass() const;
	cord getAcc() const;
	cord getVel() const;

	void updateAcceleration(const vector<object> & bodies);
	void updateVelocity(float timeStep);
	void updatePosition(float timeStep);

	friend ostream& operator<<(ostream& os, const object& p);

};

cord object::getVel() const
{
	return velocity;
}

ostream& operator<<(ostream& os, const object &p)
{
	os << "pos: " << p.getPos().x << " " << p.getPos().y << " acc: " << p.getAcc().x << " " << p.getAcc().y;
	return os;
}

cord object::getAcc() const
{
	return acceleration;
}

double sameSign(double a, double b)
{
	if (a > 0)//a is pos
	{
		b *= (b > 0) ? 1 : -1;
	}
	else//a is neg
	{
		b *= (b > 0) ? -1 : 1;
	}

	return b;
}

void object::updateAcceleration(const vector<object> & bodies)
{
	cord newA;
	newA.x = 0;
	newA.y = 0;

	double grav;
	double dist;
	double theta;

	for (short i = 0; i < bodies.size(); i++)
	{
		if (!(bodies[i].position.x == position.x && bodies[i].position.y == position.y && bodies[i].mass == mass))
		{

			dist = sqrt(pow((bodies[i].position.x - position.x), 2)+ pow((bodies[i].position.y - position.y), 2));
			grav = G * ((bodies[i].mass) / (pow((dist),2)));
			theta = atan((bodies[i].position.y - position.y) / (bodies[i].position.x - position.x));
			
			newA.x += sameSign((bodies[i].position.x - position.x), grav * cos(theta));
			newA.y += sameSign((bodies[i].position.y - position.y), grav * sin(theta));

		
		}
	}

	//cout << newA.x << " "<< newA.y<< "pooop" << endl;

	acceleration = newA;
}



void object::updateVelocity(float timeStep)
{
	velocity.x += (acceleration.x) * timeStep;
	velocity.y += (acceleration.y) * timeStep;

	
}

void object::updatePosition(float timeStep)
{
	position.x += velocity.x * timeStep;
	position.y += velocity.y * timeStep;
}



float object::getMass() const
{
	return mass;
}

object::object(float x, float y, float m, cord vel)
{
	position.x = x;
	position.y = y;
	mass = m;
	velocity = vel;



	acceleration.x = 0;
	acceleration.y = 0;



}

cord object::getPos() const
{
	return position;
}





class Grid
{
private:
	vector<float> vert_line_pos[NUM_GRID_LAYERS];
	vector<float> hor_line_pos[NUM_GRID_LAYERS];
	float line_distances[NUM_GRID_LAYERS];
	cord center;
	float pixPerMeter;

public:
	Grid();

	void showGrid(Mat cv);
	void zoomIn();
	void zoomOut();

	void moveH(short numPix);
	void moveV(short numPix);

	void addLines();
	void subLines();

	//void updateHierarchy();

	float getPixPerMeter();

	cord getCenter();

};

cord Grid::getCenter()
{
	return center;
}

float Grid::getPixPerMeter()
{
	return pixPerMeter;
}
/*
void Grid::updateHierarchy()
{


	if (line_distances[1] >= NUM_PIX_BETWEEN_LAYER_1 || vert_line_pos[0].size() == 1 || hor_line_pos[0].size() == 1)
	{
		//zoom in
		line_distances[0] = line_distances[1];
		line_distances[1] = line_distances[2];
		line_distances[2] = line_distances[2] / 4.0;

		vert_line_pos[0] = vert_line_pos[1];
		vert_line_pos[1] = vert_line_pos[2];
		vert_line_pos[2].clear();
		vert_line_pos[2].push_back(vert_line_pos[1][0]);

		hor_line_pos[0] = hor_line_pos[1];
		hor_line_pos[1] = hor_line_pos[2];
		hor_line_pos[2].clear();
		hor_line_pos[2].push_back(hor_line_pos[1][0]);

		addLines();
	}

	if (line_distances[0] <= NUM_PIX_BETWEEN_LAYER_2)
	{
		//zoomout
		line_distances[2] = line_distances[1];
		line_distances[1] = line_distances[0];
		line_distances[0] = line_distances[1] * 4.0;




		
		vert_line_pos[2] = vert_line_pos[1];
		vert_line_pos[1] = vert_line_pos[0];
		vert_line_pos[0].clear();
		vert_line_pos[0].push_back(vert_line_pos[1][0]);

		hor_line_pos[2] = hor_line_pos[1];
		hor_line_pos[1] = hor_line_pos[0];
		hor_line_pos[0].clear();
		hor_line_pos[0].push_back(hor_line_pos[1][0]);

		addLines();
	}


}
*/

void Grid::addLines()
{
	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		bool check = 1;
		do {
			if (hor_line_pos[i][0] - line_distances[i] > 0)
			{
				hor_line_pos[i].insert(hor_line_pos[i].begin(), hor_line_pos[i][0] - line_distances[i]);
			}
			else
			{
				check = 0;
			}
		} while (check);

		check = 1;
		do {
			if (vert_line_pos[i][0] - line_distances[i] > 0)
			{
				vert_line_pos[i].insert(vert_line_pos[i].begin(), vert_line_pos[i][0] - line_distances[i]);
			}
			else
			{
				check = 0;
			}
		} while (check);







		check = 1;
		do {
			if (vert_line_pos[i][vert_line_pos[i].size()-1] + line_distances[i] < WIN_WIDTH)
			{
				vert_line_pos[i].push_back(vert_line_pos[i][vert_line_pos[i].size() - 1] + line_distances[i]);
			}
			else
			{
				check = 0;
			}
		} while (check);

		check = 1;
		do {
			if (hor_line_pos[i][hor_line_pos[i].size() - 1] + line_distances[i] < WIN_HEIGHT)
			{
				hor_line_pos[i].push_back(hor_line_pos[i][hor_line_pos[i].size() - 1] + line_distances[i]);
			}
			else
			{
				check = 0;
			}
		} while (check);



	}
}

void Grid::subLines()
{

	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{


		bool check = 1;
		while (check && hor_line_pos[i].size() > 1) {
			if (hor_line_pos[i][0] < 0)
			{
				hor_line_pos[i].erase(hor_line_pos[i].begin());
			}
			else
			{
				check = 0;
			}
		};

		check = 1;
		while (check && vert_line_pos[i].size() > 1) {
			if (vert_line_pos[i][0] < 0)
			{
				vert_line_pos[i].erase(vert_line_pos[i].begin());
			}
			else
			{
				check = 0;
			}
		}


		



		check = 1;
		while (check && hor_line_pos[i].size() > 1) {
			if (hor_line_pos[i][hor_line_pos[i].size() - 1] > WIN_HEIGHT)
			{
				hor_line_pos[i].pop_back();
			}
			else
			{
				check = 0;
			}
		}

		check = 1;
		while (check && vert_line_pos[i].size() > 1) {
			if (vert_line_pos[i][vert_line_pos[i].size() - 1] > WIN_WIDTH)
			{
				vert_line_pos[i].pop_back();
			}
			else
			{
				check = 0;
			}
		}

		

	}
	
}


void Grid::moveH(short numPix)
{
	center.x += -numPix * 1.0/pixPerMeter;
	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < vert_line_pos[i].size(); j++)
		{
			vert_line_pos[i][j] = vert_line_pos[i][j] + numPix;
		}
	}

	addLines();
	subLines();
}
void Grid::moveV(short numPix)
{
	center.y += -numPix * 1.0/pixPerMeter;
	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < hor_line_pos[i].size(); j++)
		{
			hor_line_pos[i][j] = hor_line_pos[i][j] + numPix;
		}
	}

	addLines();
	subLines();
}

void Grid::zoomIn()
{

	pixPerMeter *= ZOOM_MULT;
	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < vert_line_pos[i].size(); j++)
		{
			vert_line_pos[i][j] = ((vert_line_pos[i][j] - (WIN_WIDTH / 2.0)) * ZOOM_MULT) + (WIN_WIDTH / 2.0);
		}
	}

	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < hor_line_pos[i].size(); j++)
		{
			hor_line_pos[i][j] = ((hor_line_pos[i][j] - (WIN_HEIGHT / 2.0)) * ZOOM_MULT) + (WIN_HEIGHT / 2.0);
		}
	}
	
	line_distances[0] = line_distances[0] * ZOOM_MULT;
	line_distances[1] = line_distances[1] * ZOOM_MULT;
	line_distances[2] = line_distances[2] * ZOOM_MULT;
	//updateHierarchy();

	subLines();

}

void Grid::zoomOut()
{
	float zoom_mult = 1.0/ZOOM_MULT;
	pixPerMeter *= 1.0 / ZOOM_MULT;
	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < vert_line_pos[i].size(); j++)
		{
			vert_line_pos[i][j] = ((vert_line_pos[i][j] - (WIN_WIDTH / 2.0)) * zoom_mult) + (WIN_WIDTH / 2.0);
		}
	}

	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		for (short j = 0; j < hor_line_pos[i].size(); j++)
		{
			hor_line_pos[i][j] = ((hor_line_pos[i][j] - (WIN_HEIGHT / 2.0)) * zoom_mult) + (WIN_HEIGHT / 2.0);
		}
	}

	line_distances[0] = line_distances[0] * zoom_mult;
	line_distances[1] = line_distances[1] * zoom_mult;
	line_distances[2] = line_distances[2] * zoom_mult;

	//updateHierarchy();
	addLines();
}

void Grid::showGrid(Mat cv)
{
	rectangle(cv, Point(0, 0), Point(WIN_WIDTH, WIN_HEIGHT), Scalar(20, 20, 20), FILLED);

	int a;
	for (short i = NUM_GRID_LAYERS - 1; i >= 0; i--)
	{
		if (line_distances[i] > STOP_SHOW)
		{
			a = (((line_distances[i] - 18) / 282.0) * 140 + 50);
			Scalar color(a, a, a);
			for (short j = vert_line_pos[i].size() - 1; j >= 0; j--)
			{
				line(cv, Point(static_cast<int>(vert_line_pos[i][j]), 0), Point(static_cast<int>(vert_line_pos[i][j]), WIN_HEIGHT), color, 1);
			}

			for (short j = hor_line_pos[i].size() - 1; j >= 0; j--)
			{
				line(cv, Point(0, static_cast<int>(hor_line_pos[i][j])), Point(WIN_WIDTH, static_cast<int>(hor_line_pos[i][j])), color, 1);
			}
		}

		
	}

	//cout << center.x << " " << center.y << " " << pixPerMeter << endl;

}

Grid::Grid()
{
	center.x = 0;
	center.y = 0;
	pixPerMeter = 1;


	line_distances[0] = NUM_PIX_BETWEEN_LAYER_1;
	line_distances[1] = NUM_PIX_BETWEEN_LAYER_2;
	line_distances[2] = NUM_PIX_BETWEEN_LAYER_3;


	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		vert_line_pos[i].push_back(WIN_WIDTH / 2.0);
		hor_line_pos[i].push_back(WIN_HEIGHT / 2.0);
	}

	//vert
	for (short i = 1; i < static_cast<int>(WIN_WIDTH / 2.0 / NUM_PIX_BETWEEN_LAYER_1) + 1; i++)
	{
		vert_line_pos[0].push_back((WIN_WIDTH / 2.0) - i * NUM_PIX_BETWEEN_LAYER_1);
		vert_line_pos[0].push_back((WIN_WIDTH / 2.0) + i * NUM_PIX_BETWEEN_LAYER_1);
	}

	//hor
	for (short i = 1; i < static_cast<int>(WIN_HEIGHT / 2.0 / NUM_PIX_BETWEEN_LAYER_1) + 1; i++)
	{
		hor_line_pos[0].push_back((WIN_HEIGHT / 2.0) - i * NUM_PIX_BETWEEN_LAYER_1);
		hor_line_pos[0].push_back((WIN_HEIGHT / 2.0) + i * NUM_PIX_BETWEEN_LAYER_1);
	}




	//vert
	for (short i = 1; i < static_cast<int>(WIN_WIDTH / 2.0 / NUM_PIX_BETWEEN_LAYER_2) + 1; i++)
	{
		vert_line_pos[1].push_back((WIN_WIDTH / 2.0) - i * NUM_PIX_BETWEEN_LAYER_2);
		vert_line_pos[1].push_back((WIN_WIDTH / 2.0) + i * NUM_PIX_BETWEEN_LAYER_2);
	}

	//hor
	for (short i = 1; i < static_cast<int>(WIN_HEIGHT / 2.0 / NUM_PIX_BETWEEN_LAYER_2) + 1; i++)
	{
		hor_line_pos[1].push_back((WIN_HEIGHT / 2.0) - i * NUM_PIX_BETWEEN_LAYER_2);
		hor_line_pos[1].push_back((WIN_HEIGHT / 2.0) + i * NUM_PIX_BETWEEN_LAYER_2);
	}



	//vert
	for (short i = 1; i < static_cast<int>(WIN_WIDTH / 2.0 / NUM_PIX_BETWEEN_LAYER_3) + 1; i++)
	{
		vert_line_pos[2].push_back((WIN_WIDTH / 2.0) - i * NUM_PIX_BETWEEN_LAYER_3);
		vert_line_pos[2].push_back((WIN_WIDTH / 2.0) + i * NUM_PIX_BETWEEN_LAYER_3);
	}

	//hor
	for (short i = 1; i < static_cast<int>(WIN_HEIGHT / 2.0 / NUM_PIX_BETWEEN_LAYER_3) + 1; i++)
	{
		hor_line_pos[2].push_back((WIN_HEIGHT / 2.0) - i * NUM_PIX_BETWEEN_LAYER_3);
		hor_line_pos[2].push_back((WIN_HEIGHT / 2.0) + i * NUM_PIX_BETWEEN_LAYER_3);
	}


	

	for (short i = 0; i < NUM_GRID_LAYERS; i++)
	{
		sort(hor_line_pos[i].begin(), hor_line_pos[i].end());
		sort(vert_line_pos[i].begin(), vert_line_pos[i].end());
	}


}

class Stats
{
private:
	char setting; // 'm' = menu, 's' = specific body, 'n' = new body
	object* specificBody;


public:
	Stats();
	Mat makeStatsImg(vector<object> bodies);

	void makeMenu(Mat& cv, vector<object> bodies);

	void makeSpecific(Mat& cv, vector<object> bodies);

	void setSetting(char newS);

	char getSetting() const;

	void setSBody(object* newSB);
};

char Stats::getSetting() const
{
	return setting;
}


void Stats::setSetting(char newS)
{
	setting = newS;
}

void Stats::setSBody(object* newSB)
{
	specificBody = newSB;
}

Stats::Stats()
{
	setting = 'm';
	specificBody = nullptr;
}


Mat Stats::makeStatsImg(vector<object> bodies)
{
	Mat logo2 = imread("C:\\Users\\aus3d\\Documents\\c++\\solarSim\\logo.bmp", -1);
	Mat logo;

	logo2.convertTo(logo, CV_8UC3);

	Mat screen(WIN_HEIGHT - LOGOH, STATS_WIN_WIDTH, CV_8UC3);

	if (setting == 'm')
	{
		makeMenu(screen, bodies);
	}
	else
	{
		makeSpecific(screen, bodies);
	}


	Mat output(WIN_HEIGHT, STATS_WIN_WIDTH, CV_8UC3);



	vconcat(logo, screen, output);

	return output;
}




void Stats::makeMenu(Mat& cv, vector<object> bodies)
{
	const short numBodies = 500;

	const short numMenu = 75;

	const short numClick = 400;




	string msg;
	rectangle(cv, Point(0, 0), Point(STATS_WIN_WIDTH, WIN_HEIGHT - LOGOH), Scalar(20, 20, 20), FILLED);




	//msg = "Menu";
	//putText(cv, msg, Point(10, numMenu), 2, 2, Scalar(200, 200, 200), 3);
	//line(cv, Point(10, numMenu + 15), Point(175, numMenu + 15), Scalar(200, 200, 200), 3);


	//line(cv, Point(10, numClick - 40), Point(230, numClick - 40), Scalar(200, 200, 200), 2);

	//msg = "Click a body to see";
	//putText(cv, msg, Point(10, numClick), 2, .7, Scalar(200, 200, 200), 1.5);
	//msg = "its stats";
	//putText(cv, msg, Point(70, numClick + 35), 2, .7, Scalar(200, 200, 200), 1.5);

	line(cv, Point(10, numClick + 60), Point(230, numClick + 60), Scalar(200, 200, 200), 2);

	msg = "Number of Bodies:";
	putText(cv, msg, Point(10, numBodies), 2, .7, Scalar(200, 200, 200), 1.5);

	msg = to_string(bodies.size());
	putText(cv, msg, Point(10, numBodies + 35), 2, 1, Scalar(200, 200, 200), 1.5);

}

class object;

void Stats::makeSpecific(Mat& cv, vector<object> bodies)
{
	const short numBodies = 500;
	const float vArrowMultiplier = 80;
	const short numCords = 250;
	const float zeroThresh = 0.01;
	cord v;
	float vx;
	float vy;
	float speed;
	float theta;


	string msg;
	rectangle(cv, Point(0, 0), Point(STATS_WIN_WIDTH, WIN_HEIGHT - LOGOH), Scalar(20, 20, 20), FILLED);


	circle(cv, Point(130, 130), 50, Scalar(200, 0, 0), FILLED);

	v = specificBody->getVel();
	vx = (abs(v.x) > abs(v.y)) ? v.x/abs(v.x) : v.x / v.y;
	vy = -((abs(v.x) < abs(v.y)) ? v.y/abs(v.y) : v.y / v.x);

	vx = (abs(v.x) < zeroThresh) ? 0.01 : vx;
	vy = (abs(v.y) < zeroThresh) ? 0.01 : vy;
	
	theta = atan(vy / vx);


	line(cv, Point(130, 130), Point(((vArrowMultiplier * sameSign(vx, cos(theta))) + 130), ((vArrowMultiplier * sameSign(vy, sin(theta))) + 130)), Scalar(230, 230, 230), 4);

	speed = sqrt(pow(v.x, 2) + pow(v.y, 2));


	cout << v.x << endl;

	if (specificBody->getPos().x < 0)
	{
		msg = "X cord:" + to_string(specificBody->getPos().x);
	}
	else
	{
		msg = "X cord: " + to_string(specificBody->getPos().x);
	}
	putText(cv, msg, Point(10, numCords), 2, .5, Scalar(200, 200, 200), 1);

	if (specificBody->getPos().y < 0)
	{
		msg = "Y cord:" + to_string(specificBody->getPos().y);
	}
	else
	{
		msg = "Y cord: " + to_string(specificBody->getPos().y);
	}
	putText(cv, msg, Point(10, numCords+35), 2, .5, Scalar(200, 200, 200), 1);

	if (specificBody->getVel().x < 0)
	{
		msg = "X vel:" + to_string(specificBody->getVel().x);
	}
	else
	{
		msg = "X vel: " + to_string(specificBody->getVel().x);
	}
	putText(cv, msg, Point(10, numCords+70), 2, .5, Scalar(200, 200, 200), 1);

	if (specificBody->getVel().y < 0)
	{
		msg = "Y vel:" + to_string(specificBody->getVel().y);
	}
	else
	{
		msg = "Y vel: " + to_string(specificBody->getVel().y);
	}
	putText(cv, msg, Point(10, numCords + 35+ 70), 2, .5, Scalar(200, 200, 200), 1);

	if (speed < 0)
	{
		msg = "Speed:" + to_string(speed);
	}
	else
	{
		msg = "Speed: " + to_string(speed);
	}
	putText(cv, msg, Point(10, numCords + 140), 2, .5, Scalar(200, 200, 200), 1);

	msg = "Mass " + to_string(specificBody->getMass());
	putText(cv, msg, Point(10, numCords + 140+35), 2, .5, Scalar(200, 200, 200), 1);

	cout << msg << endl;




	msg = "Number of Bodies:";
	putText(cv, msg, Point(10, numBodies), 2, .7, Scalar(200, 200, 200), 1.5);

	msg = to_string(bodies.size());
	putText(cv, msg, Point(10, numBodies + 35), 2, 1, Scalar(200, 200, 200), 1.5);
}






int z = 0;
int m = 0;
int hChange = 0;
int vChange = 0;
int xP = -1;
int yP = -1;
int mx = -1;
int my = -1;
static void onMouse(int event, int x, int y, int flags, void* arg)
{
	//mouseBox* mb = static_cast<mouseBox*>(arg);
	//cout << mb->poop << endl;
	if (event == EVENT_RBUTTONDOWN)
	{
		m = 1;
	}
	if (event == EVENT_RBUTTONUP)
	{
		m = 0;
	}

	if (event == EVENT_LBUTTONDOWN)
	{
		m = -1;
		mx = x;
		my = y;
	}



	if (m == 1)
	{
		hChange = x-xP;
		vChange = y-yP;
	}

	if (event == EVENT_MOUSEWHEEL)
	{
		if (getMouseWheelDelta(flags) > 0)
			z = 1;
		else
			z = -1;
	}

	xP = x;
	yP = y;


}



void showObjects(const vector<object> &objects, Mat cv, Grid grid);
void updateAllBodies(vector<object>& bodies, float timeStep, Grid grid);
void collionProcessing(vector<object>& bodies, Grid grid);
vector<object> makeBodyList1();
vector<object> makeBodyList2();
void addAsteroids(vector<object> & bodies, short num, float lowerR, float upperR);
void compileAndShowScreen(Grid grid, Stats stats, vector<object> planets);
void LeftMouseHandeler(vector<object> bodies, Grid grid, Stats & stats);

int main()//===============================================================================================================================
{
	srand(time(0));
	Grid grid;

	Stats stats;


	

	//make window
	namedWindow(WIN_NAME);


	//mouse react
	setMouseCallback(WIN_NAME, onMouse);
	auto FPS1 = chrono::steady_clock::now();
	auto FPS2 = chrono::steady_clock::now();
	short FPS3 = 0;



	//make bodies list

	//vector<object> planets = makeBodyList2();
	vector<object> planets;
	//cord velo(0, 0);
	//object sun(0, 0, 350,velo);
	//planets.push_back(sun);

	planets = makeBodyList1();

	

	

	//loop
	while(1)
	{
		
		updateAllBodies(planets, TIME_STEP, grid);

		compileAndShowScreen(grid, stats, planets);

		



		if (z == 1 && grid.getPixPerMeter() < 200000)
		{
			grid.zoomIn();
			z = 0;
		}
		if (z == -1 && grid.getPixPerMeter() > .0000001)
		{
			grid.zoomOut();
			z = 0;
		}
		if (m == 1)
		{
			grid.moveH(hChange);
			grid.moveV(vChange);
		}
		if (m == -1)
		{
			LeftMouseHandeler(planets, grid, stats);

			m = 0;
		}

		//fps

		FPS1 = chrono::steady_clock::now();
		if (FPS3 % 50 == 0)
		{
			//cout << "FPS: " << 1 / ((chrono::duration_cast<chrono::nanoseconds>(FPS1 - FPS2).count()) * pow(10, -9)) << endl;
			//cout << "Bodies: " << planets.size() << endl;
			FPS3 = 0;
		}
		FPS2 = chrono::steady_clock::now();
		FPS3++;
		//~fps


		//vitals====================================
		waitKey(1);
		if (!getWindowProperty(WIN_NAME, WND_PROP_VISIBLE))
		{
			exit(1);
		}
	}

	return 0;
}


void showObjects(const vector<object> & objects, Mat cv, Grid grid)
{
	float r;
	for (short i = 0; i < objects.size(); i++)
	{
		r = 10* cbrt(objects[i].getMass()) * grid.getPixPerMeter();
		circle(cv, Point((objects[i].getPos().x-grid.getCenter().x) * grid.getPixPerMeter() + WIN_WIDTH/2.0, (-objects[i].getPos().y- grid.getCenter().y) * grid.getPixPerMeter() + WIN_HEIGHT/2.0), r, Scalar(250, 10, 0), FILLED);
	}

}

void updateAllBodies(vector<object>& bodies, float timeStep, Grid grid)
{

	for (short i = 0; i < bodies.size(); i++)
	{
		bodies[i].updateAcceleration(bodies);
	}


	for (short i = 0; i < bodies.size(); i++)
	{
		bodies[i].updateVelocity(timeStep);
		bodies[i].updatePosition(timeStep);
	}

	collionProcessing(bodies, grid);
}

void collionProcessing(vector<object>& bodies, Grid grid)
{
	//vector<object> copy = bodies;

	float r1;
	float r2;

	float dist;

	cord vel;

	double imass;
	double jmass;
	cord ipos;
	cord jpos;

	float massSum;

	for (short i = 0; i < bodies.size(); i++)
	{
		for (short j = 0; j < bodies.size(); j++)
		{
			if (i != j)
			{
				imass = bodies[i].getMass();
				jmass = bodies[j].getMass();
				ipos = bodies[i].getPos();
				jpos = bodies[j].getPos();


				r1 = 10 * cbrt(imass) * grid.getPixPerMeter();
				r2 = 10 * cbrt(jmass) * grid.getPixPerMeter();
				dist = sqrt(pow((ipos.x-jpos.x),2)+pow((ipos.y - jpos.y),2)) * grid.getPixPerMeter();

				if (r1 + r2 >= dist)
				{
					

					massSum = (imass + jmass);

					r1 = ipos.x + (jmass / (massSum)) * (jpos.x - ipos.x);
					r2 = ipos.y + (jmass / (massSum)) * (jpos.y - ipos.y);
					vel.x = (imass * bodies[i].getVel().x + jmass * bodies[j].getVel().x) / (massSum);
					vel.y = (imass * bodies[i].getVel().y + jmass * bodies[j].getVel().y) / (massSum);
					object newPlanet(r1, r2, massSum, vel);

					bodies.push_back(newPlanet);

					bodies.erase(bodies.begin() + ((i > j) ? i : j));
					bodies.erase(bodies.begin() + ((i > j) ? j : i));

					i = (i > 0) ? i - 1 : i;
					j = bodies.size();

					cout << "crash! " << bodies.size() << " bodies left" << endl;
				}
			}
		}
	}
	
	for (short i = 0; i < bodies.size(); i++)
	{
		dist = sqrt(pow((bodies[i].getPos().x), 2) + pow((bodies[i].getPos().y), 2));
		//cout << " " << bodies[i].getPos().y << endl;
		if (dist > MAX_DIST_FROM_ORIGIN)
		{
			
			//cout << "wiped!" << endl;
			bodies.erase(bodies.begin() + i);
			i = i - 1;
		}


	}

}


vector<object> makeBodyList1()
{
	vector<object> planets;
	int xpo;
	int ypo;
	float ma;
	cord v(0, 0);
	//=================================================================================================================
	int xyrange = 2000;
	int mrange = 5;
	long vrange = 150000;
	cout << "start" << endl;
	for (long i = 0; i < 300; i++)
	{
		xpo = rand() % xyrange - xyrange / 2.0;
		ypo = rand() % xyrange - xyrange / 2.0;
		ma = (rand() % (mrange * 4)) / 4.0 + 0.1;
		v.x = ((rand() % vrange) - vrange / 2.0) * 10;
		v.y = ((rand() % vrange) - vrange / 2.0) * 10;
		//cout << v.x << endl;
		object body(xpo, ypo, ma, v);
		planets.push_back(body);

		//cout << i << endl;
	}
	v.x = 0;
	v.y = 0;
	object body(0, 0, 400, v);
	planets.push_back(body);
	cout << "done initializing data" << endl;

	return planets;
}

vector<object> makeBodyList2()
{
	short num = 4;
	float masses[] = { 10, 10, 10, 10};
	float dists[] = { 900, 1200, 1600, 2000};
	float sunMass = 400;
	bool moons = 1;
	float moonDist = 60;

	vector<object> planets;
	cord vel(0, 0);

	object sun(0,0, sunMass,vel);
	planets.push_back(sun);

	for (short i = 0; i < num; i++)
	{
		float upV = sqrt((G * sunMass) / dists[i]);
		cord v(0, ((i % 2 == 0) ? upV : -upV));
		object body((i % 2 == 0) ? dists[i] : -dists[i], 0, masses[i], v);
		planets.push_back(body);

		if (moons)
		{
			float upVV = sqrt((G * masses[i]) / moonDist);
			cord ve(0, ((i % 2 == 0) ? upVV + upV : -upVV - upV));
			object body((i % 2 == 0) ? dists[i] + moonDist : -dists[i] - moonDist, 0, 1, ve);
			planets.push_back(body);
		}
	}


	return planets;
}


void addAsteroids(vector<object> & bodies, short num, float lowerR, float upperR)
{
	float theta;
	float inTheta;
	float radius;
	float mass;
	float v;

	for (short i = 0; i < num; i++)
	{
		theta = (rand() % 628) / 100.0;
		inTheta = pi / 2 - theta;
		radius = ((rand() % (static_cast<long>(upperR - lowerR) * 100)) / 100.0) + lowerR;
		mass = (rand() % 100) / 500.0 + 0.01;
		v = sqrt((G * bodies[0].getMass()) / radius);

		cord vel(-v * cos(inTheta),v * sin(inTheta));

		object body(radius * cos(theta), radius * sin(theta),mass,vel);

		bodies.push_back(body);
	}
}




void compileAndShowScreen(Grid grid, Stats stats, vector<object> planets)
{
	Mat image(WIN_HEIGHT, WIN_WIDTH, CV_8UC3);

	grid.showGrid(image);
	showObjects(planets, image, grid);

	Mat info;
	info = stats.makeStatsImg(planets);

	Mat allCombined;
	hconcat(image, info, allCombined);



	imshow(WIN_NAME, allCombined);

	return;

}

void LeftMouseHandeler(vector<object> bodies, Grid grid, Stats & stats)
{
	if (mx < WIN_WIDTH)
	{
		float r;
		float dx;
		float dy;
		bool found = 0;
		for (short i = 0; i < bodies.size(); i++)
		{
			r = 10 * cbrt(bodies[i].getMass());

			dx = (grid.getCenter().x + (1.0 / grid.getPixPerMeter()) * (mx - (WIN_WIDTH / 2.0))) - bodies[i].getPos().x;
			dy = -(grid.getCenter().y + (1.0 / grid.getPixPerMeter()) * (my - (WIN_HEIGHT / 2.0))) - bodies[i].getPos().y;

			//cout << dx << " " << dy << " " << r << endl;

			if (r >= sqrt(pow(dx, 2) + pow(dy, 2)))
			{
				stats.setSBody(&bodies[i]);
				found = 1;
			}

			if (found)
			{
				stats.setSetting('s');
				stats.setSBody(&bodies[i]);
				//cout << "YESSSS" << endl;
				break;
			}

		}


	}
}


