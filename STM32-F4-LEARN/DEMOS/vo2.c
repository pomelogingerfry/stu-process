#include<stdio.h>
#include<stdint.h>
typedef enum{
    ERR_NONE,
    ERR_OVERCURRENT,
    ERR_OVERVOLTAGE,
    ERR_ENCODERLOSS,
    ERR_OVERHEAT
}ERR_TYPE;
typedef struct{
    uint16_t fault_code;
    uint32_t time_ms;
    uint8_t fault_data[4];
}ERR_INFO;
#define FAULT_RECORD_SIZE 8U
typedef void(*ERRCB)(ERR_INFO* p);
static ERRCB cb=NULL;
static uint8_t used_cnt=0U;
static ERR_INFO REAL_MEDIUM[FAULT_RECORD_SIZE];
void register_callback(ERR_INFO *pb){
    cb=pb;
}
void add_record(uint16_t code,uint32_t times,const uint8_t *dat){
    if(used_cnt>=FAULT_RECORD_SIZE){
        return NULL;
    }
    ERR_INFO *p=&REAL_MEDIUM[used_cnt];
    p->fault_code=code;
    p->time_ms=times;
    uint8_t i=0;
    for(i=0;i<4U;i++){
        p->fault_data[i]=dat[i];
    }
    used_cnt++;
    if(cb!=NULL){
        cb(p);
    }
    return p;
}
uint8_t get_cnt(){
    return used_cnt;
}
int get_log(uint8_t index,ERR_INFO **ppout){
    if(ppout=NULL){
        return 0;
    }
    if(index>=used_cnt){
        return 0;
    }
    *ppout=&REAL_MEDIUM[index];
    return 1;
}
uint16_t extract_bits(const ERR_INFO *pc,uint8_t bit_start,uint8_t bit_len){
    if(pc==NULL||((bit_start+bit_len)>16U)){
        return 0;
    }
    uint16_t mask=(1U<<bit_len)-1U;
    return(pc->fault_code>>bit_start)&mask;
}
void mycallback(ERR_INFO *pd){
    (void)pd;
}
int main(){
    uint8_t d1={0x01,0x03,0x05,0x07};
    uint8_t d2={0x10,0x30,0x50,0x70};
    register_callback(mycallback);
    add_record(ERR_OVERCURRENT,1000U,d1);
    add_record(ERR_OVERHEAT,2000U,d2);
    get_cnt();
    ERR_INFO **p_log=NULL;
    get_log(1U,p_log);
    int ret=get_log(1U,p_log);
    if(ret==1){
        extract_bits(*p_log,2U,3U);
    }
    return 0;
}

