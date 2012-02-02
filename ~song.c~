#include "song.h"

PTone create_tone(Tone *song,int toneID,int key){
	PTone tone;
	//tone=(PTone)malloc(sizeof(Tone)); 
	//malloc is not supported,there is error 'undefined __outchar'
	//and 'undefined __tstchar'
	tone -> nextTone = NULL;
	tone -> pitch = *(
			brotherJohn->pitchSet+
			(*(brotherJohn->brotherJohnPitch+toneID)+10+key)
			);
			
	tone -> length = *(
			brotherJohn->lengthSet+
			(*(brotherJohn->brotherJohnLength+toneID)+10+key)
			);

	return tone;
	
} //private function

void load_tone(Tone *song,int key){
	int i=0;
	PTone prevTone = song;
	for (i;i<SONG_LENGTH;i++){
		PTone newTone;// = (PTone)malloc(sizeof(Tone));
		newTone = create_tone(song,i,key);
		
		if(prevTone == NULL){
			song = newTone;
		}else{
			prevTone -> nextTone = newTone;
			prevTone = newTone;		
		}
		
	}

	prevTone -> nextTone = song; //create a circular lists

}   //public function




PTone display_tone(Tone *song,PTone preTone){
	if(preTone==NULL){
		return song;
	}else{
		PTone pre =(PTone)preTone;
		return pre -> nextTone;
	}

}
  //public function



