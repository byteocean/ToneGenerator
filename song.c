#include "song.h"



void add_tone(Tone *song,PTone tone){
	PTone prevTone = NULL;
	PTone thisTone = song;
	
	while(thisTone){
		prevTone = thisTone;
		thisTone =  thisTone->nextTone;
	}
	
	if(prevTone == NULL){
		song = tone;
	}else{
		prevTone->nextTone = tone;
	}

}  //private function


void load_tone(Tone *song,int key){
	int toneID=0;
	PTone currentTone;
	for(toneID;toneID<SONG_LENGTH;toneID++){
		//create every tone element
		currentTone = &toneContainer[toneID]; 
		//compiler cannot understand 'toneContainer+offset'
		currentTone -> nextTone = NULL;
		currentTone -> pitch = *(
				brotherJohn->pitchSet+
				(*(brotherJohn->brotherJohnPitch+toneID)+10+key)
				);
			
		currentTone -> length = *(
				brotherJohn->lengthSet+
				(*(brotherJohn->brotherJohnLength+toneID)+10)
				);
		//insert tone element into song
		add_tone(song,currentTone);
	}
	
	currentTone -> nextTone = song; //create a circular list

}  //public function



PTone display_tone(Tone *song,PTone preTone){
	if(preTone==NULL){
		return song;
	}else{
		PTone pre =(PTone)preTone;
		return pre -> nextTone;
	}

}
  //public function



