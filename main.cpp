#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream> 
#include <vector>
#include <cmath>
#include <string>
#include <chrono>

using namespace std;

#define WIDTH 620
#define HEIGHT 620
#define PIECE_SIZE 80

void draw_gamespace(vector<vector<int>> gamespace) {
    for (int i = 0; i < gamespace.size(); i++) {
        cout << "| ";
        for (int e = 0; e < gamespace[i].size(); e++) {
            cout << gamespace[i][e] << " | ";
        }
        cout << "\n"; 
    } 
}
void number_generator(vector<vector<int>>& gamespace) {
    int row;
    int position;
    while (true) {
        row = rand() % 4; 
        position = rand() % 4; 
        if (gamespace[row][position] == 0) {
            int generator = rand() % 10;
            int starting_number;
            if (generator == 9) {
                starting_number = 4;
            }
            else {
                starting_number = 2;
            }
            gamespace[row][position] = starting_number;
            break; 
        }
    }
}
bool possible_moves(vector<int> row,vector<bool> has_merged){
    for (int i = 0; i < row.size() - 1; i++){
        if (((row[i] == row[i + 1] && has_merged[i] == false && has_merged[i + 1] == false) || row[i + 1] == 0) && row[i] != 0 ) {
            return true;
        }
    }
    return false;
}
bool possible_moves_all_rows(vector<vector<int>>& gamespace){
    vector<bool> has_merged = {false, false, false, false};
    for (int i = 0; i < gamespace.size(); i++){
        if (possible_moves(gamespace[i], has_merged)) {
            return true;
        }
    }
    return false;
}
void movement_base(vector<vector<int>>& gamespace) {
    for (int i = 0; i < gamespace.size(); i++){
        vector<bool> has_merged = {false, false, false, false};
        while(possible_moves(gamespace[i], has_merged)) {
            for (int e = gamespace[i].size() - 2; e >= 0  ; e--){
                if (gamespace[i][e + 1] == 0){
                    gamespace[i][e + 1] = gamespace[i][e];
                    gamespace[i][e] = 0;
                    has_merged[e + 1] = has_merged[e];    
                }
                else if (gamespace[i][e] == gamespace[i][e + 1] && !has_merged[e + 1] && !has_merged[e]) {
                    gamespace[i][e + 1] = gamespace[i][e] * 2;
                    gamespace[i][e] = 0;
                    has_merged[e + 1] = true;
                }
            }
            
        }
    }
}
vector<vector<int>> rotate_gamespace_right(vector<vector<int>> gamespace){
    vector<vector<int>> gamespace_rotated = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}; 
        for(int e = 0; e < gamespace_rotated.size(); e++) {
            for (int i = gamespace_rotated.size() - 1; i >= 0; i--) {
                gamespace_rotated[e][3 - i] = gamespace[i][e];
            }
        }  
        return gamespace_rotated;   
}
vector<vector<int>> rotate_gamespace_left(vector<vector<int>> gamespace){
    vector<vector<int>> gamespace_rotated = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}; 
    for (int e = gamespace_rotated.size() - 1; e >= 0; e--){
        for(int i = 0; i < gamespace_rotated.size(); i++){
            gamespace_rotated[3 - e][i] = gamespace[i][e];
        }
    }
    return gamespace_rotated;
}
bool game_won(vector<vector<int>> gamespace){
    for(int i = 0; i < gamespace.size() - 1; i++){
        for(int e = 0; e < gamespace[i].size() - 1; e++){
            if(gamespace[i][e] == 2048){
                return true;
            }  
        }
    }
    return false;
}
bool game_is_lost(vector<vector<int>> gamespace){
    for(int i = 0; i < 4; i++){
        if(possible_moves_all_rows(gamespace)) {
            return false;
        }
        gamespace = rotate_gamespace_left(gamespace);
    }
    return true;
}

int textWidth(TTF_Font* font, const char* text) {
    int w, h;
    TTF_SizeText(font, text, &w, &h);
    return w;
};
int textHeight(TTF_Font* font, const char* text) {
    int w, h;
    TTF_SizeText(font, text, &w, &h);
    return h;
};
SDL_Color getNumberColor (string value) {
    if (value == "gamespace") return {187, 173, 160};
    if (value == "piece") return {205, 193, 181};
    if (value == "2") return {238, 228, 218};
    if (value == "4") return {237, 224, 200};
    if (value == "8") return {242, 177, 121};
    if (value == "16") return {245, 147, 99};
    if (value == "32") return {246, 124, 96};
    if (value == "64") return {246, 94, 59};
    if (value == "128") return {237, 207, 115};
    if (value == "256") return {237, 204, 98};
    if (value == "512") return {237, 200, 80};
    if (value == "1024") return {237, 197, 63};
    if (value == "2048") return {237, 194, 45};
    return {205, 193, 181};
}
void drawRect(SDL_Renderer* renderer, int x, int y, int w, int h, string color, string text = "", TTF_Font* font = NULL) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_Color renderColor = getNumberColor(text != "" ? text : color);
  SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, 255);
  SDL_RenderFillRect(renderer, &rect);

  SDL_Color white = {255, 255, 255};
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), white);

  
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  int textW = textWidth(font, text.c_str());
  int textH = textHeight(font, text.c_str());

  SDL_Rect Message_rect;
  Message_rect.x = x + (w - textW) / 2; 
  Message_rect.y = y + (h - textH) / 2;
  Message_rect.w = textW;
  Message_rect.h = textH;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}
void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 251, 248, 239, 255);
  SDL_RenderClear(renderer);
}

void drawGamePieces(SDL_Renderer* renderer, vector<vector<int>> gamespace, TTF_Font* font) {
    for(int i = 0; i < gamespace.size(); i++){
        for(int e = 0; e < gamespace[i].size(); e++){
            drawRect(renderer, e * PIECE_SIZE + 20 * (e + 1) + 100, i * PIECE_SIZE + 20 * (i + 1) + 100, PIECE_SIZE, PIECE_SIZE, "piece", to_string(gamespace[i][e]), font);
        }
    }
}

void drawTick (SDL_Renderer* renderer, vector<vector<int>> gamespace, TTF_Font* font) {
  SDL_RenderClear(renderer);
  drawBackground(renderer);
  drawRect(renderer, 100, 100, 420, 420, "gamespace");
  drawGamePieces(renderer, gamespace, font);
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    TTF_Font* Sans = TTF_OpenFont("OpenSans.ttf", 60);

    SDL_Window *window;
    SDL_Renderer *renderer;

    // window = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    srand(time(NULL));
    //Tabuleiro de jogo
    vector<vector<int>> gamespace = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}; 
    //Moves
    //Geracao de numeros
        //2,4 em qualquer posicao livre a cada movimento
    bool game_is_running = true;
    number_generator(gamespace);
    number_generator(gamespace);
    drawTick(renderer, gamespace, Sans);
    char movement;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event quit;
    
    int ticksToInput = 0;
    
    while(game_is_running) {
        if (ticksToInput == 0) {
            SDL_PollEvent(&quit);
            ticksToInput = 100;
            if (keys[SDL_SCANCODE_RIGHT]) {
                if (possible_moves_all_rows(gamespace)) {
                    movement_base(gamespace);
                    number_generator(gamespace);
                }
            }
            if (keys[SDL_SCANCODE_UP]) {
                gamespace = rotate_gamespace_right(gamespace);
                if(possible_moves_all_rows(gamespace)){
                    movement_base(gamespace);
                    number_generator(gamespace);
                }
                gamespace = rotate_gamespace_left(gamespace);
            }
            if (keys[SDL_SCANCODE_LEFT]){
                gamespace = rotate_gamespace_left(gamespace);
                gamespace = rotate_gamespace_left(gamespace);
                if(possible_moves_all_rows(gamespace)){
                    movement_base(gamespace);
                    number_generator(gamespace);
                }
                gamespace = rotate_gamespace_right(gamespace);
                gamespace = rotate_gamespace_right(gamespace);
            }
            if (keys[SDL_SCANCODE_DOWN]) {
                gamespace = rotate_gamespace_left(gamespace);
                if(possible_moves_all_rows(gamespace)){
                    movement_base(gamespace);
                    number_generator(gamespace);
                }
                gamespace = rotate_gamespace_right(gamespace);
            }
        } else {
            ticksToInput--;
        }
        drawTick(renderer, gamespace, Sans);
        if (game_won(gamespace)){
            cout << "You won";
        }
        if(game_is_lost(gamespace)){
            game_is_running = false;
            cout << "You lost";
        }
    }
    SDL_Delay(5000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
