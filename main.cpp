#include <GL/glut.h>				
#include <math.h>
#include <random>
#include <iostream>
#include <queue>
#include <time.h>
#include <mmsystem.h>
#include <string>
using namespace std;

double x_boat = 0;
float angle = 0.0f, boatWidth = 270;
bool initPosition = true;
unsigned int score = 0;
time_t start;
int spawnRate = 1;
float speed = 0.5;
float boatHeight = 125;
float boatHitbox = 200;

#define EXIT_ON_ERROR -1

void drawCircleFill(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;

	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + cx, y + cy);//output vertex 

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

void drawStar(float baseX, float baseY, float scale) {
	glBegin(GL_POLYGON);
	glVertex2f(baseX, baseY);
	glVertex2f(baseX - 3 * scale, baseY - 4 * scale);
	glVertex2f(baseX, baseY - 3 * scale);
	glVertex2f(baseX + 3 * scale, baseY - 4 * scale);
	glVertex2f(baseX + 2 * scale, baseY - 1 * scale);
	glVertex2f(baseX + 4 * scale, baseY);
	glVertex2f(baseX + 2 * scale, baseY + 1 * scale);
	glVertex2f(baseX, baseY + 4 * scale);
	glVertex2f(baseX - 2 * scale, baseY + 1 * scale);
	glVertex2f(baseX - 4 * scale, baseY);
	glVertex2f(baseX - 2 * scale, baseY - 1 * scale);
	glVertex2f(baseX - 3 * scale, baseY - 4 * scale);
	glEnd();
}

void drawX(float centerX, float centerY, float scale) {
	float baseX = centerX, baseY = centerY;
	glBegin(GL_POLYGON);
	glVertex2f(baseX, baseY);
	glVertex2f(baseX - 1 * scale, baseY - 2 * scale);
	glVertex2f(baseX, baseY - 1 * scale);
	glVertex2f(baseX + 1 * scale, baseY - 2 * scale);
	glVertex2f(baseX + 2 * scale, baseY - 1 * scale);
	glVertex2f(baseX + 1 * scale, baseY);
	glVertex2f(baseX + 2 * scale, baseY + 1 * scale);
	glVertex2f(baseX + 1 * scale, baseY + 2 * scale);
	glVertex2f(baseX, baseY + 1 * scale);
	glVertex2f(baseX - 1 * scale, baseY + 2 * scale);
	glVertex2f(baseX - 2 * scale, baseY + 1 * scale);
	glVertex2f(baseX - 1 * scale, baseY);
	glVertex2f(baseX - 2 * scale, baseY - 1 * scale);
	glVertex2f(baseX - 1 * scale, baseY - 2 * scale);
	glEnd();
}

void drawBomb(float x, float y) {

	y -= 25 / 2;
	// draw the bomb fuse
	// color brown
	glColor3f(0.5, 0.25, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - 5, y + 20);
	glVertex2f(x + 5, y + 20);
	glVertex2f(x + 5, y + 35);
	glVertex2f(x + 10, y + 45);
	glVertex2f(x + 10, y + 55);
	glVertex2f(x - 5, y + 35);
	glEnd();

	// fuse flash
	// color red
	float baseX = x + 10;
	float baseY = y + 50;
	float scale = 3;
	glColor3f(1, 0, 0);
	drawStar(baseX, baseY, scale);
	// color yellow
	scale = 2;
	glColor3f(1, 1, 0);
	drawStar(baseX, baseY, scale);

	// draw the bomb body
	glColor3f(0, 0, 0);
	drawCircleFill(x, y, 25, 360);

}

void drawCoin(int posX, int posY) {
	float coinSize = 25;

	// draw coin body
	glColor3f(1.0f, 0.843f, 0.0f);
	drawCircleFill(posX, posY, coinSize, 360);
	glColor3f(1.0f, 1, 0.0f);
	drawCircleFill(posX, posY, coinSize * 0.75, 360);
	glColor3f(1.0f, 0.843f, 0.0f);
	drawStar(posX, posY, 3.5);
}

void drawChest(float x, float y) {
	float chestSize = 50;
	y -= chestSize / 2;
	// semi-circle lid of the chest
	glColor3f(0.4, 0.15, 0);
	drawCircleFill(x, y + chestSize / 2, chestSize, 360);
	glColor3f(0.5, 0.25, 0); // Set color to brown
	drawCircleFill(x, y + chestSize / 2, chestSize * 0.9, 360);

	// body border
	glColor3f(0.4, 0.15, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - chestSize, y - chestSize / 2); // Bottom left corner
	glVertex2f(x + chestSize, y - chestSize / 2); // Bottom right corner
	glVertex2f(x + chestSize, y + chestSize / 2); // Top right corner
	glVertex2f(x - chestSize, y + chestSize / 2); // Top left corner
	glEnd();
	// body
	glColor3f(0.5, 0.25, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - chestSize * 0.9, y - chestSize / 2 * 0.8); // Bottom left corner
	glVertex2f(x + chestSize * 0.9, y - chestSize / 2 * 0.8); // Bottom right corner
	glVertex2f(x + chestSize * 0.9, y + chestSize / 2 * 0.8); // Top right corner
	glVertex2f(x - chestSize * 0.9, y + chestSize / 2 * 0.8); // Top left corner
	glEnd();

	// drawing keyhole
	float keyholeSize = chestSize / 5;
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(x - keyholeSize, y + chestSize / 2 * 1.2); // Bottom left corner
	glVertex2f(x + keyholeSize, y + chestSize / 2 * 1.2); // Bottom right corner
	glVertex2f(x + keyholeSize, y + chestSize / 2 * 0.6); // Top right corner
	glVertex2f(x - keyholeSize, y + chestSize / 2 * 0.6); // Top left corner
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x - keyholeSize / 4, y + chestSize / 2); // Bottom left corner
	glVertex2f(x + keyholeSize / 4, y + chestSize / 2); // Bottom right corner
	glVertex2f(x + keyholeSize / 4, y + chestSize / 2 * 0.7); // Top right corner
	glVertex2f(x - keyholeSize / 4, y + chestSize / 2 * 0.7); // Top left corner
	glEnd();
	drawCircleFill(x, y + chestSize / 2, keyholeSize / 3, 360);

}

void drawBottle(int x, int y) {
	float bottleSize = 15;
	y -= bottleSize * 3;

	//color brown
	glColor3f(0.5, 0.25, 0);

	// Draw the bottle
	glBegin(GL_QUADS);
	glVertex2f(x - bottleSize, y);
	glVertex2f(x + bottleSize, y);
	glVertex2f(x + bottleSize, y + bottleSize * 4);
	glVertex2f(x - bottleSize, y + bottleSize * 4);
	glEnd();
	// Draw label
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(x - bottleSize, y + bottleSize * 1);
	glVertex2f(x + bottleSize, y + bottleSize * 1);
	glVertex2f(x + bottleSize, y + bottleSize * 3);
	glVertex2f(x - bottleSize, y + bottleSize * 3);
	glEnd();

	float scale = 2;
	glColor3f(1, 1, 1);
	drawX(x, y + bottleSize * 2, scale);
	drawX(x + bottleSize / 1.5, y + bottleSize * 2, scale);
	drawX(x - bottleSize / 1.5, y + bottleSize * 2, scale);

	// drawing bottle neck
	glColor3f(0.5, 0.25, 0);
	drawCircleFill(x, y + bottleSize * 4, bottleSize, 360);
	glBegin(GL_QUADS);
	glVertex2f(x - bottleSize / 3, y + bottleSize * 4);
	glVertex2f(x + bottleSize / 3, y + bottleSize * 4);
	glVertex2f(x + bottleSize / 3, y + bottleSize * 6);
	glVertex2f(x - bottleSize / 3, y + bottleSize * 6);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(x - bottleSize / 2.5, y + bottleSize * 6);
	glVertex2f(x + bottleSize / 2.5, y + bottleSize * 6);
	glVertex2f(x + bottleSize / 2.5, y + bottleSize * 6.5);
	glVertex2f(x - bottleSize / 2.5, y + bottleSize * 6.5);
	glEnd();
}

class BaseItem {
public:
	float x;
	float y;
	virtual void drawItem() {};
	virtual void droppedItem() {};

	BaseItem(float x, float y) : x(x), y(y) {}
};

class Coin : public BaseItem {
public:
	void drawItem() {
		drawCoin(x, y);
		y -= speed;
	}
	void droppedItem() {
		score += 10;
		cout << "You got a COIN! +10 Points" << endl;
		cout << "Current score: " << score << endl;
		cout << endl;
	}
	Coin(float x, float y) : BaseItem(x, y) {};
};

class Chest : public BaseItem {
public:
	void drawItem() {
		drawChest(x, y);
		y -= speed;
	}
	void droppedItem() {
		score += 50;
		cout << "You got a CHEST! +50 Points" << endl;
		cout << "Current score: " << score << endl;
		cout << endl;
	}
	Chest(float x, float y) : BaseItem(x, y) {};
};

class Bottle : public BaseItem {
public:
	void drawItem() {
		drawBottle(x, y);
		y -= speed;
	}
	void droppedItem() {
		score += 100;
		cout << "You got a BOTTLE! +100 Points" << endl;
		cout << "Current score: " << score << endl;
		cout << endl;
	}
	Bottle(float x, float y) : BaseItem(x, y) {};
};

class Bomb : public BaseItem {
public:
	void drawItem() {
		drawBomb(x, y);
		y -= speed;
	}
	void droppedItem() {
		cout << "**********************************" << endl;
		cout << "GAME OVER!" << "\nYOU HAVE BEEN BOMBED!" << endl;
		cout << "Final score: " << score << endl;
		cout << "**********************************" << endl;
		exit(EXIT_SUCCESS);
	}
	Bomb(float x, float y) : BaseItem(x, y) {};
};

std::queue<BaseItem*> items;

enum Item
{
	bottle,
	chest,
	coin,
	bomb,
	NO_ITEMS
};


int getIntRandomNumber(const int min, const int max)
{
	if (!(min < max))
		throw std::invalid_argument("parameters must respect the following condition: min<max!");

	std::random_device randomDevice;
	std::mt19937 mt19937Generator(randomDevice());
	std::uniform_int_distribution<int> unifromIntDistribution(min, max);

	return unifromIntDistribution(mt19937Generator);
}

Item getRandomItem()
{
	const int min = 0;
	const int max = static_cast<int>(Item{ NO_ITEMS }) - 1;
	const int randomInteger = getIntRandomNumber(min, max);
	Item randomItem{ static_cast<Item>(randomInteger) };

	return randomItem;

}

void createTree(int tree_base_x, int tree_base_y, int island_radius)
{

	float trunkWidth = island_radius / 10; // Width of the trunk
	float trunkHeight = island_radius / 1.5; // Height of the trunk
	float offset_x = tree_base_x, offset_y = tree_base_y + trunkHeight;

	int leaves_radius = island_radius / 2;
	int top_trunk_x = offset_x, top_trunk_y = offset_y + leaves_radius;
	int leaf_offset = leaves_radius / 2;
	int shadow_offset = leaves_radius / 5;

	// --------Top Left Leaf--------
	glColor3f(0.0, 0.6, 0.0);// leaf color
	drawCircleFill(top_trunk_x - leaves_radius / 2, top_trunk_y + leaf_offset, leaves_radius, 360);
	glColor3f(0.0, 0.5, 0.0); //dark leaf
	drawCircleFill(top_trunk_x - leaves_radius / 2 - leaf_offset + shadow_offset, top_trunk_y + shadow_offset, leaves_radius * 0.9, 360);
	glColor3f(0.001f, 0.53f, 0.98f); //blue
	drawCircleFill(top_trunk_x - leaves_radius / 2 - leaf_offset, top_trunk_y, leaves_radius, 360);

	// --------Top Right Leaf--------
	glColor3f(0.0, 0.6, 0.0);// leaf color
	drawCircleFill(top_trunk_x + leaves_radius / 2, top_trunk_y + leaf_offset, leaves_radius, 360);
	glColor3f(0.0, 0.5, 0.0); //dark leaf
	drawCircleFill(top_trunk_x + leaves_radius / 2 + leaf_offset - shadow_offset, top_trunk_y + shadow_offset, leaves_radius * 0.9, 360);
	glColor3f(0.001f, 0.53f, 0.98f); //blue
	drawCircleFill(top_trunk_x + leaves_radius / 2 + leaf_offset, top_trunk_y, leaves_radius, 360);

	// --------Bottom Left Leaf--------
	glColor3f(0.0, 0.6, 0.0);// leaf color
	drawCircleFill(top_trunk_x - leaves_radius / 2, top_trunk_y, leaves_radius, 360);
	// --------Bottom Right Leaf--------
	glColor3f(0.0, 0.6, 0.0);// leaf color
	drawCircleFill(top_trunk_x + leaves_radius / 2, top_trunk_y, leaves_radius, 360);

	// Shadows
	glColor3f(0.0, 0.5, 0.0); //dark leaf
	drawCircleFill(top_trunk_x - leaves_radius / 2 - leaf_offset + shadow_offset, top_trunk_y - leaf_offset + shadow_offset, leaves_radius * 0.9, 360);
	glColor3f(0.0, 0.5, 0.0); //dark leaf
	drawCircleFill(top_trunk_x + leaves_radius / 2 + leaf_offset - shadow_offset, top_trunk_y - leaf_offset + shadow_offset, leaves_radius * 0.9, 360);

	glColor3f(0.001f, 0.53f, 0.98f); //blue
	drawCircleFill(top_trunk_x + leaves_radius / 2 + leaf_offset, top_trunk_y - leaf_offset, leaves_radius, 360);
	glColor3f(0.001f, 0.53f, 0.98f); //blue
	drawCircleFill(top_trunk_x - leaves_radius / 2 - leaf_offset, top_trunk_y - leaf_offset, leaves_radius, 360);

	// --------Trunk--------
	glColor3f(0.6, 0.4, 0.2); // Set color to brown
	glBegin(GL_QUADS);        // Start drawing a filled quadrilateral

	glVertex2f(-trunkWidth + tree_base_x, -trunkHeight + offset_y); // Bottom-left vertex
	glVertex2f(trunkWidth + tree_base_x, -trunkHeight + offset_y);  // Bottom-right vertex
	glVertex2f(trunkWidth + tree_base_x, trunkHeight + offset_y);   // Top-right vertex
	glVertex2f(-trunkWidth + tree_base_x, trunkHeight + offset_y);  // Top-left vertex

	glEnd(); // End drawing the quadrilateral
}

void createIsland(int island_x, int island_y, int island_radius)
{
	createTree(island_x, island_y + island_radius - 10, island_radius);
	glColor3f(0.8f, 0.6f, 0.2f);//sand color
	drawCircleFill(island_x, island_y, island_radius, 360);
	glColor3f(0.001f, 0.53f, 0.98f); //blue
	drawCircleFill(island_x, island_y - island_radius * 1.7, island_radius * 2, 360);
}

void drawScene(void)
{
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	float aspectRatio = (float)screenWidth / (float)screenHeight;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// desenam cerul
	glColor3f(0.53, 0.74, 0.96);
	glBegin(GL_POLYGON);
	glVertex2i(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	glVertex2i(0, glutGet(GLUT_SCREEN_HEIGHT));
	glVertex2i(0, glutGet(GLUT_SCREEN_HEIGHT) * 0.7916);
	glVertex2i(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT) * 0.7916);
	glEnd();

	// desenam soarele
	glColor3f(1.0, 1.0, 0.0);
	drawCircleFill(glutGet(GLUT_SCREEN_WIDTH) * 0.93, glutGet(GLUT_SCREEN_HEIGHT) * 0.916, 50, 360);

	// desenam insulele
	int island_x = glutGet(GLUT_SCREEN_WIDTH) * 0.067, island_y = glutGet(GLUT_SCREEN_HEIGHT) * 0.62, island_radius = 50;
	createIsland(island_x, island_y, island_radius);
	island_x = glutGet(GLUT_SCREEN_WIDTH) * 0.33, island_y = glutGet(GLUT_SCREEN_HEIGHT) * 0.43, island_radius = 100;
	createIsland(island_x, island_y, island_radius);
	island_x = glutGet(GLUT_SCREEN_WIDTH) * 0.67, island_y = glutGet(GLUT_SCREEN_HEIGHT) * 0.54, island_radius = 60;
	createIsland(island_x, island_y, island_radius);
	island_x = glutGet(GLUT_SCREEN_WIDTH) * 0.87, island_y = glutGet(GLUT_SCREEN_HEIGHT) * 0.4, island_radius = 80;
	createIsland(island_x, island_y, island_radius);

	//Items falling
	if (time(0) - start == spawnRate) {
		start += spawnRate;
		switch (getRandomItem())
		{
		case 0:
			items.push(new Bottle(getIntRandomNumber(200, screenWidth - 200), screenHeight + 100));
			break;
		case 1:
			items.push(new Chest(getIntRandomNumber(200, screenWidth - 200), screenHeight + 100));
			break;
		case 2:
			items.push(new Coin(getIntRandomNumber(200, screenWidth - 200), screenHeight + 100));
			break;
		case 3:
			items.push(new Bomb(getIntRandomNumber(200, screenWidth - 200), screenHeight + 100));
			break;
		default:
			cout << "Got random number out of bounds!";
			break;
		}
	}
	std::queue<BaseItem*> temp;
	while (items.size() > 0) {
		Bottle* bottle = dynamic_cast<Bottle*>(items.front());
		Chest* chest = dynamic_cast<Chest*>(items.front());
		Coin* coin = dynamic_cast<Coin*>(items.front());
		Bomb* bomb = dynamic_cast<Bomb*>(items.front());
		if (bottle != NULL)
			bottle->drawItem();
		if (chest != NULL)
			chest->drawItem();
		if (coin != NULL)
			coin->drawItem();
		if (bomb != NULL)
			bomb->drawItem();
		if (items.front()->y < boatHeight && abs(x_boat - items.front()->x) < boatHitbox) {
			if (bottle != NULL)
				bottle->droppedItem();
			if (chest != NULL)
				chest->droppedItem();
			if (coin != NULL)
				coin->droppedItem();
			if (bomb != NULL)
				bomb->droppedItem();
		}
		else if (items.front()->y > 0)
			temp.push(items.front());
		items.pop();
	}
	while (temp.size()) {
		items.push(temp.front());
		temp.pop();
	}

	//Show score on screen
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2f(10.0, screenHeight * 0.95);
	std::string text = "Score: ";
	text.append(std::to_string(score));
	for (int i = 0; i < text.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);

	// denenam corabia de pirati
	glPushMatrix();

	if (initPosition) {
		glTranslated(glutGet(GLUT_SCREEN_WIDTH) / 2, 0.0, 0.0);
	}
	else {
		glTranslated(x_boat, 0.0, 0.0);
	}

	glRotatef(angle, 0.0, 1.0, 0.0);

	glColor3f(0.251, 0.184, 0.114);
	glBegin(GL_POLYGON);
	glVertex2f(-85, 130);
	glVertex2f(-30, 130);
	glVertex2f(-22, 120);
	glVertex2f(-25, 70);
	glVertex2f(-55, 70);
	glVertex2f(-85, 110);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-30, 130);
	glVertex2f(-135, 130);
	glVertex2f(-135, 140);
	glVertex2f(-40, 140);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-25, 120);
	glVertex2f(45, 120);
	glVertex2f(45, 70);
	glVertex2f(-25, 70);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(45, 120);
	glVertex2f(70, 155);
	glVertex2f(135, 155);
	glVertex2f(135, 110);
	glVertex2f(105, 70);
	glVertex2f(45, 70);
	glEnd();

	//DUNGI ->
	glColor3f(0.2, 0.15, 0.1);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(-85, 125);
	glVertex2f(-26, 125);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(49, 125);
	glVertex2f(135, 125);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(59, 140);
	glVertex2f(135, 140);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(-85, 110);
	glVertex2f(135, 110);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(-74, 95);
	glVertex2f(124, 95);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(-63, 80);
	glVertex2f(113, 80);
	glEnd();

	//DUNGI <-
	glColor3f(0.33, 0.33, 0.34);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(15, 120);
	glVertex2f(15, 245);
	glEnd();

	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(-45, 140);
	glVertex2f(-45, 200);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(95, 155);
	glVertex2f(95, 185);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(15, 220);
	glVertex2f(15, 180);
	glVertex2f(-15, 180);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(15, 175);
	glVertex2f(15, 135);
	glVertex2f(-25, 135);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-45, 195);
	glVertex2f(-45, 150);
	glVertex2f(-75, 150);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(95, 165);
	glVertex2f(95, 185);
	glVertex2f(120, 185);
	glVertex2f(120, 165);
	glEnd();

	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(100, 170);
	glVertex2f(115, 180);
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(100, 180);
	glVertex2f(115, 170);
	glEnd();

	glColor3f(0.251, 0.184, 0.114);
	glBegin(GL_POLYGON);
	glVertex2f(5, 240);
	glVertex2f(25, 240);
	glVertex2f(20, 220);
	glVertex2f(10, 220);
	glEnd();

	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void init(void)
{
	// initial desenam pe tot ecranul marea
	// iar mai apoi anumite parti le coloram conform obiectelor corespunzatoare
	glClearColor(0.001, 0.53, 0.98, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, glutGet(GLUT_SCREEN_WIDTH), 0.0, glutGet(GLUT_SCREEN_HEIGHT), -1.0, 1.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void mouseMovement(int x_cursor, int y_cursor) {
	initPosition = false;

	if (x_cursor > x_boat) {
		angle = 180;
	}
	else if (x_cursor < x_boat) {
		angle = 0;
	}

	if (x_cursor >= boatWidth / 2) {
		if (x_cursor + boatWidth / 2 <= glutGet(GLUT_SCREEN_WIDTH))
			x_boat = x_cursor;
		else
			x_boat = glutGet(GLUT_SCREEN_WIDTH) - boatWidth / 2;
	}
	else {
		x_boat = boatWidth / 2;
	}

	glutPostRedisplay();
}

void exitGame(unsigned char key, int x, int y) {
	if (key == 27) {
		cout << "\n**********************************" << endl;
		cout << "Game ended by user" << endl;
		cout << "Final score: " << score << endl;
		cout << "**********************************" << endl;
		exit(EXIT_SUCCESS);
	}


	glutPostRedisplay();
}

void playMusic() {
	const wchar_t* path = L"D:\\FACULTATE\\ANUL 3\\SEM 2\\GRAFICA\\Proiect 2D\\pirate.wav";
	PlaySound(path, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

int main(int argc, char** argv) {
	playMusic();
	start = time(0);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bay of Pirates");
	init();
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(mouseMovement);
	glutKeyboardFunc(exitGame);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}