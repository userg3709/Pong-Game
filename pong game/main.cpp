#include "raylib.h"
#include <iostream>
#include "raymath.h"
#include<ctime>
class ball {
private:
	Vector2 bpos;
	Vector2 obpos;
	Vector2 V;
	Vector2 OV;
	bool Hit = false;
	int r;
public:
	ball(Vector2 p):bpos(Vector2Scale(p,0.5f)),obpos(Vector2Scale(p, 0.5f)){
		V = randD(V);
		OV = randD(V);

		
	}
	bool getHit() {
		return Hit;
	}
	void sethit(bool status) {
		Hit = status;
	 }
	Vector2 randD(Vector2& v) {
		v=Vector2Scale( Vector2Normalize({ (float)GetRandomValue(-500,500) / 100.0f,(float)GetRandomValue(-100,100) / 100.0f }),5.0f);
		return v;
	}
	Vector2 getpos() {
		return bpos;
	}
	Vector2 getV() {
		return V;
	}
	void setr(int R) {
		r = R;
	}
	void reflectV(Vector2 d) {
		V = Vector2Reflect(V, d);
		if(V.x<=10&&V.y<=10)
		V = Vector2Scale(V, 1.05f);
		if (Vector2Length(V) > 20.0f)
			V = Vector2Scale(Vector2Normalize(V), 20.0f);
		if (Vector2Length(V) < 5.0f)
			V = Vector2Scale(Vector2Normalize(V), 5.0f);
	}
	void setV(Vector2 pv) {
		V = Vector2Add(V, Vector2Scale(pv,0.5f));

	}
	int getr() {
		return r;
	}
	void reset() {
		bpos = obpos;
		V = randD(V);
	}
	void setbpos(Vector2 v) {
		bpos = Vector2Add(bpos, v);
	}
	void moveball(ball& b1) {
		b1.setbpos(b1.getV());
	};
};
class paddle {
private:
	Vector2 ppos;
	Vector2 oppos;
	Vector2 pVelocity;
	int width, height;
	bool isleft;
public:
	paddle(Vector2 pos,int s) :ppos(pos),width(pos.x/40),height(pos.y/6),isleft(s) {
		ppos.x = isleft ? pos.x / 10 : pos.x - pos.x / 10 - width;
		ppos.y = (pos.y-height)/2;
		oppos = ppos;
	}
	Vector2 getpppos() {
		return ppos;
	}
	void setpVelocity(Vector2 d) {
		pVelocity = d;
	}
	Vector2 getpVelocity() {
		return pVelocity;
	}
	void setpppos(Vector2 k) {
		 ppos=Vector2Add(ppos,k);
	}
	int getwidth() {
		return width;
	}
	int getheight(){
		return height;
	}
	void reset() {
		ppos=oppos;
	}
};
class GameEngine {
private:
	ball b1;
	paddle p1;
	paddle p2;
	 char Player[32];

	int score1 = 0, score2 = 0;
	int swidth, sheight;
public:
	bool gameStarted = false;
	bool gameEnded = false;
	GameEngine(Vector2 d, const char* c) :b1(d), p1(d, 1), p2(d, 0), swidth(d.x), sheight(d.y) { strncpy_s(Player, c, 31);
	Player[31] = '\0';
	}
	void drawpaddle(Texture2D P1, Texture2D P2) {
		DrawTexture(P1, p1.getpppos().x, p1.getpppos().y, WHITE);
		DrawTexture(P2, p2.getpppos().x, p2.getpppos().y, WHITE);
		
	}
	void DrawIntroScreen() {
		const char* title = "PONG CLASSIC";
		const char* instructions = "Press SPACE to Start";

		int titleFontSize = swidth / 15;
		int instrFontSize = swidth / 40;

		int titleWidth = MeasureText(title, titleFontSize);
		int instrWidth = MeasureText(instructions, instrFontSize);

		DrawText(title, (swidth - titleWidth) / 2, sheight / 3, titleFontSize, YELLOW);
		DrawText(instructions, (swidth - instrWidth) / 2, sheight * 2 / 3, instrFontSize, WHITE);
	}
	void drawresult() {
		if (score1 >= 10)
		{
			const char* title = TextFormat("%s WINS", Player);
			const char* instructions = "Press Enter to Restart";

			int titleFontSize = swidth / 15;
			int instrFontSize = swidth / 40;

			int titleWidth = MeasureText(title, titleFontSize);
			int instrWidth = MeasureText(instructions, instrFontSize);

			DrawText(title, (swidth - titleWidth) / 2, sheight / 3, titleFontSize, YELLOW);
			DrawText(instructions, (swidth - instrWidth) / 2, sheight * 2 / 3, instrFontSize, WHITE);
		}
		else if (score2 >= 10)
		{
			const char* title = TextFormat("%s Loses", Player);
			const char* instructions = "Press Enter to Start";

			int titleFontSize = swidth / 15;
			int instrFontSize = swidth / 40;

			int titleWidth = MeasureText(title, titleFontSize);
			int instrWidth = MeasureText(instructions, instrFontSize);

			DrawText(title, (swidth - titleWidth) / 2, sheight / 3, titleFontSize, YELLOW);
			DrawText(instructions, (swidth - instrWidth) / 2, sheight * 2 / 3, instrFontSize, WHITE);
		}

	}
	
	void collision() {
		if (b1.getpos().x - b1.getr() - 2 > p1.getpppos().x + p1.getwidth() || b1.getpos().x + b1.getr() + 2 < p2.getpppos().x)
			b1.sethit(false);
		if (b1.getpos().y + b1.getr() >= sheight-40)
		{
			Vector2 normal;
			normal = { 0,1 };
			b1.reflectV(normal);
		}
		if( b1.getpos().y - b1.getr() <= 30)
		{
			Vector2 normal;
			normal = { 0,-1 };
			b1.reflectV(normal);

		}
		else if ((!b1.getHit())&&b1.getpos().x - b1.getr() <= p1.getpppos().x + p1.getwidth() &&
			b1.getpos().x - b1.getr() >= p1.getpppos().x &&
			b1.getpos().y >= p1.getpppos().y &&
			b1.getpos().y <= p1.getpppos().y + p1.getheight())
		{
			Vector2 normal;
			normal = { 1,0 };
			b1.reflectV(normal);
			b1.setV(p1.getpVelocity());
			b1.sethit(true);
		}
		else if ((!b1.getHit()) && b1.getpos().x + b1.getr() <= p2.getpppos().x + p2.getwidth() &&
			b1.getpos().x + b1.getr() >= p2.getpppos().x &&
			b1.getpos().y >= p2.getpppos().y &&
			b1.getpos().y <= p2.getpppos().y + p2.getheight())
		{
			Vector2 normal;
			normal = { -1,0 };
			b1.reflectV(normal);
			b1.setV(p2.getpVelocity());
			b1.sethit(true);
		}
		


	}
	void drawball(Texture2D Ball){
		b1.setr(((swidth + sheight) / 200));
		DrawTexture(Ball, b1.getpos().x, b1.getpos().y, WHITE);
		b1.moveball(b1);
	}
	void movepadddle() {
		p1.setpVelocity({ 0, 0 });
		p2.setpVelocity({ 0, 0 });

		Vector2 up = { 0,-p1.getheight()/30 };
		Vector2 down = { 0,p1.getheight() / 30 };
		if (IsKeyDown(KEY_W))
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				up = { 0,(float) - p1.getheight() / 15};
				p1.setpVelocity(up);
				if (p1.getpppos().y >= 0)
					p1.setpppos(up);
			}
			else
			{
				
				up = { 0,-(float)p1.getheight() / 30 };
				p1.setpVelocity(up);
				if (p1.getpppos().y >= 0)
					p1.setpppos(up);
			}
		if (IsKeyDown(KEY_S))
			if (IsKeyDown(KEY_LEFT_SHIFT)) 
			{	
				down = { 0,(float)p1.getheight() / 15 };
				p1.setpVelocity(down);
				if (p1.getpppos().y + p1.getheight() <= sheight)
					p1.setpppos(down);
			}
			else
			{
				down = { 0,(float)p1.getheight() / 30 };
				p1.setpVelocity(down);
				if (p1.getpppos().y + p1.getheight() <= sheight)
					p1.setpppos(down);
			}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				down = { 0,(float)p2.getheight() / 15 };
				p2.setpVelocity(down);
				if (p2.getpppos().y + p2.getheight() <= sheight)
					p2.setpppos(down);
			}
			else
			{
				down = { 0,(float)p2.getheight() / 30 };
				p2.setpVelocity(down);
				if (p2.getpppos().y + p2.getheight() <= sheight)
					p2.setpppos(down);
			}
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				up = { 0,(float)-p2.getheight() / 15 };
				p2.setpVelocity(up);
				if (p2.getpppos().y >= 0)
					p2.setpppos(up);
			}
			else
			{
				up = { 0,-(float)p2.getheight() / 30 };
				p2.setpVelocity(up);
				if (p2.getpppos().y >= 0)
					p2.setpppos(up);
			}

		
		Vector2 p2pos = p2.getpppos();
		float paddleCenterY = p2pos.y + p2.getheight() / 2.0f;
		float ballY = b1.getpos().y;

		
		float aiSpeed = p2.getheight() / 20.0f; 

		Vector2 aiVelocity = { 0, 0 };
		if (ballY < paddleCenterY - 10)
			aiVelocity = { 0, -aiSpeed };
		else if (ballY > paddleCenterY + 10)
			aiVelocity = { 0, aiSpeed };

		
		if ((aiVelocity.y < 0 && p2pos.y > 0) ||
			(aiVelocity.y > 0 && p2pos.y + p2.getheight() < sheight)) {
			p2.setpppos(aiVelocity);
			p2.setpVelocity(aiVelocity);
		}


	}
	void Score() {
		if (b1.getpos().x <= 0)
		{
			score2++;
			b1.reset();
			p1.reset();
			p2.reset();
		}
		else if (b1.getpos().x >= swidth)
		{
			score1++;
			b1.reset();
			p1.reset();
			p2.reset();
		}
		if ( strlen(Player) == 0)
			strcpy_s(Player, "PLAYER");
		const char* P1 = TextFormat("%s:%d", Player, score1);
		const char* P2 = TextFormat("COMPUTER: %d",score2);
		int fontsize = swidth / 40;
		int textwidth1 = MeasureText(P1, fontsize);
		int textwidth2 = MeasureText(P2, fontsize);
		DrawText(P1, (swidth / 4) - (textwidth1 / 2), sheight / 40, fontsize, GREEN);
		DrawText(P2, (swidth * 3 / 4) - (textwidth2 / 2), sheight / 40, fontsize, GREEN);
		if (score2 == 10)
			gameEnded = true;
		else if (score1 == 10)
			gameEnded = true;
		

		
	}
	void reset()
	{
		if (IsKeyPressed(KEY_ENTER))
		{
			b1.reset();
			p1.reset();
			p2.reset();
			score2 = 0;
			score1 = 0;
			gameEnded = false;
		}
	}

};


int main()
{
	srand(time(NULL));
	Vector2 d = { 1920,1080 };
	char playerName[32] = "Player";
	int letterCount = 0;
	bool nameEntered = false;
	

	
	
	InitWindow(d.x,d.y, "raylib - Drag Circle with Mouse");
	Texture2D board = LoadTexture("textures/Board.jpg");
	Texture2D paddle1 = LoadTexture("textures/paddle 1.jpg");
	Texture2D paddle2 = LoadTexture("textures/paddle2.jpg");
	Texture2D Ball = LoadTexture("textures/ball.png");
	
	
	GameEngine* g = nullptr;
	ToggleFullscreen();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		if (!nameEntered) {
			int key = GetCharPressed();

			while (key > 0)
			{
				if ((key >= 32) && (key <= 125) && (letterCount < 31)) 
				{
					playerName[letterCount] = (char)key;
					letterCount++;
					playerName[letterCount] = '\0'; 
				}
				key = GetCharPressed();
			}

			
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				letterCount--;
				if (letterCount < 0) letterCount = 0;
				playerName[letterCount] = '\0';
			}
		}

		
		if (IsKeyPressed(KEY_ENTER) && letterCount > 0)
		{
			nameEntered = true;
			g = new GameEngine(d, playerName);
		}
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Enter your name:", 100, 100, 30, WHITE);
		DrawRectangle(100, 150, 400, 50, DARKGRAY);
		DrawText(playerName, 110, 160, 30, RAYWHITE);

		if ((letterCount < 31) && ((GetTime() - (int)GetTime()) < 0.5))
			DrawText("_", 110 + MeasureText(playerName, 30), 160, 30, RAYWHITE);


		if (g && !g->gameStarted && nameEntered) {
			g->DrawIntroScreen();
			if (IsKeyPressed(KEY_SPACE)) {
				g->gameStarted = true;
			}
		}
		else if (g && g->gameStarted && nameEntered && !g->gameEnded) {
			DrawTexture(board, 0, 0, WHITE);


			g->Score();
			g->drawpaddle(paddle1, paddle2);
			g->drawball(Ball);
		}
		else if (g&&g->gameEnded)
			g->drawresult();

		EndDrawing();
		

		if (g&&g->gameStarted) {
			g->collision();
			g->movepadddle();
			g->reset();
		}
	}
	UnloadTexture(board);

	CloseWindow();
	return 0;
}
