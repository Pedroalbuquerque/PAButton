#ifndef BUTTON_H
#define BUTTON_H

#include <DebounceEvent.h>
#include <DebugTools.h>
// Calibration button defines and functions

#define SETTLINGTIME 50
#define REPEATDELAY 500
#define LONGDELAY 1000

typedef enum  { NONEclick , ONEclick , TWOclick, THREEclick, LONGclick, } BTclick_t;


class PAButton : public DebounceEvent{ 
public:	
	uint8_t check();
	PAButton(uint8_t pin,uint8_t mode);
	PAButton(uint8_t pin);
	void setBTevent( uint8_t event, uint8_t count, uint16_t length);
	BTclick_t BTclick;
	
	void keys(IO_t* IO,char* BTeventKey);
protected:
    IO_t* _IO = NULL;   // global pointer to IO object
    int8_t _BTeventIdx = -1;  // Index for IO object parameters (-1 => not assigned)

};

PAButton::PAButton(uint8_t pin):DebounceEvent(pin){};

PAButton::PAButton(uint8_t pin,uint8_t mode):DebounceEvent(pin, mode){
}
void PAButton::keys(IO_t* IO,char* BTeventKey){
  _IO = IO;
  _BTeventIdx = _IO->index(BTeventKey);
}

uint8_t PAButton::check(){

	uint8_t event;
	event = DebounceEvent::loop();
	if(event){
		setBTevent( event,getEventCount(), getEventLength());
		//in case an IO object is associated, store value in the IO 
		if(_IO != NULL){
			_IO->set(_BTeventIdx,BTclick);
			_IO->newData = true;
			DEBUG_MSG("_IO.newdata:%d\n",_IO->newData);
		}
		if(BTclick == 0){
			DEBUG_MSG("debounce event:%d\t",event);
		}
		return true;
	}
	return false;

}
void PAButton::setBTevent(uint8_t event, uint8_t count, uint16_t length) {
		
	if(event == EVENT_RELEASED){
	
		if(length > LONGDELAY)
			BTclick = LONGclick;
		else{
			switch(count){
				case 1:
					BTclick = ONEclick;
					break;
				case 2:
					BTclick = TWOclick;
					break;
				case 3:
					BTclick = THREEclick;
					break;
			}
		}        
	} else {
		BTclick = NONEclick;
	}
	
	//DEBUG_MSG("Event : %d\t",event);
	//DEBUG_MSG(" Clicks : %d\t",count);
	//DEBUG_MSG(" Length: %d\n",length);
}









#endif