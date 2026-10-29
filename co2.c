#include<stdio.h>
#include<stdint.h>
#define Ring_Buf_Size 256

typedef struct{
    uint8_t buf[Ring_Buf_Size];
    uint16_t write_index;
    uint16_t read_index;
}RingBuf_t;

void RingBuf_Init(RingBuf_t *ring){
    ring->write_index=0;
    ring->read_index=0;
}

void RingBuf_Clear(RingBuf_t *ring){
    ring->write_index=0;
    ring->read_index=0;
}

uint8_t RingBuf_isEmpty(RingBuf_t *ring){
    if(ring->read_index == ring->write_index){
        return 1;
    }
    else{
        return 0;
    }
}

uint8_t RingBuf_isFull(RingBuf_t *ring){
    if((ring->write_index+1)%Ring_Buf_Size==ring->read_index){
        return 1;
    }
    else{
        return 0;
    }
}

uint8_t RingBuf_Write_Byte(RingBuf_t*ring,uint8_t byte){
    if(RingBuf_isFull(ring)){
        return 0;
    }
    ring->buf[ring->write_index]=byte;
    ring->write_index=(ring->write_index+1)%Ring_Buf_Size;
    return 1;
}

uint8_t RingBuf_Read_Byte(RingBuf_t *ring,uint8_t *out_byte){
    if (RingBuf_isEmpty(ring)){
        return 0;
    }
    *out_byte=ring->buf[ring->read_index];
    ring->read_index=(ring->read_index+1)%Ring_Buf_Size;
    return 1;
}

uint16_t RingBuf_t_GetLen(RingBuf_t *ring){
    if(ring->write_index >= ring->read_index){
        return ring->write_index - ring->read_index;
    }
    return (Ring_Buf_Size - ring->read_index) + ring->write_index;
}

uint16_t RingBuf_ReadMulti(RingBuf_t *ring,uint8_t *dsk,uint16_t len){
    uint16_t i;
    for(i=0;i<len;i++){
        if(RingBuf_Read_Byte(ring,&dsk[i])==0){
            break;
        }
    }
    return i;
}

uint16_t RingBuf_WriteMulti(RingBuf_t *ring,const uint8_t *str,uint16_t len){
    uint16_t i;
    for(i=0;i<len;i++){
        if(RingBuf_Write_Byte(ring,str[i])==0){
            break;
        }
    }
    return i;
}

int main(){
    RingBuf_t uuu_t;
    uint8_t temp;
    uint8_t read_buf[10];
    uint8_t write_buf[5]={0x11,0x22,0x33,0x44,0x55};
    uint16_t cn;
    RingBuf_Init(&uuu_t);
    printf("初始化完成,进入单字节写入测试\r\n");
    RingBuf_Write_Byte(&uuu_t,write_buf[0]);
    RingBuf_Write_Byte(&uuu_t,write_buf[1]);
    RingBuf_Write_Byte(&uuu_t,write_buf[2]);
    printf("现有缓冲区长度为%d\r\n",RingBuf_t_GetLen(&uuu_t));
    if(RingBuf_isEmpty(&uuu_t)==1){
        printf("缓冲区为空\r\n");
    }
    else{
        printf("缓冲区有数据\r\n");
    }
    if(RingBuf_isFull(&uuu_t)==1){
        printf("缓冲区已满\r\n");
    }
    else{
        printf("缓冲区未满\r\n");
    }
    printf("单字节循环读取测试：\r\n");
    while(RingBuf_Read_Byte(&uuu_t,&temp)==1){
        printf("0x%02X\r\n",temp);
    }
    printf("读完后缓冲区有效字节长度为：%d\r\n",RingBuf_t_GetLen(&uuu_t));
    RingBuf_Clear(&uuu_t);
    printf("接下来是批量写入测试\r\n");
    cn=RingBuf_WriteMulti(&uuu_t,write_buf,5);
    printf("写入成功字节数为%d\r\n",cn);
    printf("写入成功后缓冲区长度为%d\r\n",RingBuf_t_GetLen(&uuu_t));
    printf("接下来是批量读取测试\r\n");
    cn=RingBuf_ReadMulti(&uuu_t,read_buf,10);
    printf("批量读取字节数为%d\r\n",cn);
    for(uint16_t i=0;i<cn;i++){
        printf("0x%02X\r\n",read_buf[i]);
    }
    printf("运行结束利用Clear清除缓冲\r\n");
    RingBuf_Clear(&uuu_t);
    printf("清空后缓冲区长度为%d\r\n",RingBuf_t_GetLen(&uuu_t));
    getchar();
    return 0;
}
