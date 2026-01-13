#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace constants
{
    // Constants
   	constexpr int gScreenWidth   { 640 };
   	constexpr int gScreenHeight  { 480 };


   	// PATH TO RESOURCE FOLDER 'assets', 'gAssetsPath'
   	const std::string gAssetsPath { "./assets/" };


	// PATH'S TO ALL EXTERNAL RESOURCES using 'gAssetsPath'
	const std::string bg_str  { gAssetsPath + "images/bg.jpg" };
    const std::string sample_str { gAssetsPath + "sounds/sample.wav"};
	
	const std::string paddle_str { gAssetsPath + "images/paddle.png" };
	const std::string ball_str { gAssetsPath + "images/ball.png" };
	const std::string brick_red_str { gAssetsPath + "images/brick_red.png" };
	const std::string brick_orange_str { gAssetsPath + "images/brick_orange.png" };
	const std::string brick_yellow_str { gAssetsPath + "images/brick_yellow.png" };
	const std::string brick_green_str { gAssetsPath + "images/brick_green.png" };
	const std::string brick_blue_str { gAssetsPath + "images/brick_blue.png" };
	
	const std::string ballbounce_str { gAssetsPath + "sounds/BallBounce.wav" };
	const std::string destroybrick_str { gAssetsPath + "sounds/DestroyBrick.wav" };
	
	const std::string arial_font { gAssetsPath + "fonts/ARIAL.TTF" };

}

#endif