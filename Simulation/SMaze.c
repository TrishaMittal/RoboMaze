#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <libplayerc/playerc.h>

// Random numbers generated for random walk movement by robots
double randomInt(int low, int high)
	{
	return low + (high - low) * (rand()/(RAND_MAX * 1.0 ));
 	}

// Main function for the program
int main(int argc, const char **argv)
	{
 	int i;
	playerc_client_t *client;
	playerc_position2d_t *position2d;
	playerc_laser_t *laser;
	int cycle, index=0,sum=177;
	double dist,angle,fidAngle = 0,lineAngle=0, fidDist=0, prevYaw=0,posAngle=0;
	// Create a client and connect it to the server.
	client = playerc_client_create(NULL, "localhost", 6665);
	if (0 != playerc_client_connect(client))
		return -1;
	// Create and subscribe to a position2d device.
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
		return -1;
	playerc_position2d_enable(position2d,1);
	laser = playerc_laser_create(client,0);
	if (playerc_laser_subscribe(laser, PLAYERC_OPEN_MODE) != 0)
		{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
      		}

	// Initiating linear velocities
	if (0 != playerc_position2d_set_cmd_vel(position2d, randomInt(1,3), randomInt(1,3), DTOR(randomInt(-20,20)),1))
		return -1;
	fprintf(stdout, "robot random positions \n");
	int count = 0;
	float x,y;
	
	while(1)
		{
		// Wait for new data from server
		playerc_client_read(client);
		fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",
		position2d->px, position2d->py, position2d->pa);
		printf("Left%f\n",laser->min_left);
		printf("Right%f\n",laser->min_right);
	
		if (0 != playerc_position2d_set_cmd_vel(position2d,randomInt(0.1,1), randomInt(0.1,1), DTOR(randomInt(-20,20)),1))
			return -1;
		if((laser->min_left <= 1 ))
			{
			playerc_client_read(client);
			for(i=0;i<3;i++)
	                	if (0 != playerc_position2d_set_cmd_vel(position2d, -2.5, -2.5, DTOR(-95) ,1))
	                        	return -1;                      
                		usleep(1000);
                	}
		if((laser->min_right <= 1))
			{	
			playerc_client_read(client);
			fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
			for(i=0;i<3;i++)
				if (0 != playerc_position2d_set_cmd_vel(position2d, -2.5,-2.5,DTOR(90) ,1))
					return -1;
			usleep(1000);
			}     
 	         usleep(1000);
                 }

playerc_position2d_unsubscribe(position2d);
playerc_position2d_destroy(position2d);
playerc_client_disconnect(client);
playerc_client_destroy(client);
return 0;

}
