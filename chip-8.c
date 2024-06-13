#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    uint8_t *ram;
    uint8_t *display;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t V[16];
    uint16_t PC;
    uint16_t I;
    uint16_t SP;
    uint16_t stack[16];

} chip8;


void DisasemblerChip8(uint8_t *ramBuffer, int pc){
    uint8_t *op = &ramBuffer[pc];

    uint8_t firstBit = *op >> 4;
    printf("%04x %02x %02x ", pc, op[0], op[1]);
    switch(firstBit){
        case 0x00:
            if (op[0] != 0x00){
                printf("subroutine at %x%02x\n", op[0] & 0x0f, op[1]);
                break;
            }
            switch(op[1]){
                case 0xE0:
                    printf("clear\n");
                    break;
                case 0xEE:
                    printf("return\n");
                    break;
                default:
                    printf("subroutine at %x%02x\n", op[0] & 0x0f, op[1]);
                    break;
            }
            break;
        case 0x01:
            printf("jump %x%02x\n", op[0] & 0x0f, op[1]);
            break;
        case 0x02:
            printf("call subroutine %x%02x\n", op[0] & 0x0f, op[1]);
            break;
        case 0x03:
            printf("if v%x != %02x then\n", op[0] & 0x0f, op[1]);
            break;
        case 0x04:
            printf("if v%x == %02x then\n", op[0] & 0x0f, op[1]);
            break;
        case 0x05:
            printf("if v%x == v%x then\n", op[0] & 0x0f, op[1] & 0xf0);
            break;
        case 0x06:
            printf("v%x := %02x\n", op[0] & 0x0f, op[1]);
            break;
        case 0x07:
            printf("v%x += %02x\n", op[0] & 0x0f, op[1]);
            break;
        case 0x08:
            switch(op[1] & 0x0f){
                case 0x00:
                    printf("v%x := v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x01:
                    printf("v%x |= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x02:
                    printf("v%x &= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x03:
                    printf("v%x ^= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x04:
                    printf("v%x += v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x05:
                    printf("v%x -= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x06:
                    printf("v%x >>= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x07:
                    printf("v%x =- v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                case 0x0e:
                    printf("v%x <<= v%x\n", op[0] & 0x0f, op[1] & 0xf0);
                    break;
                default:
                    printf("\n");
                    break;
            }
            break;
        case 0x09:
            printf("if v%x == v%x then\n", op[0] & 0x0f, op[1] & 0xf0);
            break;
        case 0x0a:
            printf("i := %x%02x\n", op[0] & 0x0f , op[1]);
            break;
        case 0x0b:
            printf("jump0 %x%02x\n", op[0] & 0x0f , op[1]);
            break;
        case 0x0c:
            printf("v%x := random %02x\n",op[0] & 0x0f , op[1]);
            break;
        case 0x0d:
            printf("sprite v%x v%x %x\n", op[0] & 0x0f, op[1] & 0xf0, op[1] & 0x0f);
            break;
        case 0x0e:
            switch(op[1]){
                case 0x9E:
                    printf("if v%x -key then\n", op[0] & 0x0f);
                    break;
                case 0xA1:
                    printf("if v%x key then\n", op[0] & 0x0f);
                    break;
                default:
                    printf("\n");
                    break;
            }
            break;
        case 0x0f:
            switch(op[1]){
                case 0x07:
                    printf("v%x := delay\n", op[0] & 0x0f);
                    break;
                case 0x0a:
                    printf("v%x := key\n", op[0] & 0x0f);
                    break;
                case 0x15:
                    printf("delay := v%x\n", op[0] & 0x0f);
                    break;
                case 0x18:
                    printf("buzzer := v%x\n", op[0] & 0x0f);
                    break;
                case 0x1e:
                    printf("i += v%x\n", op[0] & 0x0f);
                    break;
                case 0x29:
                    printf("i += hex v%x\n", op[0] & 0x0f);
                    break;
                case 0x33:
                    printf("bcd v%x\n", op[0] & 0x0f);
                    break;
                case 0x55:
                    printf("save v%x\n", op[0] & 0x0f);
                    break;
                case 0x65:
                    printf("load v%x\n", op[0] & 0x0f);
                    break;
                default:
                    printf("\n");
                    break;    
            }
            break;    
        default:
            break;
    }
}


void InstructionDecoder(chip8 *c8, int pc){
    uint8_t *op = &c8->ram[pc];
    printf("%d\n", *op);
    uint8_t firstBit = *op >> 4;
    printf("%04x %02x %02x ", pc, op[0], op[1]);
    
    switch(firstBit){
        case 0x00:
            switch(op[1]){
                case 0xE0:
                    printf("clear\n");
                    break;
                case 0xEE: //00EE: Return from a subroutine
                    c8->PC = c8->stack[SP];
                    c8->SP--;
                    break;
                default:
                    break;
            }
            break;
        case 0x01: //1NNN: Jump to address NNN
            c8->PC = (op[0] << 8 | op[1]) & 0x0fff;  
            break;
        case 0x02: //2NNN: Call subroutine at address NNN
            c8->stack[SP] = c8->PC;
            c8->SP++;
            c8->PC = (op[0] << 8 | op[1]) & 0x0fff;
            break;
        case 0x03: //3XNN: Skip the following instruction if the value of register VX equals NN
            if(c8->V[op[0] & 0x0f] == op[1]){
                c8->PC +=2;
            }
            break;        
        case 0x04: //4XNN: Skip the following instruction if the value of register VX is not equal to NN
            if(c8->V[op[0] & 0x0f] != op[1]){
                c8->PC += 2;
            }
            break;
        case 0x05: //5XY0: Skip the following instruction if the value of register VX is equal to the value of register VY 
            if(c8->V[op[0] & 0x0f] == c8->V[op[1] & 0xf0]){
                c8->PC += 2;
            }
            break;
        case 0x06: //6XNN: Store number NN in register VX
            c8->V[op[0] & 0x0f] = op[1];
            c8->PC += 2;
            break;
        case 0x07: //7XNN: Add the value NN to register VX
            c8->V[op[0] & 0x0f] += op[1];
            c8->PC += 2;
            break;
        case 0x08:
            switch(op[1] & 0x0f){
                case 0x00: //8XY0: Store the value of register VY in register VX
                    c8->V[op[0] & 0x0f] = c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x01: //8XY1: Set VX to VX OR VY
                    c8->V[op[0] & 0x0f] = c8->V[op[0] & 0x0f] | c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x02: //8XY2: Set VX to VX AND VY
                    c8->V[op[0] & 0x0f] = c8->V[op[0] & 0x0f] & c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x03: //8XY3: Set VX to VX XOR VY
                    c8->V[op[0] & 0x0f] = c8->V[op[0] & 0x0f] ^ c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x04: //8XY4: Add the value of register VY to register VX
                           //Set VF to 01 if a borrow occurs
                           //Set VF to 00 if a carry does not occur
                    if(c8->V[op[1] & 0xf0] > (0xFF - c8->V[op[0] & 0x0f])){
                        c8->V[0xf] = 1;
                    }else{
                        c8->V[0xf] = 0;
                    }
                    c8->V[op[0] & 0x0f] += c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x05: //8XY5: Set register VX to the value of VX minus VY
                           //Set VF to 00 if a borrow occurs
                           //Set VF to 01 if borrow does not occur
                    if(c8->V[op[0] & 0x0f] > c8->V[op[1] & 0xf0]){
                        c8->V[0xf] = 1;
                    }else{
                        c8->V[0xf] = 0;
                    }
                    c8->v[op[0] & 0x0f] -= c8->V[op[1] & 0xf0];
                    c8->PC += 2;
                    break;
                case 0x06: //8XY6: Store the value of register VY shifted right one bit in register VX
                           //Set register VF to the least significant bit prior to the shift
                    c8->V[0xf] = c8->V[op[1] & 0xf0] & 0b00000001;
                    c8->V[op[0] & 0x0f] = c8->V[op[1] & 0xf0] >> 1;
                    c8->PC += 2;
                    break;
                case 0x07:  //8XY7: Set register VX to the value of VY minus VX
                            //Set VF to 00 if a borrow occurs
                            //Set VF to 01 if a borrow does not occur
                    if(c8->V[op[1] & 0xf0] > c8->V[op[0] & 0x0f]){
                        c8->V[0xf] = 1;
                    }else{
                        c8->V[0xf] = 0;
                    }
                    c8->v[op[0] & 0x0f] = c8->V[op[1] & 0xf0] - c8->v[op[0] & 0x0f];
                    c8->PC += 2;
                    break;
                case 0x0e: //8XYE: Store the value of register VY shifted left one bit in register VX
                           //Set register VF to the most significant bit prior to the shift
                    c8->V[0xf] = c8->V[op[1] & 0xf0] & 0b10000000;
                    c8->V[op[0] & 0x0f] = c8->V[op[1] & 0xf0] << 1;
                    c8->PC += 2;
                    break;
                default:
                    printf("\n");
                    break;
            }
            break;
        case 0x09: //9XY0: Skip the following instruction if the value of register VX is not equal to the value of register VY
            if(c8->V[op[0] & 0x0f] != c8->V[op[1] & 0xf0]){
                c8->PC += 2;
            }
            break;
        case 0x0a: //ANNN: Store memory address NNN in register I
            c8->I = (op[0] << 8 | op[1]) & 0x0fff; 
            c8->PC += 2; 
            break;
        case 0x0b: //BNNN: Jump to address NNN+V0
            c8->PC = ((op[0] << 8 | op[1]) & 0x0fff) + PC->V[0];  
            break;
        case 0x0c: //CXNN: Set VX to a random number with a masf of NN
            c8->V[op[0] & 0x0f] = (rand() % 255) & op[1];
            break;
        case 0x0d:
            printf("sprite v%x v%x %x\n", op[0] & 0x0f, op[1] & 0xf0, op[1] & 0x0f);
            break;
        case 0x0e:
            switch(op[1]){
                case 0x9E:
                    printf("if v%x -key then\n", op[0] & 0x0f);
                    break;
                case 0xA1:
                    printf("if v%x key then\n", op[0] & 0x0f);
                    break;
                default:
                    printf("\n");
                    break;
            }
            break;
        case 0x0f:
            switch(op[1]){
                case 0x07: //FX07: Store the current value of the delay timer in register VX
                    c8->V[op[0] & 0x0f] = c8->delay_timer;
                    c8->PC += 2;
                    break;
                case 0x0a:
                    printf("v%x := key\n", op[0] & 0x0f);
                    break;
                case 0x15: //FX15: Set the delay timer to the value of register VX
                    c8->delay_timer = c8->V[op[0] & 0x0f];
                    c8->PC += 2;
                    break;
                case 0x18: //FX18: Set the sound timer to the value of register VX
                    c8->sound_timer = c8->V[op[0] & 0x0f];
                    c8->PC += 2;
                    break;
                case 0x1e: //FX1E: Add the value stored in register VX to register I
                    c8->I += c8->V[op[0] & 0x0f];
                    c8->PC += 2;
                    break;
                case 0x29:
                    printf("i += hex v%x\n", op[0] & 0x0f);
                    break;
                case 0x33:
                    printf("bcd v%x\n", op[0] & 0x0f);
                    break;
                case 0x55:
                    printf("save v%x\n", op[0] & 0x0f);
                    break;
                case 0x65:
                    printf("load v%x\n", op[0] & 0x0f);
                    break;
                default:
                    printf("\n");
                    break;    
            }
            break;    
        default:
            break;
    }
}


chip8* InitChip8(void){
    chip8* c = calloc(sizeof(chip8), 1);
    
    c->ram = calloc(0x1000, 1);
    c->display = calloc(0x800, 1);
    c->SP = 0x0;
    c->PC = 0x200;

    uint8_t fontset[80] =
     {
       0xF0, 0x90, 0x90, 0x90, 0xF0, //0
       0x20, 0x60, 0x20, 0x20, 0x70, //1
       0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
       0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
       0x90, 0x90, 0xF0, 0x10, 0x10, //4
       0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
       0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
       0xF0, 0x10, 0x20, 0x40, 0x40, //7
       0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
       0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
       0xF0, 0x90, 0xF0, 0x90, 0x90, //A
       0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
       0xF0, 0x80, 0x80, 0x80, 0xF0, //C
       0xE0, 0x90, 0x90, 0x90, 0xE0, //D
       0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
       0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };
    
    for(int i = 0x50; i <= 0xA0; i++){
      c->ram[i] = fontset[i];
    }
    

    return c;
}

int main(int argc, char**argv){
    srand(time(NULL))
    chip8 *c8 = InitChip8();
    
    FILE *fp = fopen(argv[1], "rb");
    if (fp==NULL){
        printf("couldn't open %s\n", argv[1]);
        exit(1);
    }

    //Get the file size
    fseek(fp, 0L, SEEK_END);
    int fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //The program is loaded into the RAM at address 0x200
    //This is the standard for CHIP-8 programs
    fread(c8->ram+0x200, fsize, 1, fp);
    
    fclose(fp);
    
    for(int c = 0; c < 4096 ; c += 1){
        printf("%d: %x\n", c, c8->ram[c]);
    }
    //printf("%X", c8->ram[200]);
    
    
    InstructionDecoder(c8, 0x200);

    /*
    while(c8->PC < (fsize+0x200)){
        DisasemblerChip8(c8->ram, c8->PC);
        c8->PC += 2;
    }
    */
    return 0;
}
