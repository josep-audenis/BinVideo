#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stb_image_write.h>

#define WIDTH 640
#define HEIGHT 480
//(WIDTH x HEIGHT) % 8 = 0
#define BYTE_SIZE 8
#define FRAME_BYTES 640*480
#define VIDEO_EXTENSION ".mp4"
#define BINARY_SIZE 40


int* decimalToBinary(int decimal){
    int* binary = (int*)malloc(BINARY_SIZE * sizeof(int));
    int i = 0;

    while(decimal > 0){
        binary[i] = decimal % 2;
        decimal /= 2;
        i++;
    }

    while(i < 40){
        binary[i] = 0;
        i++;
    }

    return binary;
}

int binaryToDecimal(/*passa array del binary*/){
    int decimal = 0;
    int power = 1;

    for(int i = 0; i < BINARY_SIZE; i++){
        decimal += binary[i] * power;
        power *= 2;  
    }

    return decimal;
}

//Funcio que donat el nom d'un fitxer parsejar el seu valor binari i el converteix en un video
void file_to_video(const char *input_file, const char *output_file){
    
    FILE *fil e = fopen(input_file, "rb");
    if (file == NULL){
        perror("Error opening file %s", input_file);
        return;
    }

    uint8_t *frame = (uint8_t*)malloc(WIDTH * HEIGHT);
    
    if (frame == NULL){
        perror("Error allocating frame memory");
        return;
    }

    printf("Converting %s into video...\n", input_file);
    size_t bytes_read;
    size_t current_bit = 0;

    while((bytes_read = fread(frame + current_bit / BYTE_SIZE, 1, 1, file)) > 0){
        //paresejar tot el byte
        for (int i = 0; i < BYTE_SIZE; i++){
            //aillar el bit actual
            int bit = (frame[current_bit / BYTE_SIZE] >> (7 - i)) & 1;
            //comprovar el valor binari i assignar blanc(1-255) o negre(0-0)
            frame[current_bit] = bit == 1 ? 255 : 0; 
            current_bit++;
            //comprovar limit de bytes per frame
            if (current_bit >= FRAME_BYTES){
                //generar frame
                char filename[256];
                sprintf(filename, "frame%05d.png", current_bit / FRAME_BYTES);
                printf("\nGenerated frame%05d.png", current_bit / FRAME_BYTES);
                stbi_write_png(filename, WIDTH, HEIGHT, 1, frame, WIDTH);
                current_bit = 0;
            }
        }
    }

    if (current_bit > 0) {
        char filename[256];
        sprintf(filename, "frame%05d.png", current_bit / FRAME_BYTES);
        stbi_write_png(filename, WIDTH, HEIGHT, 1, frame, WIDTH);
    }

    free(frame);
    fclose(file);

    //al guardar el nom del video es guardara no el nom que es demana sino que es guardara amb el següent format: nom del fitxer original_format del fitxer original_tamany del fitxer original en bytes.mp4
    //de cara al futur es podria mirar de reservar els primers bits del video a aquesta informacio
    //- Reservar els 40 primers bits per al tamany original (aprox)
    //- Reservar els 4 següents bits per a l'extensio original


}

void video_to_file(){

}

int main(){


    int* binary = decimalToBinary(/*number*/);
    for (int i = 40; i > 0; i--){
        printf("\n%d", binary[i-1]);
    }
    int decimal = binaryToDecimal(binary);
    printf("\n%d", decimal);
    

    free(binary);
    return 0;
}
