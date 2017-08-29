#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <libplayerc/playerc.h>

enum bumper {left = 0, right};

playerc_client_t *client;
playerc_position2d_t *position2d;
playerc_bumper_t *bumper;

//Irobot turns right

int turn_right(playerc_client_t *client,playerc_position2d_t *position2d)
	{
	playerc_client_read(client);
    	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
    	playerc_position2d_set_cmd_vel(position2d, 0,0 , -1*DTOR(46),1);
    	sleep(1);
    	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
    	sleep(1);	
	}

//Irobot turns left
int turn_left(playerc_client_t *client,playerc_position2d_t *position2d)
	{
	// Wait for new data from server
	playerc_client_read(client);
    	fprintf(stdout, "X: %3.2f, Y: %3.2f, Yaw: %3.2f \n",position2d->px, position2d->py, position2d->pa);
	playerc_position2d_set_cmd_vel(position2d, 0,0 , DTOR(47),1);
	sleep(1);
	playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
	sleep(1);
	}

// Main function for the program
int main(int argc, const char **argv)
	{
	int i;

	// Create a client and connect it to the server.
	client = playerc_client_create(NULL, "localhost", 6665);
	if (0 != playerc_client_connect(client))
		return -1;
	
	// Create and subscribe to a position2d device.
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
		return -1;

	// Create and subscribe to a bumper device.
  	bumper = playerc_bumper_create(client,0);
	if (playerc_bumper_subscribe(bumper, PLAYER_OPEN_MODE))
		{
	  	printf("Subscribing to bumper failed\n");
		return -1;
		}
	else
		printf("Subscribing to bumber successfull\n");

	
	// Initialising the linear velocities
	if (0 != playerc_position2d_set_cmd_vel(position2d,5,0,0,1))
		return -1;
	fprintf(stdout, "*********** ROBOMAZE **********\n");

	
	//Robomaze : Start ---> End
	while(1)
		{
		playerc_client_read(client);
		if (0 != playerc_position2d_set_cmd_vel(position2d, 5, 0, 0 ,1))
			return -1;

		//To update the values of the bumper
		playerc_bumper_get_geom(bumper);
		printf("%d %d %d %d\n",(int)bumper->bumpers[0],(int)bumper->bumpers[1],bumper->bumper_count,bumper->poses);	
		
		//Turns right, if both the bumpers get triggered 
		if (((int)bumper->bumpers[1]==1) && (int)bumper->bumpers[0]==1)
			turn_right(client,position2d);
		
		//Turns left, if right bumper gets triggered 
		else if(((int)bumper->bumpers[1]==1))
			turn_left(client,position2d);
	
		printf("%d %d %d %d\n",(int)bumper->bumpers[0],(int)bumper->bumpers[1],bumper->bumper_count,bumper->poses);	
		}

	//Unsubscribing and destroying devices
	playerc_bumper_unsubscribe(bumper);
	playerc_bumper_destroy(bumper);
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);
	return 0;
	}
	
