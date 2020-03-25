#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#define SNAKE_CNT 1


// Typedef structure contains current game settings/score which will be saved to EEPROM
typedef struct
{
		int8_t max_score;
		uint8_t speed;
		uint8_t users_cnt;
} TCFG;

// Typedef structure contains snake's features
typedef struct
{
	uint8_t speed;
	int8_t points;
	char face;
	uint8_t keys[4];
	uint8_t direction;
	uint8_t body_len;
	uint8_t head;
	uint16_t byx[512];
	void(*draw_snake_fun)(void * tsn);
} TSNAKE;

// Bit field with value of 1 bit per variable
struct
{
	uint8_t rightDirection:	1;
	uint8_t leftDirection:	1;
	uint8_t upDirection:	1;
	uint8_t downDirection:	1;

} s_direction;

// Inline function which obtains current Y coordinate
inline uint16_t get_y(uint16_t yx)
{
	return (uint16_t)(yx>>8);
}

// Inline function which obtains current X coordinate
inline uint8_t get_x(uint8_t yx)
{
	return (uint8_t)yx;
}

// enum for snake's directions
enum {left, right, up, down};

// enum for snake's speed;
enum {min = 20, mid = 30, max = 40};

//
extern uint8_t restart;
//
extern uint8_t snake_cnt;
//
extern TCFG ram_cfg;
//
void snake_init(uint8_t cnt);
//
void draw_snake(TSNAKE * tsn);
//
TSNAKE * snakes(uint8_t n);

//
void register_start_screen_callback( void (*callback)(unsigned char usr_cnt) );
void register_end_screen_callback( void (*callback)(unsigned char usr_cnt) );
//
void check_and_load_defaults(void);
void load_defaults(void);
void copy_ram_to_eem(void);


#endif /* SNAKE_SNAKE_H_ */
