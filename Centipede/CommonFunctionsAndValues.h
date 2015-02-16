#ifndef _CommonFunctionsAndValues
#define _CommonFunctionsAndValues

//WINDOW VALUES//
const int window_width= 480; //Width of window in pixels
const int window_height= 512; //Height of window in pixels
///////////////

//BLASTER VALUES//
const float blaster_SPEED= 5; //Blaster speed
const float y_upperbound= 404; //Defines the highest y area the blaster can travel, in pixels
const float	y_lowerbound= 488; //Defines the lowest y area the blaster can travel, in pixels 
const float start_x= 240; //Starting x position
const float start_y= 488; //Starting y position 
const float sprite_bumper= 8; //"bumper" should be half of sprite width
//////////////////

//BULLET VALUES//
const float bullet_SPEED= 10; //Bullet speed
//////////////////

//SPAWNING GRID VALUES//
const int spawn_chance= 5; //Spawn chance (out of 100)
const float pixelvalue= 16; //# of horizontal and vertical pixels per grid space (changed from x_pixelvalue and y_pixelvalue to keep grid spaces in square ratio)
const float y_offset= 1; //# of horizontal lines to skip (from the top) before the first row
const int columns= 30; //# of rows to spawn - SHOULD NOT EXCEED 30. 
const int rows= 29; //# of columns to spawn - SHOULD NOT EXCEED 30. 
const int mushroom_center= 8; //the "center" (not origin) of the current mushroom sprite, in pixels (e.g. center of a 16x16 sprite is 8)
const int turn_offset= 16; //the position at which the centipede ought to "turn" before it hits a mushroom
const int player_boundary_int= 23; //the grid position equivalent of y_upperbound (see Blaster Values)
//////////////////

//MUSHROOM VALUES//
const int mushroom_health= 4; //Number of hits from a Bullet that a Mushroom can take before it is destroyed
///////////////

///CENTIPEDE VALUES///
const float centipede_SPEED= 2; //Centipede initial speed. PLEASE, FOR YOUR SAKE, MAKE THIS NUMBER FIT NICELY WITH 16 (2,4,8,16) 
const float centipede_leftbound= 0; //Left boundary for centipede
const float centipede_rightbound= 480;//Right boundary for centipede
const int body_points= 10; //point value of body
const int head_points= 100; //point value of head
///////////////

///FLEA VALUES///
const float flea_SPEED= 4; //Flea initial speed.
const float mushroom_chance= 5; //Chance of a flea trying to spawn a mushroom as he descends.
const float y_initial= -8; //Initial y position of flea (x is randomized).
const int flea_points= 200; //point value of flea
///////////////

///SCORPION VALUES///
const float scorpion_SPEED= 2; //Scorpion initial speed
const int scorpion_points= 1000; //point value of scorpion
///////////////

///SPIDER VALUES///
const float spider_SPEED= 2;//Spider initial speed
const float spider_leftpos= -8; //X value for where spider spawns if he does so on the left side of the screen.
const float spider_rightpos= 488; //X value for where spider spawns if he does so on the right side of the screen.
const float spider_spawnheight= 312; //Y value for where spider spawns.
const float spider_upperbound= 312; //Y value for highest the spider will travel.
const float spider_lowerbound= 488; //Y value for lowest the spider will travel.
const int spider_minlength= 9; //minimum queue length for spider
const int spider_maxlength= 32; //maximum queue length for spider
const float spider_300_distance= 128; //distance between the spider and the player (or beyond) where 300 points are awarded
const float spider_900_distance= 64; //distance between the spider and the player (or closer) where 900 points are awarded
///////////////

///PLAYER MANAGER VALUES///
const int initial_lives= 2; //initial number of lives for a player
///////////////

///WAVE MANAGER VALUES///
const int mushroom_limit= 5; //If the amount of mushrooms in the player area is below this number, a flea will be created.
const int spider_timer_min= 5; //Min time it can take a spider to spawn (in seconds)
const int spider_timer_max = 10; //Max time it can take a spider to spawn (in seconds)
const int scorpion_timer_min= 15; //Min time it can take a scorpion to spawn (in seconds)
const int scorpion_timer_max= 30; //Max time it can take a scorpion to spawn (in seconds)
const int flea_scorpion_wave= 1; //Any wave equal to or lower than this number will not have fleas or scorpions
///////////////

///HUD MANAGER VALUES///
const int max_digits= 6; //maximum digits for a score
const float p1_score_position= 0;  //the leftmost x position which the p1 score will appear at (right aligned)
const float p2_score_position= 384; //the leftmost x position which the p2 score will appear at (right aligned)
const float hs_score_position= 192; //the leftmost x position which the high score will appear at (right aligned)
const float p1_lives_position= 96; //the x position at which player 1's lives will appear
const float p2_lives_position= 368; //the x position at which player 2s lives will appear
const float high_score_position_x= 176; //the x position of where to begin writing "high score" on the title screen
const float high_score_position_y= 48; //the y position of where to begin writing "high score" on the title screen
const float hs1_position_x= 192; //the x position of where to begin writing the first high score on the title screen
const float hs1_position_y= 64; //the y position of where to begin writing the first high score on the title screen
const float credits_2_position_x= 176; //the x position of where to begin writing "credits 2" on the title screen
const float credits_2_position_y= 224; //the y position of where to begin writing "credits 2" on the title screen
const float bonus_every_12000_position_x= 128; //the x position of where to begin writing "bonus after 12000" on the title screen
const float bonus_every_12000_position_y= 240; //the y position of where to begin writing "bonus after 12000" on the title screen
///////////////

///AI BLASTER CONTROLLER VALUES//
const int ai_minlength= 12; //min length for ai queues
const int ai_maxlength= 40; //max length for ai queues

///GAME OVER DISPLAY VALUES///
const float gameover_x= 176; //x value for "GAME OVER" text
const float gameover_y= 256; //y value for "GAME OVER" text
const float playertext_x= 176; //x value for "PLAYER _" text
const float playertext_y= 272; //y value for "PLAYER _" text

///HIGH SCORE ENTRY VALUES///
const float greatscore_x= 144; //x value for "GREAT SCORE P_" text
const float greatscore_y= 304; //y value for "GREAT SCORE P_" text
const float typeyourinitials_x= 96; //x value for "TYPE YOUR INITIALS" text
const float typeyourinitials_y= 320; //y value for "TYPE YOUR INITIALS" text
const float input_x= 192; //x value for initials text
const float input_y= 352; //y value for initials text

///SCORE VALUES///
const int oneup_points= 12000; //amount of points needed to earn a 1up

#endif