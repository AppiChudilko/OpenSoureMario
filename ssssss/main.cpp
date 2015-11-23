#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
float offsetX=0, offsetY=0;
float GameOver=false,NewGm=false,Win=false;

const int H = 51;
const int W = 150;

#include "Map\MapGame.h";

class PLAYER
{
	public:
	double dx,dy,currentFrame;
	FloatRect rect;
	bool onGround,life;
	Sprite sprite;
	PLAYER(Texture &image) //конструктор
	{
		sprite.setTexture(image);
		rect = FloatRect(100,180,16,16);
		dy=dx=0.1;
		currentFrame = 0;
	}
	void update(float time)
	{	
		rect.left += dx * time;	
		Collision(0); 
		if (!onGround) dy=dy+0.0005*time;	
		rect.top += dy*time;
		onGround=false;
		Collision(1);
		currentFrame += time * 0.005;
		if (currentFrame > 3) currentFrame -= 3; //Анимация
		if (dx>0) sprite.setTextureRect(IntRect(112+31*int(currentFrame),144,16,16));
		if (dx<0) sprite.setTextureRect(IntRect(112+31*int(currentFrame)+16,144,-16,16));
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);  
		dx=0;
	}
	void Collision(int num)
	{	
		for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++)
			for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
			{
				if ((TileMap[i][j]=='P') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t'))
				{ 
					if (dy>0 && num==1) { rect.top =   i*16 -  rect.height;  dy=0;   onGround=true; }
					if (dy<0 && num==1) { rect.top = i*16 + 16;   dy=0; }
					if (dx>0 && num==0) { rect.left =  j*16 -  rect.width; }
					if (dx<0 && num==0) { rect.left =  j*16 +16;}
				}
				if (TileMap[i][j]=='c') TileMap[i][j]=' '; 
				if (TileMap[i][j]=='x') GameOver=true;
				if (TileMap[i][j]=='2') {
					rect.left=100; rect.top=364;
					sprite.setPosition(100, 364);  
					offsetX = rect.left-100; 
					offsetY = rect.top-90; 
					std::cout<<rect.left<<"   "<< rect.top<<std::endl;
				}
				if (TileMap[i][j]=='3') {
					rect.left=100; rect.top=634;
					sprite.setPosition(100, 634);  
					offsetX = rect.left-100; 
					offsetY = rect.top-90; 
					std::cout<<rect.left<<"   "<< rect.top<<std::endl;
				}
				if (TileMap[i][j]=='4') {
					Win=true; GameOver=true;
				}
			}
	}
};

class ENEMY
{
	public:
	double dx,dy,currentFrame;
	FloatRect rect;
	bool onGround,life;
	Sprite sprite;
	void set(Texture &image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x,y,16,16);
		dx=0.05;
		currentFrame = 0;
		life=true;
	}
	void update(float time)
	{	
		rect.left += dx * time;
		Collision();
		currentFrame += time * 0.005;
		if (currentFrame > 2) currentFrame -= 2;
		sprite.setTextureRect(IntRect(18*int(currentFrame),   0, 16,16));
		if (!life) sprite.setTextureRect(IntRect(58, 0, 16,16));
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}
	void Collision()
	{
		for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++)
			for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
				if ((TileMap[i][j]=='P') || (TileMap[i][j]=='0')) { 
					if (dx>0) { rect.left =  j*16 - rect.width; dx*=-1; }
					else if (dx<0) { rect.left =  j*16 + 16;  dx*=-1; }
											
				}
	}
};

int main()
{  
    RenderWindow window(VideoMode(800, 250), "Mario v0.1 Alpha");
	Texture tileSet;
	tileSet.loadFromFile("Mario_Tileset.png");

	//===========================
	PLAYER Mario(tileSet);
	//==========[Bot]============
	ENEMY  enemy[3];
	enemy[0].set(tileSet,48*16,13*16);//1lvl
	enemy[1].set(tileSet,110*16,13*16);
	enemy[2].set(tileSet,48*16,30*16);//2lvl
	/*enemy[3].set(tileSet,110*16,13*16);
	enemy[4].set(tileSet,48*16,13*16);//3lvl
	enemy[5].set(tileSet,110*16,13*16);*/
	//===========================
	Sprite tile(tileSet);
	SoundBuffer buffer;
	//==========[Звук]============
	buffer.loadFromFile("Jump.ogg");
	Sound sound(buffer);
	//==========[Шрифты]==========
	sf::Font font;
	font.loadFromFile("arial.ttf");
	//==========[Меню]============
	Texture menu_texture1,menu_texture2,menu_texture3,about_texture;
	menu_texture1.loadFromFile("111.png");
	menu_texture2.loadFromFile("222.png");
    menu_texture3.loadFromFile("333.png");
	about_texture.loadFromFile("about.png");
	Sprite menu1(menu_texture1),menu2(menu_texture2),menu3(menu_texture3),about(about_texture);
	if(NewGm==true) {
		NewGame:
		GameOver=false;
		Mario.rect.left=100; Mario.rect.top=180;
		Mario.sprite.setPosition(100, 180);  
		offsetX = Mario.rect.left-100; 
		enemy[0].life=true; enemy[1].life=true;
		offsetY = Mario.rect.top-180; 
		std::cout<<Mario.rect.left<<"   "<< Mario.rect.top<<std::endl;
		NewGm=false;
	}

	bool Menu=1;
	int MenuNum=0;
	menu1.setPosition(100,30);
	menu2.setPosition(100,90);
    menu3.setPosition(100,150);

	while(Menu)
	{	   
	   menu1.setColor(Color::White);
	   menu2.setColor(Color::White);
	   menu3.setColor(Color::White);
	   MenuNum=0;
	   window.clear(Color(0,0,0));

	   if (IntRect(100,30,300,50).contains(Mouse::getPosition(window))) {menu1.setColor(Color::Yellow); MenuNum=1;}
       if (IntRect(100,90,300,50).contains(Mouse::getPosition(window))) {menu2.setColor(Color::Yellow); MenuNum=2;}
       if (IntRect(100,150,300,50).contains(Mouse::getPosition(window))) {menu3.setColor(Color::Yellow); MenuNum=3;}

	   if (Mouse::isButtonPressed(Mouse::Left))
	     {
			 if (MenuNum==1) Menu=false;
			 if (MenuNum==2) {window.draw(about); window.display(); while(!Keyboard::isKeyPressed(Keyboard::Escape)) ;}
			 if (MenuNum==3)  {window.close();Menu=false;}

	     }
		window.draw(menu1);
        window.draw(menu2);
		window.draw(menu3);
		window.display();
	}
	//==========[Врубаем музыку в игру]============
	Music music;
    music.openFromFile("Mario_Theme.ogg");
    music.play();
	//==================[Игра]=====================
	Clock clock;

    while (GameOver==false)
    { 
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time/500;
		if (time > 20) time = 20; 

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)      
                window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) Mario.dx=-0.1; 
	    if (Keyboard::isKeyPressed(Keyboard::Right)) Mario.dx=0.1;
	    if (Keyboard::isKeyPressed(Keyboard::Up)) if (Mario.onGround) { Mario.dy=-0.27; Mario.onGround=false;  sound.play(); }
	 
		 Mario.update(time);
		 enemy[0].update(time);
		 enemy[1].update(time);
		 enemy[2].update(time);

		 if  (Mario.rect.intersects(enemy[0].rect))
		 {
			 if (enemy[0].life) {
				 if (Mario.dy>0) { enemy[0].dx=0; Mario.dy=-0.2; enemy[0].life=false;}
				 else { Mario.dy=-0.27; GameOver=true; }
			 }
		 }
		 if  (Mario.rect.intersects(enemy[1].rect))
		 {
			 if (enemy[1].life) {
				 if (Mario.dy>0) { enemy[1].dx=0; Mario.dy=-0.2; enemy[1].life=false;}
				 else { Mario.dy=-0.27; GameOver=true; }
			 }
		 }
		 if  (Mario.rect.intersects(enemy[2].rect))
		 {
			 if (enemy[2].life) {
				 if (Mario.dy>0) { enemy[2].dx=0; Mario.dy=-0.2; enemy[2].life=false;}
				 else { Mario.dy=-0.27; GameOver=true; }
			 }
		 }
		 if (Mario.rect.left>200) offsetX = Mario.rect.left-200;
		 window.clear(Color(107,140,255));
		 for (int i=0; i<H; i++)
			 for (int j=0; j<W; j++)
			 { 
				switch(TileMap[i][j]) {
					case 'P':  tile.setTextureRect( IntRect(143-16*3,112,16,16)); break; 
					case 'k': tile.setTextureRect( IntRect(143,112,16,16)); break; 
					case 'c': tile.setTextureRect( IntRect(143-16,112,16,16)); break; 
					case 't': tile.setTextureRect( IntRect(0,47,32,95-47)); break; 
					case 'm': tile.setTextureRect( IntRect(96,6,202-47,111)); break; 
					case 'g': tile.setTextureRect( IntRect(0,16*9-5,3*16,16*2+5)); break; 
					case 'G': tile.setTextureRect( IntRect(145,222,222-145,255-222)); break; 
					case 'd': tile.setTextureRect( IntRect(0,106,74,127-106)); break; 
					case 'w': tile.setTextureRect( IntRect(99,224,140-99,255-224)); break; 
					case 'r': tile.setTextureRect( IntRect(143-32,112,16,16)); break; 
					default: continue;
				}

  			    tile.setPosition(j*16-offsetX,i*16 - offsetY) ; 
		        window.draw(tile);
			}
		window.draw(Mario.sprite);
	    window.draw(enemy[0].sprite);
		window.draw(enemy[1].sprite);
		window.draw(enemy[2].sprite);
		window.display();
    }
	while(Win)
	{	
		GameOver=false;
		window.clear(Color(0,0,0));
		sf::Text text("You Win! :3", font); 
		text.setPosition(100,30);
		text.setCharacterSize(30);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Yellow);
		window.draw(text);	
		sf::Text text1("Press ESC", font); 
		text1.setPosition(100,60);
		text1.setCharacterSize(30);
		text1.setStyle(sf::Text::Bold);
		text1.setColor(sf::Color::Red);
		window.draw(text1);	
		window.display();
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { goto NewGame; NewGm=true; }
	}
	while(GameOver)
	{	   
		window.clear(Color(0,0,0));
		sf::Text text("Game Over :c", font); 
		text.setPosition(100,30);
		text.setCharacterSize(30);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Red);
		window.draw(text);	
		sf::Text text1("Press ESC", font); 
		text1.setPosition(100,60);
		text1.setCharacterSize(30);
		text1.setStyle(sf::Text::Bold);
		text1.setColor(sf::Color::Red);
		window.draw(text1);	
		window.display();
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { goto NewGame; NewGm=true; }
	}
    return 0;
}



