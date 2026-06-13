#include<stdio.h>
#include<stdint.h>
#define BUFFER_SIZE 8
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
}RingBuffer;
void RingBuffer_Init(RingBuffer *rb)
{
    rb-> head = 0;
    rb-> tail = 0;
    rb-> count = 0;
}
uint8_t RingBuffer_IsEmpty(RingBuffer *rb)
{
    return (rb->count == 0);
}
uint8_t RingBuffer_IsFull(RingBuffer *rb)
{
    return(rb->count == BUFFER_SIZE);
}
uint8_t RingBuffer_Count(RingBuffer *rb)
{
    return rb-> count;
}
int RingBuffer_write(RingBuffer *rb, uint8_t data)
{
    if(RingBuffer_IsFull(rb))
    return -1;
    rb->buffer[rb->head] = data;
    /* Faster than modulo because BUFFER_SIZE is power of 2*/
    rb->head = (rb->head + 1)&(BUFFER_SIZE - 1);
    rb->count++;
    return 0;
}
int RingBuffer_Read(RingBuffer *rb, uint8_t *data)
{
    if(RingBuffer_IsEmpty(rb))
    return -1;
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1)&(BUFFER_SIZE - 1);
    rb->count--;
    return 0;
}
int main(void)
{
    RingBuffer rb;
    uint8_t data;
    RingBuffer_Init(&rb);
    uint8_t writeData1[]=
    {
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48

    };
    for(int i = 0; i < 8; i++)
    {
        RingBuffer_write(&rb, writeData1[i]);
        printf("[WRITE] 0x%02X -> OK(count = %d)%s\n", writeData1[i],RingBuffer_Count(&rb), RingBuffer_IsFull(&rb)?"FULL":"");

    }
    if(RingBuffer_write(&rb, 0x99)!=0)
    {
        printf("[WRITE] 0x99 - > FAIL(buffer full)\n");
    }
    for(int i = 0; i < 3; i++)
    {
        RingBuffer_Read(&rb, &data);
        printf("[READ]->0x%02X(count = %d)\n",data, RingBuffer_Count(&rb));
    }
    uint8_t writeData2[]=
    {
        0x49, 0x4A, 0x4B
    };
    for(int i = 0; i < 3; i++)
    {
        RingBuffer_write(&rb, writeData2[i]);
        printf("[WRITE] 0x%02X-> OK(count = %d)\n",writeData2[i], RingBuffer_Count(&rb));
    }
    while(!RingBuffer_IsEmpty(&rb))
    {
        RingBuffer_Read(&rb, &data);
        printf("[READ]->0x%02X(count = %d)\n", data,RingBuffer_Count(&rb));
    }
    if(RingBuffer_Read(&rb, &data)!= 0)
    {
    printf("[READ](empty)-> FAIL(buffer empty)\n");
    }
    return 0;
}
