#ifndef LNC_DL_H
#define LNC_DL_H

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

namespace lnc {
    namespace dl {
        enum class CURSOR {
            SHOW,
            HIDE
        };
        /* Clear screen */
        void clear(){std::cout << "\033[49;37m" << std::flush << "\033[H\033[2J" << std::flush;}
        
        void set_color(int bg, int fg){std::cout << "\033[" << bg << ";" << fg << "m" << std::flush;}
        
        /* Set cursor mode */
        
        void cursor_mode(dl::CURSOR mode){
            switch(mode){
                case dl::CURSOR::SHOW:
                    std::cout << "\033[?25h" << std::flush;
                    break;
                case dl::CURSOR::HIDE:
                    std::cout << "\033[?25l" << std::flush;
                    break;
            }
        }
        
        /* Draw rectangle */
        
        void draw_rectangle(int rect_w_0, int rect_h_0, int rect_w_1, int rect_h_1, int rect_colour_fg, int rect_colour_bg, char sym){
            int delta_r_h{rect_h_1 - rect_h_0};
                int delta_r_w{rect_w_1 - rect_w_0};
            std::string symbols;
            std::string symbols_1;
            for(int count{0}; count <= delta_r_w; ++count){
                symbols += sym;
            }
            for(int count{0}; count <= delta_r_h; ++count){
                int cprh{rect_h_0 + count};
                symbols_1 += "\033[" + std::to_string(cprh) + ";" + std::to_string(rect_w_0) + "H" + symbols;
            }
            std::string buff{"\033[" + std::to_string(rect_colour_bg) + ";" + std::to_string(rect_colour_fg) + "m" + symbols_1};
            std::cout << buff << std::flush;
        }
     
        /* Go to position x, y */
     
        void gotoxy(int x, int y){
                std::cout << "\033[" << y << ";" << x << "H" << std::flush;
        }
     
        /* Print text to position x, y with colour */
        
        void printxy(int x, int y, std::string text, int bg, int fg){
                gotoxy(x, y);
            set_color(bg, fg);
                std::cout << text << std::flush;
        }
     
        /* Load picture from .esc file */
        
        void load_picture(std::string filename, std::string (&to_str)){
            std::ifstream picfile((filename).c_str());
            to_str.clear();
            char sym;
            while((sym = picfile.get()) != EOF){
                to_str += sym;
            }
            picfile.close();
        }
        
        /* Draw text color picture */
     
        void draw_picture(int pos_x, int pos_y, std::string (&str)) {
            int count_2 { 0 };
            for(int count_1 { 0 }; count_1 < static_cast<int>(str.length()); ++count_1){
                if(str[static_cast<unsigned long>(count_1)] == '\n'){
                    dl::gotoxy(pos_x, pos_y + count_2);
                    ++count_2;
                    ++count_1;
                }
                std::cout << str[static_cast<unsigned long>(count_1)] << std::flush;
            }
        } 
        
        /* Draw line function */
     
        void drawLine(int x1, int y1, int x2, int y2, int background, int foreground, std::string sym) {
            const int deltaX = abs(x2 - x1);
            const int deltaY = abs(y2 - y1);
            const int signX = x1 < x2 ? 1 : -1;
            const int signY = y1 < y2 ? 1 : -1;
            int error = deltaX - deltaY;
            dl::printxy(x2, y2, sym, background, foreground);
            while(x1 != x2 || y1 != y2) 
            {
                dl::printxy(x1, y1, sym, background, foreground);
                const int error2 = error * 2;
                if(error2 > -deltaY) {
                    error -= deltaY;
                    x1 += signX;
                }
                if(error2 < deltaX) {
                    error += deltaX;
                    y1 += signY;
                }
            }                    
        }
    } 
}

#endif /* LNC_DL_H */
