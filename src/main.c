#include <time.h>
#include "game.h"

int main(int argc, char** argv)
{
	srand(time(NULL));
	init_app(&argc, argv);
	glutMainLoop();
	return(EXIT_SUCCESS);
}
