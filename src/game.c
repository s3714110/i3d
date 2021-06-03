
#include "game.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int screen_width = 0;
int screen_height = 0;
int arena_width = 0;
int arena_height = 0;
float last_time = 0.0;
float arena_start_x = 0;
float arena_start_y = 0;
float arena_end_x = 0;
float arena_end_y = 0;
float arena_scale = 0;
bool reposition_ship = true;

float last_ship_particle_time = 0;
float last_ship_particle_pending = 0;
float last_ship_particle_count = 0;

bool close_warning_top = false;
bool close_warning_bottom = false;
bool close_warning_left = false;
bool close_warning_right = false;
float arena_r = DEFAULT_ARENA_R;
float arena_g = DEFAULT_ARENA_G;
float arena_b = DEFAULT_ARENA_B;

float asteroid_circle_radius = 0;
int asteroid_count = 0;
int last_increment = 0;
float time_last_wave = 0;
float time_wave_pending = 0;

float time_last_shot = 0;
float time_shot_pending = 0;

float player_time = 0;
int player_score = 0;
bool first_start = true;
bool a_key_is_pressed_start = false;
bool a_key_is_pressed_end = false;
bool quit_message = false;


struct ship first_ship;
struct key keys[256];
struct mouse left_mouse_button;
struct vec2d asteroid_launch_point;
struct vec2d arena_center;
struct asteroid asteroids[MAX_ASTEROIDS * 2];
struct asteroid_particle asteroids_particles[MAX_ASTEROIDS_PARTICLES];
struct bullet bullets[MAX_BULLETS];
struct ship_particle ship_particles[MAX_SHIP_PARTICLES];



void on_key_press(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case MOVE_FORWARD_KEY:
		(&keys[MOVE_FORWARD_KEY])->pressed = true;
		a_key_is_pressed_start = true;
		quit_message = false;
		break;
	case ROTATE_LEFT_KEY:
		(&keys[ROTATE_LEFT_KEY])->pressed = true;
		a_key_is_pressed_start = true;
		quit_message = false;
		break;
	case ROTATE_RIGHT_KEY:
		(&keys[ROTATE_RIGHT_KEY])->pressed = true;
		a_key_is_pressed_start = true;
		quit_message = false;
		break;
	case KEY_ESC:
		exit(EXIT_SUCCESS);
		break;
	default:
		a_key_is_pressed_start = true;
		quit_message = false;
		break;
	}
}

void on_key_release(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case MOVE_FORWARD_KEY:
		(&keys[MOVE_FORWARD_KEY])->pressed = false;
		break;
	case ROTATE_LEFT_KEY:
		(&keys[ROTATE_LEFT_KEY])->pressed = false;
		break;
	case ROTATE_RIGHT_KEY:
		(&keys[ROTATE_RIGHT_KEY])->pressed = false;
		break;
	case KEY_ESC:
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void on_mouse_button(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			(&left_mouse_button)->pressed = true;
		}
		else if (state == GLUT_UP) {
			(&left_mouse_button)->pressed = false;
		}
	}
}

void reset_asteroid(struct asteroid* ast)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		float velocity_random = rand() / (float)RAND_MAX;
		float circle_random = rand() / (float)RAND_MAX;
		float rotate_random = rand() / (float)RAND_MAX;
		float radius_random = rand() / (float)RAND_MAX;

		ast[i].velocity = ast[i].velmin + velocity_random * (ast[i].velmax - ast[i].velmin);
		ast[i].circle_lines = 6 + circle_random * (32 - 6);
		ast[i].random_angles[ast[i].circle_lines];
		for (int j = 0; j < ast[i].circle_lines; j++)
		{
			float rand_scale = rand() / (float)RAND_MAX;
			float random_number = 0 + rand_scale * 1;
			ast[i].random_angles[j] = random_number;
		}

		ast[i].rotate_speed = ast[i].rotate_min + rotate_random * (ast[i].rotate_max - ast[i].rotate_min);

		int dir_array[2] = { -1,1 };
		int random_dir = rand() % 2;
		ast[i].rotate_dir = dir_array[random_dir];
		ast[i].radius = ast[i].radius_min + radius_random * (ast[i].radius_max - ast[i].radius_min);

		ast[i].rc = 0;

		float hitpoint_f = 1 + HIT_POINT_MULTIPLIER * ast[i].radius;
		ast[i].hitpoint = (int)round(hitpoint_f);

		ast[i].alive = false;
		ast[i].launched = false;
		ast[i].inside = false;
		ast[i].split = false;
		ast[i].exploded = false;
	}

}

void reset_wave_time()
{
	time_last_shot = 0;
	time_last_wave = 0;
	last_increment = 0;
	asteroid_count = 0;

}

void ship_reshape(struct ship* ship)
{
	ship->size = arena_width * arena_height / (2 * 120 * 120);

	float start_pos_x = ((screen_width / 2) - (arena_width / 2) + MARGIN_ADJ) + ship->size;
	float start_pos_y = ((screen_height / 2) - (arena_height / 2) + MARGIN_ADJ) + ship->size;
	ship->pos.x = start_pos_x;
	ship->pos.y = start_pos_y;
	ship->rc = 315;

	arena_scale = ship->size;
	ship->alive = true;

}

void on_reshape(int w, int h)
{
	fprintf(stderr, "on_reshape(%d, %d)\n", w, h);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -100, 100);

	screen_width = w;
	screen_height = h;
}


void draw_arena_line(float colr, float colg, float colb, float x_start, float y_start, float x_end, float y_end, bool warning)
{

	if (warning) {
		glColor3f(1, 0, 0);
	}
	else
	{
		glColor3f(colr, colg, colb);
	}
	glBegin(GL_LINES);
	glVertex2f(x_start, y_start);
	glVertex2f(x_end, y_end);
	glEnd();

}

void draw_arena(float colr, float colg, float colb, float width, float height) {

	if (colr < 0 || colr > 1 || colg < 0 || colg > 1 || colb < 0 || colb > 1)
	{
		colr = colg = colb = 1; //default color is white if rgb value is invalid	
	}

	if (width < MARGIN_ADJ || width > screen_width) {
		width = screen_width;
	}

	if (height < MARGIN_ADJ || height > screen_height) {
		height = screen_height;
	}
	//make arena at the center of the screen
	float x_start = (screen_width / 2) - (width / 2) + MARGIN_ADJ;
	float y_start = (screen_height / 2) - (height / 2) + MARGIN_ADJ;
	float x_end = (screen_width / 2) + (width / 2) - MARGIN_ADJ;
	float y_end = (screen_height / 2) + (height / 2) - MARGIN_ADJ;

	if ((arena_start_x == 0 && arena_start_y == 0) || arena_start_x != x_start || arena_start_y != y_start) {
		reposition_ship = true;
	}


	draw_arena_line(colr, colg, colb, x_start, y_start, x_end, y_start, close_warning_bottom);
	draw_arena_line(colr, colg, colb, x_end, y_start, x_end, y_end, close_warning_right);
	draw_arena_line(colr, colg, colb, x_end, y_end, x_start, y_end, close_warning_top);
	draw_arena_line(colr, colg, colb, x_start, y_end, x_start, y_start, close_warning_left);


	arena_width = width;
	arena_height = height;

	arena_start_x = x_start;
	arena_start_y = y_start;
	arena_end_x = x_end;
	arena_end_y = y_end;


}

void draw_ship(struct ship* ship)
{
	if (ship->alive == true) {


		glPushMatrix();
		glColor3f(ship->fillr, ship->fillg, ship->fillb);
		glTranslatef(ship->pos.x, ship->pos.y, 0);
		glRotatef(ship->rc, 0, 0, 1);
		glScalef(ship->size, ship->size, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.f, -0.3f);
		glVertex2f(0.f, sqrtf(0.5f));
		glEnd();
		glPopMatrix();


		glPushMatrix();
		glColor3f(ship->fillr, ship->fillg, ship->fillb);
		glTranslatef(ship->pos.x, ship->pos.y, 0);
		glRotatef(ship->rc, 0, 0, 1);
		glScalef(ship->size, ship->size, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.f, -0.3f);
		glVertex2f(0.f, sqrtf(0.5f));
		glEnd();
		glPopMatrix();


		glPushMatrix();
		glColor3f(ship->outr, ship->outg, ship->outb);
		glTranslatef(ship->pos.x, ship->pos.y, 0);
		glRotatef(ship->rc, 0, 0, 1);
		glScalef(ship->size, ship->size, 0);
		glBegin(GL_LINES);
		glVertex2f(0.f, -0.3f);
		glVertex2f(0.f, sqrtf(0.5f));
		glEnd();
		glPopMatrix();


		glPushMatrix();
		glColor3f(ship->outr, ship->outg, ship->outb);
		glTranslatef(ship->pos.x, ship->pos.y, 0);
		glRotatef(ship->rc, 0, 0, 1);
		glScalef(ship->size, ship->size, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.f, -0.3f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.f, sqrtf(0.5f));
		glEnd();
		glPopMatrix();

	}

}

void draw_ship_circle(struct ship* ship, int n)
{
	float r = ship->radius;

	float x, y;
	float theta;

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(ship->pos.x, ship->pos.y, 0);
	glRotatef(ship->rc, 0, 0, 1);
	glScalef(ship->size, ship->size, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		theta = i / (float)n * 2.0 * M_PI;
		x = r * cosf(theta);
		y = r * sinf(theta);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
}

void draw_ship_particles(struct ship* ship, struct ship_particle* particles)
{
	if (ship->alive) {

		for (int i = 0; i < NUMBER_PARTICLES; i++)
		{
			if (particles[i].moving == true)
			{
				glPushMatrix();
				glColor3f(1.0, 1.0, 0.0);
				glPointSize(particles[i].starting_size);
				glTranslatef(particles[i].pos.x, particles[i].pos.y, 0);
				glRotatef(ship->rc, 0, 0, 1);
				glScalef(ship->size, ship->size, 0);
				glBegin(GL_POINTS);
				glVertex2f(0, 0);
				glEnd();
				glPopMatrix();
			}
		}

	}

}

void draw_asteroid_launch_circle(int n)
{
	struct vec2d center;
	center.x = arena_start_x + arena_width / 2;
	center.y = arena_start_y + arena_height / 2;

	float radius = 0.5 * sqrtf(powf(arena_width, 2) + powf(arena_height, 2));
	float x, y;
	float theta;

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(center.x, center.y, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		theta = i / (float)n * 2.0 * M_PI;
		x = radius * cosf(theta);
		y = radius * sinf(theta);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();

	arena_center = center;
	asteroid_circle_radius = radius;
}

void draw_asteroid_launch_point(struct asteroid* ast)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].alive == true)
		{
			glPushMatrix();
			glColor3f(0.0, 1.0, 0.0);
			glPointSize(10);
			glTranslatef(arena_start_x + arena_width / 2, arena_start_y + arena_height / 2, 0);
			glBegin(GL_POINTS);
			glVertex2f(ast[i].launch_point.x, ast[i].launch_point.y);
			glEnd();
			glPopMatrix();
		}
	}

}

void draw_asteroids(struct asteroid* ast)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		float x, y;
		float theta;

		if (ast[i].alive == true) {
			glPushMatrix();

			glColor3f(ast[i].r, ast[i].g, ast[i].b);
			glTranslatef(ast[i].pos.x, ast[i].pos.y, 0);
			glRotatef(ast[i].rc, 0, 0, 1);
			glScalef(arena_scale, arena_scale, 0);
			glBegin(GL_POLYGON);
			for (int j = 0; j < ast[i].circle_lines; j++) {
				theta = j / (float)ast[i].circle_lines * 2.0 * M_PI + ast[i].random_angles[j];
				x = ast[i].radius * cosf(theta);
				y = ast[i].radius * sinf(theta);
				glVertex2f(x, y);
			}
			glEnd();
			glPopMatrix();
		}
	}


}

void draw_bullets(struct bullet* bl, struct ship* ship)
{
	if (ship->alive == true) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bl[i].fired == true && bl[i].alive == true) {
				glPushMatrix();
				glColor3f(bl[i].r, bl[i].g, bl[i].b);
				glPointSize(bl[i].size);
				glTranslatef(bl[i].pos.x, bl[i].pos.y, 0);
				glRotatef(bl[i].ship_rc, 0, 0, 1);
				glScalef(arena_scale, arena_scale, 0);
				glBegin(GL_POINTS);
				glVertex2f(0.f, 0.f);
				glEnd();
				glPopMatrix();
			}
		}
	}
}

void draw_player_stat()
{
	int time = (int)player_time;
	int minutes = time / 60;
	int seconds = time % 60;

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(MARGIN_ADJ + 9, screen_height - 15 - MARGIN_ADJ); //define position on the screen

	char string_score[100] = { 0 };
	snprintf(string_score, 100, "Player score: %d", player_score);
	char* ch_score;
	for (ch_score = string_score; *ch_score; ch_score++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)*ch_score);
	}

	char string_time[100] = { 0 };
	snprintf(string_time, 100, "Time: %d minutes %d seconds", minutes, seconds);
	char* ch_time;
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(screen_width - 9 * 27, screen_height - 15); //define position on the screen
	for (ch_time = string_time; *ch_time; ch_time++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)*ch_time);
	}
}

void draw_welcome()
{
	glColor3f(1.0, 0.0, 1.0);
	glRasterPos2f(screen_width / 2, screen_height / 2);

	char string[100] = { 0 };
	snprintf(string, 100, "Welcome! Press any key to start");
	char* ch;
	glColor3f(1.0, 0.0, 1.0);
	glRasterPos2f(screen_width / 2 - 31 * sqrt(24), screen_height / 2 - sqrt(24)); //define position on the screen
	for (ch = string; *ch; ch++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)*ch);
	}

}

void draw_gameover()
{

	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(screen_width / 2, screen_height / 2, -100);

	char string[100] = { 0 };
	snprintf(string, 100, "Game Over. Press any key to play again...");
	char* ch;
	glColor3f(1.0, 0.0, 1.0);
	glRasterPos2f(screen_width / 2 - 31 * sqrt(41), screen_height / 2 - sqrt(24)); //define position on the screen
	for (ch = string; *ch; ch++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)*ch);
	}


}

void draw_asteroid_particles(struct asteroid_particle* particles)
{
	for (int i = 0; i < MAX_ASTEROIDS_PARTICLES; i++) {
		if (particles[i].in_use == true)
		{
			glPushMatrix();
			glColor3f(particles[i].r, particles[i].g, particles[i].b);
			glPointSize(particles[i].size);
			glTranslatef(particles[i].pos.x, particles[i].pos.y, 0);
			glScalef(arena_scale, arena_scale, 0);
			glBegin(GL_POINTS);
			glVertex2f(0.f, 0.f);
			glEnd();
			glPopMatrix();

		}
	}
}

void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (a_key_is_pressed_start == false) {
		draw_welcome();
	}
	else {
		if (quit_message == true)
		{
			draw_gameover();
		}

		draw_arena(arena_r, arena_g, arena_b, ARENA_WIDTH, ARENA_HEIGHT);

		draw_ship(&first_ship);

		//draw_ship_circle(&first_ship, SHIP_CIRCLE_LINE);

		draw_asteroid_launch_circle(ASTEROID_LAUNCH_CIRCLE_LINE);

		//draw_asteroid_launch_point(asteroids);

		draw_asteroids(asteroids);

		draw_bullets(bullets, &first_ship);

		draw_ship_particles(&first_ship, ship_particles);

		draw_player_stat();

		draw_asteroid_particles(asteroids_particles);

	}
	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("error: %s\n", gluErrorString(err));

	glutSwapBuffers();
}

void reset_player_stat() {
	player_score = 0;
	player_time = 0;
}

void check_wall(struct ship* ship, float warn_dist) {
	float scaled_radius = ship->radius * ship->size;

	if ((arena_start_x + warn_dist) >= (ship->pos.x - scaled_radius))
	{
		close_warning_left = true;
	}
	else
	{
		close_warning_left = false;
	}

	if ((arena_end_x - warn_dist) <= (ship->pos.x + scaled_radius))
	{
		close_warning_right = true;
	}
	else
	{
		close_warning_right = false;
	}

	if ((arena_start_y + warn_dist) >= (ship->pos.y - scaled_radius))
	{
		close_warning_bottom = true;
	}
	else
	{
		close_warning_bottom = false;
	}

	if ((arena_end_y - warn_dist) <= (ship->pos.y + scaled_radius))
	{
		close_warning_top = true;
	}
	else
	{
		close_warning_top = false;
	}

	if ((arena_start_x >= (ship->pos.x - scaled_radius))
		|| (arena_end_x <= (ship->pos.x + scaled_radius)))
	{
		reposition_ship = true;
		quit_message = true;
		ship->alive = false;
	}

	if ((arena_start_y >= (ship->pos.y - scaled_radius))
		|| (arena_end_y <= (ship->pos.y + scaled_radius)))
	{
		reposition_ship = true;
		quit_message = true;
		ship->alive = false;
	}



}


void update_ship_state(struct ship* ship, float dt)
{

	if ((&keys[ROTATE_LEFT_KEY])->pressed && !(&keys[ROTATE_RIGHT_KEY])->pressed)
	{
		ship->rc += ship->rotate_angle * dt;
	}

	if ((&keys[ROTATE_RIGHT_KEY])->pressed && !(&keys[ROTATE_LEFT_KEY])->pressed)
	{
		ship->rc -= ship->rotate_angle * dt;
	}

	if ((&keys[ROTATE_LEFT_KEY])->pressed && (&keys[ROTATE_RIGHT_KEY])->pressed)
	{
		ship->rc += 0;
	}


	if ((&keys[MOVE_FORWARD_KEY])->pressed)
	{

		float v = ship->last_velocity + ship->acceleration * dt;
		if (v < ship->velocity) {
			ship->pos.x += v * dt * cosf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->pos.y += v * dt * sinf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->last_velocity = v;
		}
		else {
			ship->pos.x += ship->velocity * dt * cosf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->pos.y += ship->velocity * dt * sinf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->last_velocity = ship->velocity;
		}


	}
	if ((&keys[MOVE_FORWARD_KEY])->pressed == false)
	{

		float v = ship->last_velocity - ship->decelrataion * dt;
		if (v > 0) {
			ship->pos.x += v * dt * cosf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->pos.y += v * dt * sinf((ship->rc * M_PI / 180) - (1.5 * M_PI));
			ship->last_velocity = v;
		}
		else {
			ship->pos.x += 0;
			ship->pos.y += 0;
			ship->last_velocity = 0;
		}

	}

}



void update_wave_state(struct asteroid* ast, struct ship* ship, float dt)
{
	time_wave_pending += dt;
	if (time_last_wave == 0 || time_wave_pending >= time_last_wave + TIME_BETWEEN_WAVES)
	{
		int max_drawn = asteroid_count;
		if ((asteroid_count + last_increment + INCREMENT_PER_WAVE) * 2 <= MAX_ASTEROIDS * 2) {
			max_drawn += last_increment + INCREMENT_PER_WAVE;
		}
		else {
			max_drawn = MAX_ASTEROIDS;
		}

		for (int i = asteroid_count * 2; i < max_drawn * 2; i += 2)
		{
			ast[i].pos.x = ast[i].launch_point.x + (arena_start_x + arena_width / 2);
			ast[i].pos.y = ast[i].launch_point.y + (arena_start_y + arena_height / 2);

			struct vec2d ast_minus_ship;
			ast_minus_ship.x = (ast[i].pos.x - ship->pos.x);
			ast_minus_ship.y = (ast[i].pos.y - ship->pos.y);
			float ast_minus_ship_mag = sqrtf(powf(ast_minus_ship.x, 2) + powf(ast_minus_ship.y, 2));

			ast[i].dir.x = ast_minus_ship.x / ast_minus_ship_mag;
			ast[i].dir.y = ast_minus_ship.y / ast_minus_ship_mag;

			ast[i].alive = true;
			ast[i].launched = true;
		}
		last_increment += INCREMENT_PER_WAVE;
		asteroid_count += last_increment;
		time_last_wave = time_wave_pending;


	}
}

void update_asteroid_state(struct asteroid* ast, float dt)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].alive)
		{
			ast[i].pos.x -= ast[i].velocity * dt * ast[i].dir.x;
			ast[i].pos.y -= ast[i].velocity * dt * ast[i].dir.y;
			ast[i].rc = ast[i].rc + (ast[i].rotate_dir) * (ast[i].rotate_speed * dt);
		}
	}

}

void update_asteroid_launch_point(struct asteroid* ast, struct ship* ship)
{

	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		float rand_number = (float)rand() / RAND_MAX; //random number between 0 and 1
		float radius = asteroid_circle_radius;
		float theta = rand_number * 2.0 * M_PI;

		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		ast[i].launch_point.x = x;
		ast[i].launch_point.y = y;
	}


}


void update_asteroids_particles(struct asteroid_particle* particles, struct asteroid* ast, float dt)
{

	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].exploded == true)
		{
			
			for (int j = i * NUMBER_ASTEROID_PARTICLES; j < i * NUMBER_ASTEROID_PARTICLES + NUMBER_ASTEROID_PARTICLES; j++)
			{
				if (particles[j].in_use == false)
				{
					particles[j].pos.x = ast[i].pos.x;
					particles[j].pos.y = ast[i].pos.y;
					particles[j].in_use = true;
					
				}
			}
		}
	}

	for (int i = 0; i < MAX_ASTEROIDS_PARTICLES; i++)
	{
		if (particles[i].in_use == true)
		{
			particles[i].pos.x += particles[i].dir.x * particles[i].velocity * dt;
			particles[i].pos.y += particles[i].dir.y * particles[i].velocity * dt;
		}
	}

}



void check_ship_asteroid_collision(struct asteroid* ast, struct ship* ship)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].alive == true) {
			float sum_radius = (ast[i].radius + ship->radius) * arena_scale;
			float distance_between_centers = sqrtf(powf((ast[i].pos.x - ship->pos.x), 2) + powf((ast[i].pos.y - ship->pos.y), 2));
			if (sum_radius >= distance_between_centers)
			{
				reposition_ship = true;
				quit_message = true;
				ship->alive = false;
			}
		}
	}


}

void reset_bullet(struct bullet* bl, struct ship* ship)
{
	for (int i = 0; i < MAX_BULLETS; i++) {
		bl[i].pos.x = ship->pos.x + ship->radius * arena_scale * cosf((ship->rc * M_PI / 180) - (1.5 * M_PI));;
		bl[i].pos.y = ship->pos.y + ship->radius * arena_scale * sinf((ship->rc * M_PI / 180) - (1.5 * M_PI));;
		bl[i].ship_rc = ship->rc;
		bl[i].alive = false;
		bl[i].fired = false;
	}

}

void check_bullet_asteroid_collision(struct bullet* bl, struct asteroid* ast, struct ship* ship)
{
	if (ship->alive == true) {
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bl[i].alive == true && bl[i].fired == true) {
				for (int j = 0; j < MAX_ASTEROIDS * 2; j++)
				{
					if (ast[j].alive == true)
					{
						float distance_between_centers = sqrtf(powf((ast[j].pos.x - bl[i].pos.x), 2) + powf((ast[j].pos.y - bl[i].pos.y), 2));
						if (distance_between_centers <= (ast[j].radius * arena_scale))
						{
							bl[i].alive = false;

							ast[j].hitpoint -= 1;

							if (ast[j].hitpoint <= 0 && ast[j].split == true) {
								ast[j].alive = false;
								ast[j].exploded = true;
								float hitpoint_f = 1 + HIT_POINT_MULTIPLIER * ast[j].radius;
								player_score += (int)round(hitpoint_f);
							}


						}
					}
				}

			}
		}
	}
}



void check_bullet_arena_collision(struct bullet* bl)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bl[i].alive == true && bl[i].fired == true) {
			if ((arena_start_x >= (bl[i].pos.x))
				|| (arena_end_x <= (bl[i].pos.x)))
			{
				bl[i].alive = false;
			}

			if ((arena_start_y >= (bl[i].pos.y))
				|| (arena_end_y <= (bl[i].pos.y)))
			{
				bl[i].alive = false;
			}
		}
	}
}

void update_bullet_state(struct bullet* bl, struct ship* ship, float dt)
{
	time_shot_pending += dt;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bl[i].fired == true && bl[i].alive == true) {
			bl[i].pos.x += bl[i].speed * dt * cosf((bl[i].ship_rc * M_PI / 180) - (1.5 * M_PI));
			bl[i].pos.y += bl[i].speed * dt * sinf((bl[i].ship_rc * M_PI / 180) - (1.5 * M_PI));

		}
		else {
			if (time_last_shot == 0 || time_shot_pending >= time_last_shot + BULLET_FIRE_RATE) {
				if ((&left_mouse_button)->pressed) {
					bl[i].pos.x = ship->pos.x + ship->radius * arena_scale * cosf((ship->rc * M_PI / 180) - (1.5 * M_PI));;
					bl[i].pos.y = ship->pos.y + ship->radius * arena_scale * sinf((ship->rc * M_PI / 180) - (1.5 * M_PI));;
					bl[i].ship_rc = ship->rc;
					bl[i].fired = true;
					bl[i].alive = true;
					time_last_shot = time_shot_pending;
				}
			}

		}
	}

}

bool all_bullets_fired_dead(struct bullet* bl) {

	bool all_dead = true;

	for (int i = 0; i < MAX_BULLETS; i++)
	{

		if (bl[i].alive == true) {
			all_dead = false;
		}
	}

	if (all_dead) {
		return true;
	}
	else {
		return false;
	}

}

bool all_asteroids_launched_and_dead(struct asteroid* ast) {

	bool all_launched = true;
	bool all_dead = true;

	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].launched == false)
		{
			all_launched = false;
		}
		if (ast[i].alive == true) {
			all_dead = false;
		}
	}

	if (all_launched && all_dead) {
		return true;
	}
	else {
		return false;
	}

}

bool all_asteroid_particles_in_use(struct asteroid_particle* particles)
{
	bool all_used = true;
	for (int i = 0; i < MAX_ASTEROIDS_PARTICLES; i++)
	{
		if (particles[i].in_use == false)
		{
			all_used = false;
		}
	}

	if (all_used) {
		return true;
	}
	else {
		return false;
	}
}

void check_asteroid_inside(struct asteroid* ast) {
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++) {
		if (ast[i].alive == true)
		{
			float scaled_radius = ast[i].radius * arena_scale;
			if ((ast[i].pos.x - scaled_radius >= arena_start_x)
				&& (ast[i].pos.x + scaled_radius <= arena_end_x)
				&& (ast[i].pos.y - scaled_radius >= arena_start_y)
				&& (ast[i].pos.y + scaled_radius <= arena_end_y)
				)
			{
				ast[i].inside = true;
			}

		}
	}


}

void check_asteroid_arena_collision(struct asteroid* ast)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].alive == true && ast[i].inside == true)
		{
			float scaled_radius = ast[i].radius * arena_scale;
			if ((ast[i].pos.x - scaled_radius <= arena_start_x))
			{
				ast[i].dir.x = (-1) * ast[i].dir.x;


			}

			if ((ast[i].pos.x + scaled_radius >= arena_end_x))
			{
				ast[i].dir.x = (-1) * ast[i].dir.x;

			}


			if ((ast[i].pos.y - scaled_radius <= arena_start_y))
			{
				ast[i].dir.y = (-1) * ast[i].dir.y;


			}

			if ((ast[i].pos.y + scaled_radius >= arena_end_y))
			{
				ast[i].dir.y = (-1) * ast[i].dir.y;


			}

		}
	}
}

void check_asteroids_collision(struct asteroid* ast)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i++)
	{
		if (ast[i].alive == true)
		{
			for (int j = 0; j < MAX_ASTEROIDS * 2; j++)
			{
				if (ast[j].alive == true) {
					if (i != j)
					{
						float sum_radius = (ast[i].radius + ast[j].radius) * arena_scale;
						float distance_between_centers = sqrtf(powf((ast[i].pos.x - ast[j].pos.x), 2) + powf((ast[i].pos.y - ast[j].pos.y), 2));
						if (sum_radius >= distance_between_centers)
						{
							struct vec2d tangent;
							tangent.y = -(ast[i].pos.x - ast[j].pos.x);
							tangent.x = (ast[i].pos.y - ast[j].pos.y);
							float tangent_mag = sqrtf(powf(tangent.x, 2) + powf(tangent.y, 2));

							struct vec2d tangent_unit;
							tangent_unit.x = tangent.x / tangent_mag;
							tangent_unit.y = tangent.y / tangent_mag;


							float angle_circle1 = acosf(ast[i].dir.x * tangent_unit.x + ast[i].dir.y * tangent_unit.y);
							float angle_circle2 = M_PI - acosf(ast[j].dir.x * tangent_unit.x + ast[j].dir.y * tangent_unit.y);
							
							//ast[i].dir.x = 2 * (tangent_unit.x * ast[i].dir.x) * tangent_unit.x - ast[i].dir.x;
							//ast[i].dir.y = 2 * (tangent_unit.y * ast[i].dir.y) * tangent_unit.y - ast[i].dir.y;

							//ast[j].dir.x = 2 * (tangent_unit.x * ast[j].dir.x) * tangent_unit.x - ast[j].dir.x;
							//ast[j].dir.y = 2 * (tangent_unit.y * ast[j].dir.y) * tangent_unit.y - ast[j].dir.y;

							ast[i].dir.x = cosf(angle_circle1);
							ast[i].dir.y = sinf(angle_circle1);

							ast[j].dir.x = cosf(angle_circle2);
							ast[j].dir.y = sinf(angle_circle2);

						
						}
					}
				}
			}
		}
	}
}

void check_asteroid_split(struct asteroid* ast, float dt)
{
	for (int i = 0; i < MAX_ASTEROIDS * 2; i += 2)
	{
		if (ast[i].alive == true && ast[i].hitpoint <= 0 && ast[i].split == false)
		{
			float new_radius = ast[i].radius / 2;
			ast[i + 1].velocity = ast[i].velocity;
			ast[i + 1].circle_lines = ast[i].circle_lines;

			for (int j = 0; j < ast[i].circle_lines; j++)
			{
				ast[i + 1].random_angles[j] = ast[i].random_angles[j];
			}

			ast[i + 1].rotate_speed = ast[i].rotate_speed;
			ast[i + 1].rotate_dir = ast[i].rotate_dir;
			ast[i + 1].r = ast[i].r;
			ast[i + 1].g = ast[i].g;
			ast[i + 1].b = ast[i].b;
			ast[i + 1].rc = ast[i].rc;

			ast[i + 1].radius = new_radius;
			float hitpoint_f = 1 + HIT_POINT_MULTIPLIER * new_radius;
			ast[i + 1].hitpoint = (int)round(hitpoint_f);

			ast[i + 1].alive = true;
			ast[i + 1].launched = true;
			ast[i + 1].split = true;

			ast[i].split = true;
			ast[i].radius = new_radius;
			ast[i].hitpoint = (int)round(hitpoint_f);

			struct vec2d original_pos;
			original_pos.x = ast[i].pos.x;
			original_pos.y = ast[i].pos.y;

			struct vec2d original_dir;
			original_dir.x = ast[i].dir.x;
			original_dir.y = ast[i].dir.y;

			struct vec2d perpendicular_vector;
			perpendicular_vector.y = (-1) * ast[i].dir.x;
			perpendicular_vector.x = ast[i].dir.y;

			ast[i + 1].dir.x = (-1) * ast[i].dir.x;
			ast[i + 1].dir.y = (-1) * ast[i].dir.y;

			ast[i].pos.x = original_pos.x + new_radius * arena_scale + perpendicular_vector.x * ast[i].velocity * dt;
			ast[i].pos.y = original_pos.y + new_radius * arena_scale + perpendicular_vector.y * ast[i].velocity * dt;

			ast[i + 1].pos.x = original_pos.x - new_radius * arena_scale - perpendicular_vector.x * ast[i + 1].velocity * dt * arena_scale;
			ast[i + 1].pos.y = original_pos.y + new_radius * arena_scale + perpendicular_vector.y * ast[i + 1].velocity * dt * arena_scale;

			ast[i].dir.x = cos(0.25 * M_PI) * original_dir.x - sin(0.25 * M_PI) * original_dir.y;
			ast[i].dir.y = sin(0.25 * M_PI) * original_dir.x + cos(0.25 * M_PI) * original_dir.y;


			ast[i + 1].dir.x = cos(-0.25 * M_PI) * original_dir.x - sin(-0.25 * M_PI) * original_dir.y;
			ast[i + 1].dir.y = sin(-0.25 * M_PI) * original_dir.x + cos(-0.25 * M_PI) * original_dir.y;


		}
	}
}


void reset_asteroids_particles(struct asteroid_particle* particles, float decay_time)
{
	for (int i = 0; i <= MAX_ASTEROIDS_PARTICLES; i++)
	{
		particles[i].decay_time = decay_time;
		particles[i].r = (float)rand() / RAND_MAX;
		particles[i].g = (float)rand() / RAND_MAX;
		particles[i].b = (float)rand() / RAND_MAX;

		float velocity_random = rand() / (float)RAND_MAX;
		float vel = PARTICLES_VEL_MIN + velocity_random * (PARTICLES_VEL_MAX - PARTICLES_VEL_MIN);
		particles[i].velocity = vel;

		float vector_random = rand() / (float)RAND_MAX;
		struct vec2d random_dir;
		int dir_array[2] = { -1,1 };
		int random_y = rand() % 2;
		random_dir.x = (-1 + vector_random * 2);
		random_dir.y = sqrtf(1 - pow(random_dir.x, 2)) * dir_array[random_y];

		particles[i].dir.x = random_dir.x;
		particles[i].dir.y = random_dir.y;

		particles[i].in_use = false;
		particles[i].size = PARTICLES_SIZE;
	}
}

void on_idle()
{
	float cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float dt = cur_time - last_time;

	if (quit_message == false) {
		player_time += dt;
	}

	if (reposition_ship) {

		if (quit_message == false) {
			
			ship_reshape(&first_ship);
			reset_asteroids_particles(asteroids_particles, ASTEROIDS_PARTICLES_DECAY_TIME);
			reset_asteroid(asteroids);
			reset_wave_time();
			reset_bullet(bullets, &first_ship);
			update_asteroid_launch_point(asteroids, &first_ship);
			reset_player_stat();
			reposition_ship = false;
		}
		else {

		}
	}

	update_wave_state(asteroids, &first_ship, dt);
	check_wall(&first_ship, WARNING_DISTANCE);
	check_asteroid_inside(asteroids);
	check_ship_asteroid_collision(asteroids, &first_ship);
	check_bullet_arena_collision(bullets);
	check_bullet_asteroid_collision(bullets, asteroids, &first_ship);
	check_asteroids_collision(asteroids);
	check_asteroid_arena_collision(asteroids);
	check_asteroid_split(asteroids, dt);


	if (all_bullets_fired_dead(bullets)) {
		reset_bullet(bullets, &first_ship);
	}

	if (all_asteroids_launched_and_dead(asteroids)) {
		reset_wave_time();
		reset_asteroid(asteroids);
		reset_asteroids_particles(asteroids_particles, ASTEROIDS_PARTICLES_DECAY_TIME);
		
	}

	update_asteroid_state(asteroids, dt);

	update_ship_state(&first_ship, dt);

	update_bullet_state(bullets, &first_ship, dt);

	update_asteroids_particles(asteroids_particles, asteroids, dt);


	last_time = cur_time;
	glutPostRedisplay();
}



void init_app(int* argcp, char** argv)
{
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Assignment 1");
	glutFullScreen();
	glutReshapeFunc(on_reshape);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_release);
	glutMouseFunc(on_mouse_button);

	glutDisplayFunc(on_display);
	glutIdleFunc(on_idle);

	screen_width = glutGet(GLUT_SCREEN_WIDTH);
	screen_height = glutGet(GLUT_SCREEN_HEIGHT);

	for (int i = 0; i < 256; i++) {
		key_init(&keys[i]);
	}

	mouse_init(&left_mouse_button);

	ship_init(&first_ship);

	//asteroids_particles_init(asteroids_particles, ASTEROIDS_PARTICLES_DECAY_TIME);


	for (int i = 0; i < MAX_ASTEROIDS * 2; i++) {
		asteroid_init(&asteroids[i], MIN_ASTEROID_SPEED, MAX_ASTEROID_SPEED, ASTEROID_R, ASTEROID_G, ASTEROID_B, MIN_ASTEROID_RADIUS, MAX_ASTEROID_RADIUS, MIN_ASTEROID_ROTATE_SPEED, MAX_ASTEROID_ROTATE_SPEED);
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		bullet_init(&bullets[i], BULLET_FIRE_RATE, BULLET_SPEED, BULLET_SIZE, BULLET_R, BULLET_G, BULLET_B);
	}

	for (int i = 0; i < MAX_SHIP_PARTICLES; i++) {
		ship_particle_init(&ship_particles[i], NUMBER_PARTICLES, DROP_TIME, STARTING_SIZE, DECAY_TIME);
	}

	
	last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

