/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <string>
#include <format>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define ENABLE_BUGGED_BEHAVIOUR true

int main ()
{
	#if(ENABLE_BUGGED_BEHAVIOUR == true)	
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	#else
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	#endif

	// Create the window and OpenGL context
	InitWindow(1280, 720, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	SetTextureFilter(wabbit, TEXTURE_FILTER_BILINEAR);
	GenTextureMipmaps(&wabbit);

	Vector2 wabbitStart = {10, 10};
	Vector2 wabbitEnd = {10, 132};
	Vector2 oldPosition = wabbitStart;
	double totalDistanceTraveled = 0;
	bool IsWindowResizedLatch = false;

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		int screenWidth = GetScreenWidth();
		int screenHeight = GetScreenHeight();
		Vector2 dpi = GetWindowScaleDPI();
		if(IsWindowResized())
		{
			IsWindowResizedLatch = true;
		}

		double currentTime = GetTime();
		double remainderTime = remainderf(currentTime, 2.0);
		double absoluteShiftedRemainder = abs(remainderTime);

		// Resolution Debug Info
		std::string msg = std::format("{}{:.3f}\n{}{:.3f}\n{}{:.3f}\n{}{:.3f}\n{}{:.3f},{:.3f}\n{}{},{}\n{}{}\n", 
			"Time: ", currentTime, 
			"Remainder: ", remainderTime, 
			"absShift: ", absoluteShiftedRemainder, 
			"totalWabbitDistanceTraveled: ", totalDistanceTraveled, 
			"dpi x,y: ", dpi.x, dpi.y,
			"screenWidth,screenHeight: ", screenWidth, screenHeight,
			"IsWindowResized(latched): ", (IsWindowResizedLatch ? "True(The text glitched offscreen)" : "False(The text remains)"));
		DrawText(msg.c_str(), 42, 10, 20, WHITE);
		
		// Vanishing text(anchored to top right corner)
		msg = "This text is going to vanish\nwhen you resize the window";
		DrawText(msg.c_str(), (screenWidth) - MeasureText(msg.c_str(), 20) - 10, 10, 20, WHITE);
	
		// update the wabbit
		Vector2 currentPosition = Vector2Lerp(wabbitStart, wabbitEnd, absoluteShiftedRemainder);
		totalDistanceTraveled = totalDistanceTraveled + (abs(Vector2Distance(currentPosition, oldPosition)));
		oldPosition = currentPosition;
		DrawTexture(wabbit, floorf(currentPosition.x), floorf(currentPosition.y), WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}