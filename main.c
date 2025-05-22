#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct S_CPU
{
	uint8_t A;
	uint8_t F;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;
	uint16_t SP;
	uint16_t PC;
} t_CPU;

enum instructions_op
{
	NOP 	= 0x00,
	JP	= 0xC3,
};

typedef struct s_game_boy
{
	t_CPU CPU;
	uint8_t memory[0xFFFF]; // 16 bits memory
} t_game_boy;

void nop()
{
	// Well, it does nothing.
}

void execute(t_game_boy *game_boy)
{
	uint8_t opcode = game_boy->memory[game_boy->CPU.PC];

	if (opcode == NOP)
	{
		nop();
		game_boy->CPU.PC += 1;
	}
	else if (opcode == JP)
	{
		uint16_t addr = game_boy->memory[game_boy->CPU.PC++];
		addr |= game_boy->memory[game_boy->CPU.PC++] << 8;
		game_boy->CPU.PC = addr;
	}
	else
	{
		printf("Instruction Not Implemented: 0x%X\n", opcode);
		exit (1);
	}
}

int main()
{
	t_game_boy game_boy;
	game_boy.CPU.PC = 0;
	memset(&game_boy.memory, 0, 0xFFFF);

	// Load ROM into the memory
	int fd = open("tetris.gb", O_RDONLY);
	int res = read(fd, game_boy.memory, 0xFFFF);
	if (res < 0)
	{
		printf("Error while reading");
		return (1);
	}

	// We need to jump to the address 0x100 for some reason;
	game_boy.CPU.PC = 0x100;
	
	// Just 
	while (1)
	{
		execute(&game_boy);
		if (game_boy.memory[game_boy.CPU.PC - 1] == 0xFF)
			break;
	}

	printf("Value at Memory %x: %d", 0x1234, game_boy.memory[0x1234]); 

}
