//-----------------------------------------------------------------
// ��������:
//     GPIO��������
// ��    ��:
// ��ʼ����: 2022-12-28
// �������: 2022-12-28
// �޸�����: 2022-12-28
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//  - V1.0: (2014-02-07)LED IO ����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
// ���Թ���: ����STM32���Ŀ����塢LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "GPIO_Init_Configuration.h"

//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
// ��������: GPIO����
// ��ڲ���: ��
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ����ģ��: RCC_APB2PeriphClockCmd();GPIO_Init();
// ע������: ��
//
//-----------------------------------------------------------------
void GPIO_Init_Configuration(void)
{
  int i = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructureLight; // ����ģ�� ���������A��

  // ʹ��IO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  // GPIOC��0|1|2|3|4|5��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // �������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  PC0_ON;
  PC1_ON;
  PC2_ON;
  PC3_ON;
  PC4_ON;
  PC5_ON;
  // ��ʼ��GPIO��

  // GPIOA��0~7�ڴ�
  GPIO_InitStructureLight.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructureLight.GPIO_Speed = GPIO_Speed_50MHz;
  // �������
  GPIO_InitStructureLight.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructureLight);
  // ��ʼ��GPIO��
  for (i = 0; i <= 10; i++)
  {
    PA_OFF(i);
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
