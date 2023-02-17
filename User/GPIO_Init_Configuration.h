//-----------------------------------------------------------------
// LED����ͷ�ļ�
// ͷ�ļ���: GPIO_Init_Configuration.h
// ��    ��:
// ��ʼ����: 2023-02-17
// �������: 2023-02-17
// �޸�����: 2023-02-17
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//   - V1.0: (2014-02-07) GPIO ����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
//-----------------------------------------------------------------

#ifndef _GPIO_INIT_CONFIGURATION_H
#define _GPIO_INIT_CONFIGURATION_H

//-----------------------------------------------------------------
// λ����
//-----------------------------------------------------------------

// �궨��
// �����λ�����ŵ�ƽ���ں���
#define PC0_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define PC0_ON GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define PC1_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define PC1_ON GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define PC2_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define PC2_ON GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define PC3_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define PC3_ON GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define PC4_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define PC4_ON GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define PC5_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define PC5_ON GPIO_SetBits(GPIOC, GPIO_Pin_5)

// ������ƿ����ŵ�ƽ���ں���
#define PA_OFF(x)                           \
    switch (x)                              \
    {                                       \
    case 0:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);  \
        break;                              \
    case 1:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);  \
        break;                              \
    case 2:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);  \
        break;                              \
    case 3:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_3);  \
        break;                              \
    case 4:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);  \
        break;                              \
    case 5:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);  \
        break;                              \
    case 6:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);  \
        break;                              \
    case 7:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_7);  \
        break;                              \
    case 8:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);  \
        break;                              \
    case 9:                                 \
        GPIO_ResetBits(GPIOA, GPIO_Pin_9);  \
        break;                              \
    case 10:                                \
        GPIO_ResetBits(GPIOA, GPIO_Pin_10); \
        break;                              \
    }
#define PA_ON(x)                          \
    switch (x)                            \
    {                                     \
    case 0:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_0);  \
        break;                            \
    case 1:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_1);  \
        break;                            \
    case 2:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_2);  \
        break;                            \
    case 3:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_3);  \
        break;                            \
    case 4:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_4);  \
        break;                            \
    case 5:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_5);  \
        break;                            \
    case 6:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_6);  \
        break;                            \
    case 7:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_7);  \
        break;                            \
    case 8:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_8);  \
        break;                            \
    case 9:                               \
        GPIO_SetBits(GPIOA, GPIO_Pin_9);  \
        break;                            \
    case 10:                              \
        GPIO_SetBits(GPIOA, GPIO_Pin_10); \
        break;                            \
    }
//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
void GPIO_Init_Configuration(void);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
