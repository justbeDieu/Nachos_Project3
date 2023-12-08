#ifndef STABLE_H  
#define STABLE_H  
#include "synch.h"  
#include "bitmap.h"  
#define MAX_LOCK 10  

class Semaphore {
private: 
    char name[50]; 
    Semaphore *sem; // Tạo Semaphore để quản lý 
public: 
    // khởi tạo đối tượng Sem. Gán giá trị ban đầu là null 
    // nhớ khởi tạo bm sử dụng 
    Sem(char* na, int i){ 
        strcpy(this->name,na); 
        sem = new Semaphore(name,i); 
    } 
    ~Sem(){                  // hủy các đối tượng đã tạo 
        delete sem; 
    } 
    void wait(){             // thực hiện thao tác chờ 
        sem->P(); 
    } 
    void signal(){           // thực hiện thao tác giải phóng Semaphore 
        sem->V(); 
    } 
    char* GetName(){         // Trả về tên của Semaphore 
        return name; 
    }
}

class STable {   
private:  
    BitMap* bm;  
    Lock* lockTab[MAX_SEMAPHORE];   
  public:  
    STable();
    ~STable();    
    int create(char* name);    
    int wait(char *name);    
    int signal(char *name);  
}; 
#endif
