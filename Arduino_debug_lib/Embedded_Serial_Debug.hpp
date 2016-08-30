#ifndef ESERIALDEBUG
#define ESERIALDEBUG
//constant expression converting level symbol pair to a single byte
#include <Arduino.h>

union unfloat{
	float value;
	char a[4];
};

union unint{
	int value;
	char a[2];
};

union unlong{
	int value;
	char a[4];
};

class SerialDebug {
public:
	SerialDebug() :
		auto_output_(0), wHead_(0), rHead_(0), point_(NULL), buf_(NULL), remain_(0), Size_(0) {}
	~SerialDebug(){
		delete buf_;
	}
	void begin(uint16_t Size){
		if (buf_ == NULL){
			buf_ = new char[Size];
		}
		else{
			delete buf_;
			buf_ = new char[Size];
		}
		point_ = buf_;
		Size_ = Size;
		wHead_ = 0;
		rHead_ = 0;
	}
	void write(uint8_t level, char symbol, unsigned long time){
		write(level, symbol, time, "");
	}
	void write(uint8_t level, char symbol, unsigned long time, const char* vtypes, ...){
		//Create a string stream
		String wout;
		int out_size = 0;
		char out_array[6] = { 0 };

		out_array[0] += ((level << 5) | (symbol & 0b00011111));
		wout += hex(out_array[0]);
		push(wout.c_str(), wout.length());
		wout.remove(0);
		unlong t;
		t.value = time;
		wout += hex(t.a[0]);//Send as each byte address
		wout += hex(t.a[1]);
		wout += hex(t.a[2]);
		wout += hex(t.a[3]);
		out_size = 8;
		push(wout.c_str(), wout.length());
		wout.remove(0);
		va_list vlist;
		va_start(vlist, vtypes);
		unfloat x;
		unint i;
		while (*vtypes != '\0'){
			switch (*vtypes){
			case 'F':
			case 'f':
				wout += hex('F');
				x.value = float(va_arg(vlist, double));
				wout += hex(x.a[0]);//Send as each byte address
				wout += hex(x.a[1]);
				wout += hex(x.a[2]);
				wout += hex(x.a[3]);

				push(wout.c_str(), wout.length());
				wout.remove(0);
				break;

			case 'i':
			case 'I':
				wout += hex('i');

				i.value = va_arg(vlist, int);
				wout += hex(i.a[0]);//Send as each byte address
				wout += hex(i.a[1]);

				push(wout.c_str(), wout.length());
				wout.remove(0);
				break;

			case 'B':
			case 'b':
				wout += hex('b');
				wout += hex(uint8_t(va_arg(vlist, int)));

				push(wout.c_str(), wout.length());
				wout.remove(0);
				break;


			case 'C':
			case 'c':
				wout += hex('c');
				wout += hex((char)(va_arg(vlist, int)));

				push(wout.c_str(), wout.length());
				wout.remove(0);
				break;
			}
			vtypes += 1;
		}
		va_end(vlist);

		push('\n');


	}


	void pop(){
		pop(remain());
	}
	void pop(uint16_t x){
		String sout;

		//char* s;
		if (remain() <= x){
			//s = new char[x + 1]();
			//s[x+1] = '\n';
			//Temporary version out to stdio
			for (unsigned int i = 0; i <= x; i += 1){
				rHead_ %= Size_;
				sout += *(buf_ + rHead_);
				rHead_++;
				remain_--;
			}
		}
		else {
			//s = new char[remain() + 1]();
			//s[remain()+1] = '\n';
			for (unsigned int i = 0; i < remain(); i += 1){
				rHead_ %= Size_;
				sout += *(buf_ + rHead_);
				rHead_++;
				remain_--;
			}

			//delete s;
		}

		Serial.print(sout);
		//Free the heap

	}


	void aout_off(){
		auto_output_ = 0;
	}
	void aout_on(){
		auto_output_ = 1;
	}

	//Tell me how much is in the buffer
	uint16_t remain() const {
		return remain_;
	}
	//Tell me how much free space is in in the buffer
	uint16_t free() const {
		return Size_ - remain_;
	}
private:
	void freespace(uint16_t x){
		pop(x);
	}

	void push(char value){
		if (free() <= 0){
			freespace(1);
		}
		wHead_ %= Size_;
		point_[wHead_] = value;
		wHead_++;
		remain_++;
	}
	void push(const char* value, uint16_t Length){
		if (free()<Length){
			freespace(Length);
		}
		for (unsigned int i = 0; i<Length; i += 1){
			wHead_ %= Size_;
			*(buf_ + wHead_) = value[i];
			wHead_++;
			remain_++;
		}
	}

	String hex(byte x){
		String hout;
		if (x < 16){ hout += '0'; }
		hout += String(x, HEX);
		return hout;
	}



	// instance variables
	bool auto_output_;

	//Ring pointers
	int wHead_;
	int rHead_;
	char* point_;
	// Assigned space pointer
	char* buf_;
	//Amount remaining in buffer
	uint16_t remain_;

	uint16_t Size_;
};
#endif

