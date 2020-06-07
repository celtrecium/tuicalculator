
/*                Warning
 * This file may contains very bad code ;)
 */

#include <iostream>
#include <string>
#include <fstream>
#include <string.h>

void load_picture(int width, int height, std::string (&pic), std::string (&to_str)) {
    int color_bg { 0 };
    int color_fg { 0 };
    int expr { 0 };
    
    for(int count_1 { 0 }; count_1 < height; ++count_1){
        to_str += '\n';
        for(int count_2 { 0 }; count_2 < width; ++count_2){
            expr = (count_1 * width) + count_2;
            switch(pic[ expr * 3 + 1 ]){
                case '0': color_bg = 40; break;
                case '1': color_bg = 41; break;
                case '2': color_bg = 42; break;
                case '3': color_bg = 43; break;
                case '4': color_bg = 44; break;
                case '5': color_bg = 45; break;
                case '6': color_bg = 46; break;
                case '7': color_bg = 47; break;
                case '8': color_bg = 48; break;
                case '9': color_bg = 49; break;
                case 'A': color_bg = 100; break;
                case 'B': color_bg = 101; break;
                case 'C': color_bg = 102; break;
                case 'D': color_bg = 103; break;
                case 'E': color_bg = 104; break;
                case 'F': color_bg = 105; break;
                case 'G': color_bg = 106; break;
                case 'H': color_bg = 107; break;
            }
            switch(pic[ expr * 3 + 2 ]){
                case '0': color_fg = 30; break;
                case '1': color_fg = 31; break;
                case '2': color_fg = 32; break;
                case '3': color_fg = 33; break;
                case '4': color_fg = 34; break;
                case '5': color_fg = 35; break;
                case '6': color_fg = 36; break;
                case '7': color_fg = 37; break;
                case '8': color_fg = 38; break;
                case '9': color_fg = 39; break;
                case 'A': color_fg = 90; break;
                case 'B': color_fg = 91; break;
                case 'C': color_fg = 92; break;
                case 'D': color_fg = 93; break;
                case 'E': color_fg = 94; break;
                case 'F': color_fg = 95; break;
                case 'G': color_fg = 96; break;
                case 'H': color_fg = 97; break;
            }
            if(pic[ expr * 3 - 1 ] != pic[ expr * 3 + 2 ] || pic[ expr * 3 - 2 ] != pic[ expr * 3 + 1 ]){
                to_str += "\033[" + std::to_string(color_bg) + ";" + std::to_string(color_fg) + "m";
            }
            to_str += pic[ static_cast<unsigned long>(expr * 3) ];
        }
    }
    std::cout << "File converting done\n";
}

void load_from_file(char *filename, std::string (&to_str), int (&width), int (&height)){
    std::ifstream picfile(filename);
    if(picfile.is_open()){
        to_str.clear();
        char w[5];
        char h[5];
        picfile.getline(w, 5);
        picfile.getline(h, 5);
        width = atoi(w);
        height = atoi(h);
        char sym;
        while((sym = picfile.get()) != EOF){
            if(sym == '\n') {}
            else {
                to_str += sym;
            }
        }
        std::cout << "File loading done\n";
        picfile.close();
    }
    else { 
        std::cout << "File not open\n";
    }
}

int main(int argc, char *argv[]){
    std::string raw_str;
    std::string pic;
    int width, height;
    if(argc > 1){
        load_from_file(argv[1], raw_str, width, height);
        load_picture(width, height, raw_str, pic);
        if(strcmp(argv[2], "-o") == 0){
            std::ofstream output(argv[3]);
            output << pic;
            std::cout << "File " << argv[3] << " writing done\n";
            output.close();
        }
        else{
            std::ofstream output("out.esc");
            output << pic;
            output.close();
        }
    }
    return 0;
}
