#include "song.h"


//private variable
SongInfo thissong = {INIT_BROTHERJOHN_PITCH,INIT_BROTHERJOHN_LENGTH};
PSongInfo  brotherJohn=&thissong;


Tone toneContainer[SONG_LENGTH];

void add_tone(Tone *song,PTone newTone){
	PTone prevTone = NULL;
	PTone thisTone = song;
	
	while(thisTone){
		prevTone = thisTone;
		thisTone =  thisTone->nextTone;
	}
	
	if(prevTone == NULL){
		song = newTone;
	}else{
		prevTone->nextTone = newTone;
	}

}  //private function


void load_tone(Tone *song,int unused){
	int toneID=0;
	PTone newTone;
	for(toneID;toneID<SONG_LENGTH;toneID++){
		//create every tone element
		newTone = &toneContainer[toneID]; 
		//compiler cannot understand 'toneContainer+offset'
		newTone -> nextTone = NULL;
		newTone -> pitchID = *(brotherJohn->brotherJohnPitch+toneID);
		newTone -> lengthID = *(brotherJohn->brotherJohnLength+toneID);
		//insert tone element into song
		add_tone(song,newTone);
	}
	
	newTone -> nextTone = song; //create a circular list

}  //public function

Tone *pop_next_tone(Tone *song,Tone *preTone){
	if(preTone==NULL){
		return song;
	}else{
		Tone *pre;
		pre = (Tone *)preTone;
		return pre -> nextTone;
	}

} //public function



