#pragma   once  
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<Windows.h>
#pragma comment(lib,"Winmm.lib")//“Ù¿÷ø‚
//±≥æ∞“Ù¿÷
void Ground_MUSIC() {
	mciSendString("open easy_disco.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("play mymusic repeat", NULL, 0, NULL);
}
void Check_MUSIC() {	//µ„ª˜ø®∆¨µƒ“Ù¿÷
	mciSendString("open Eliminate.mp3 alias mymusic_ELI", NULL, 0, NULL);
	mciSendString("play mymusic_ELI from 0", NULL, 0, NULL);
}
void ELIM_MUSIC() {	//œ˚≥˝ø®∆¨µƒ“Ù¿÷
	mciSendString("open shield_hit_wood_wood_2.mp3 alias mymusic_check", NULL, 0, NULL);
	mciSendString("play mymusic_check from 0", NULL, 0, NULL);

}

void Close_Ground_MUSIC() {
	mciSendString("stop easy_disco.mp3", NULL, 0, NULL);
}
