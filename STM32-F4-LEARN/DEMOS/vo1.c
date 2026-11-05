#include<stdio.h>
#include<stdint.h>
#define RINGBUF_SIZE 256
typedef struct{
    uint8_t buf[RINGBUF_SIZE];
    volatile uint16_t wr;
    volatile uint16_t rd;
    void(*fullwarn)(void);
}RINGBUF;
void RINGBUF_INIT(RINGBUF *ring,void(*almostfull)(void)){
    ring->wr=0;
    ring->rd=0;
    ring->fullwarn=almostfull;
    printf("初始化完成\r\n");
}
uint8_t RINGBUF_ISEMPTY(RINGBUF *ring){
    return (ring->wr==ring->rd);
}
uint8_t RINGBUF_ISFULL(RINGBUF *ring){
    if((ring->wr+1)%RINGBUF_SIZE==ring->rd){
        return 1;
    }
    else{
        if((ring->wr+2)%RINGBUF_SIZE==ring->rd){
            if(ring->fullwarn!=NULL){
            ring->fullwarn();
        }
        }
        return 0;
    }
}
void WARN(void){
    printf("缓冲区还剩一个可用字节\r\n");
}
uint8_t RINGBUF_READ(RINGBUF *ring,uint8_t *outbyte){
    if(RINGBUF_ISEMPTY(ring)==1){
        return 0;
    }
    *outbyte=ring->buf[ring->rd];
    ring->rd=(ring->rd+1)%RINGBUF_SIZE;
    return 1;
}
uint8_t RINGBUF_WRITE(RINGBUF *ring,uint8_t byte){
    if(RINGBUF_ISFULL(ring)==1){
        return 0;
    }
    ring->buf[ring->wr]=byte;
    ring->wr=(ring->wr+1)%RINGBUF_SIZE;
    return 1;
}
uint16_t RINGBUF_READMULTI(RINGBUF *ring,uint8_t *dsk,int len){
    int i=0;
    for(i=0;i<len;i++){
        RINGBUF_READ(ring,&dsk[i]);
        if(RINBUF_READ(ring,&dsk[i])==0){
            break;
        }
    }
    return i;
}
uint16_t RINGBUF_WRITEMULTI(RINGBUF *ring,uint8_t *str,int len){
    int i=0;
    for(i=0;i<len;i++){
        if(RINGBUF_WRITE(ring,str[i])==0){
            break;
        }
    }
    return i;
}
RINGBUF BUF;
uint8_t idata;
extern UART_HandleTypeDef huart1;
void USART1_IRQHandler(void){
    HAL_UART_IRQHandler(&huart1);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart==&huart1){
        uint8_t cn=idata;
        RINGBUF_WRITE(&BUF,cn);
    }
    HAL_UART_Receive_IT(&huart1,&idata,1);
int main(){
    HAL_Init();
    SystemClock_CONfig();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    RINGBUF_INIT(&BUF,fullwarn);
    HAL_UART_Receive_IT(&huart1,&idata,1);
}