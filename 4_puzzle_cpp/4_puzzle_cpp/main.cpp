//
//  main.cpp
//  4_puzzle_cpp
//
//  Created by line.ash on 2021/03/28.
//

#include <iostream>
#include <stdio.h>
#include <bangtal.h>
#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID game_board[25], original_board[25];
ObjectID start;
TimerID timer, timer1, timer2;

bool game = false;
int blank, mixCount;

int indexToX(int i){
    return 210 + 206 * ( i % 5 );
}

int indexToY(int i){
    return 725 - 163 * ( i / 5 );
}

int game_index(ObjectID object){
    
    for(int i = 0; i < 25 ; i++)
    {
        if(game_board[i] == object) return i;
    }
    return -1; //error, invalid object
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true){
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);
    if(shown) showObject(object);
    
    return object;
    
}

int random_move(){
    
    int i = -1; //not determined
    
    while(i == -1){
        
        switch (rand()%4) {
            case 0:if(blank % 5 > 0)i = blank - 1;
                break;
            case 1:if(blank % 5 < 4)i = blank + 1;
                break;
            case 2:if(blank / 5 > 0)i = blank - 5;
                break;
            case 3:if(blank / 5 < 4)i = blank + 5;
                break;
        }
    }
    return i;
}
void game_move(int i){
    //인덱스와 blank object 교환
    ObjectID object = game_board[i];
    game_board[i] = game_board[blank];
    game_board[blank] = object;
}


void change(ObjectID object){
    int i = game_index(object);    //object 배열 인덱스 구함
    game_move(i);    //인덱스와 blank object 교환
    locateObject(game_board[i], scene, indexToX(i), indexToY(i));
    locateObject(game_board[blank], scene, indexToX(blank), indexToY(blank));
    blank = i;
}

void game_start(){
    blank = rand()%25;
    hideObject(game_board[blank]);
    mixCount = 500;
    setTimer(timer, 0.1f);
    startTimer(timer);
    }

bool game_end(){
    
    for ( int i = 0; i<25; i++) {
        if(game_board[i] != original_board[i]) return false;
    }
    return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action){
    
    if(game){
        if(action == MouseAction::MOUSE_DRAG_UP && object == game_board[blank+5]) change(object);
        if(action == MouseAction::MOUSE_DRAG_DOWN && object == game_board[blank-5]) change(object);
        if(action == MouseAction::MOUSE_DRAG_LEFT && object == game_board[blank+1]) change(object);
        if(action == MouseAction::MOUSE_DRAG_RIGHT && object == game_board[blank -1]) change(object);
        if(game_end()){
            game = false;
            showObject(start);
            showObject(game_board[blank]);
            showMessage("Completed!!");
            
        }
    }else if(object == start)
    {
        game_start();
    }
}
/*
void shuffle(){
    
    int i = 0;
    while(i< 100){
        int a = rand()%25;
        int b = rand()%25;

        ObjectID temp = game_board[a];
        game_board[a] = game_board[b];
        game_board[b] = temp;
        locateObject(game_board[a], scene, indexToX(a), indexToY(a));
        locateObject(game_board[b], scene, indexToX(b), indexToY(b));
        i++;
    }

}*/

/*void countCallback(TimerID timer){
    setTimer(timer2, 1);
    startTimer(timer2);
    increaseTimer(timer1, 1);
}*/

void timerCallback(TimerID timer){
    mixCount--;
    if(mixCount>0){
        change(game_board[random_move()]);
        setTimer(timer, 0.01f);
        startTimer(timer);
    }else{
        game = true;
        hideObject(start);
    }
}
SceneID game_init(){
    
    scene = createScene("background", "cut.png");
    
    char buf[50];
    for( int i = 0 ; i < 25 ; i++){
        
        sprintf(buf, "%d.png", i+1);
        game_board[i] = createObject(buf);
        original_board[i] = game_board[i];
        locateObject(game_board[i], scene, indexToX(i), indexToY(i));
        showObject(game_board[i]);
        
    }
    
    //shuffle();
    start = createObject("start.png", scene, 680, 300);
    scaleObject(start, 0.3);
    //scaleObject(start, 50);
    //hideObject(game_board[blank]);
    
    timer = createTimer(0.1f);
    
    return scene;
    
}


int main() {
    
    srand((int)time(NULL));
    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);
    //setTimerCallback(countCallback);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    //showTimer(timer1);
    startGame(game_init());
    
}
