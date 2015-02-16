#include "Centipede.h"

Centipede::Centipede()
{
}

void Centipede::Update()
{
}

void Centipede::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Centipede::Destroy()
{
}

void Centipede::Initialize(float xpos, float ypos)
{
}

void Centipede::ConvertToHead()
{
}

void Centipede::ReadQueue()
{
	//creates a temporary place to store orders while the queue is being read
	orders temp_orders;

	temp_orders= order_queue.front();
	Pos.x = (temp_orders.x_offset);
	Pos.y = (temp_orders.y_offset);
	direction = temp_orders.direction;
	MainSprite.setRotation(temp_orders.rotation_degrees);
	order_queue.pop();
}