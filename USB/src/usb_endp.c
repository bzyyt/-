/**
 ******************************************************************************
 * @file    usb_endp.c
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Endpoint routines
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "control.h"
#include "GPIO_Init_Configuration.h"
#include "R_data.h"
#include "Delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL 5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Receive_Flag;     // �������ݳɹ���־λ
u16 Receive_Num;     // ���յ������ݸ���
u16 i;     // ѭ������
u16 temp1; // ��ʱ����
u16 temp2;

uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern uint8_t USART_Rx_Buffer[];
extern uint32_t USART_Rx_ptr_out;
extern uint32_t USART_Rx_length;
extern uint8_t USB_Tx_State;
extern void USB_Send_string(u8 *data_buffer, u8 N);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : EP1_IN_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP1_IN_Callback(void) // EP1_IN_Callback �ص�����
{
  uint16_t USB_Tx_ptr;
  uint16_t USB_Tx_length;

  if (USB_Tx_State == 1)
  {
    if (USART_Rx_length == 0)
    {
      USB_Tx_State = 0;
    }
    else
    {
      if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
      {
        USB_Tx_ptr = USART_Rx_ptr_out;
        USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

        USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
        USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;
      }
      else
      {
        USB_Tx_ptr = USART_Rx_ptr_out;
        USB_Tx_length = USART_Rx_length;

        USART_Rx_ptr_out += USART_Rx_length;
        USART_Rx_length = 0;
      }
      UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
      SetEPTxCount(ENDP1, USB_Tx_length);
      SetEPTxValid(ENDP1);
    }
  }
}

// ����ָ��
// 0000 -- 00 �ٿص�λ����DOWN
// 0011 -- 01 �ٿص�λ����UP
// 1100 -- 10 �ٿؿ��ư壬DOWN
// 1111 -- 11 �ٿؿ��ư壬UP
// 1122 -- 12 �������ư壬OPEN
// 1133 -- 13 �رտ��ư壬CLOSE
u32 Instruction_Decoding(void) // ָ����뺯��
{
  u32 returning = 0;
  //encording the first bit
  if (USB_Rx_Buffer[0] == (uint8_t)'0' && USB_Rx_Buffer[1] == (uint8_t)'0')
  {
    returning += 0; // Resistance 00 -- 0
  }
  else if (USB_Rx_Buffer[0] == (uint8_t)'1' && USB_Rx_Buffer[1] == (uint8_t)'1')
  {
      returning += 1; // LaserController 11 -- 1
  }
  else
  {
    return (u32)-1; // Error
  }

  //encording the second bit
  returning *= 10;
  if (USB_Rx_Buffer[2] == (uint8_t)'1' && USB_Rx_Buffer[3] == (uint8_t)'1')
  {
      returning += 1; // Up 11 -- 1
  }
  else if (USB_Rx_Buffer[2] == (uint8_t)'0' && USB_Rx_Buffer[3] == (uint8_t)'0')
  {
      returning += 0; // Down 00 -- 0
  }
  else if (USB_Rx_Buffer[2] == (uint8_t)'2' && USB_Rx_Buffer[3] == (uint8_t)'2')
  {
      returning += 2; // Open 22 -- 2
  }
  else if (USB_Rx_Buffer[2] == (uint8_t)'3' && USB_Rx_Buffer[3] == (uint8_t)'3')
  {
      returning += 3; // Close 33 -- 3
  }
  else
  {
      return (u32)-1; // Error
  }

  return returning;
}

void Resistance_Change(u16 target) // ��λ����������
{
  u16 i;
  if (target > 0 && target < 298 )//���Ʒ�Χ
  {

    for (i = 0; i < 100; i++)// ��λ������
    {
      DOWN_024();
      DOWN_135();
    }
    for (i = 0; i < r1[target]; i++)// ���ڵ����赵λ
    {
      UP_024();
    }
    for (i = 0; i < r2[target]; i++)
    {
      UP_135();
    }
  }
}

void Laser_Power_Change(u16 target) // ���ư��������
{
    u16 i;
    u16 temp;
    if (target > 0 && target < 256)//���Ʒ�Χ
    {
        PA_OFF(8); // ��߼�������1�����ʣ���������������ʵĻ���Ҫ����
        for (i = 0; i <= 7; i++)
        {
            temp = (target & 1);
            target >>= 1;//����λ
            if (temp)
            {
                PA_ON(i);
            }
            else
            {
                PA_OFF(i);
            }
        }
        PA_ON(8);
    }
}

u8 *Message_compile(u16 target, u8 *message) // ��ֵ���ַ���ת��
{
  u16 index = 0;
	u8 temp;
	u16 i;
  message[index] = target % 10 + '0';
  while ((u16)(target /= 10))
  {
    index += 1;
    message[index] = target % 10 + '0';
  }
  message[index + 1] = 0;
  for (i = 0; i < (index + 1) / 2; i++)//����
  {
      temp = message[i];
      message[i] = message[index - i];
      message[index - i] = temp;
  }
  return message;
}

u8 *Message_error(u8 *message) // ������Ϣ���
{
  message[0] = 'E';
  message[1] = 'r';
  message[2] = 'r';
  message[3] = 'o';
  message[4] = 'r';
  message[5] = 0;
  return message;
}

u8* Message_Info(u16 number, u8* message) // info����Ϣ���
{                                         // ���뻻һ�ַ�ʽ����Ҳ̫����
    if (number == 1)// �������ɹ�����
    {
        message[0] = 'L';
        message[1] = 'a';
        message[2] = 's';
        message[3] = 'e';
        message[4] = 'r';
        message[5] = ' ';
        message[6] = 'O';
        message[7] = 'N';
        message[8] = 0;
    }
    else if (number == 2)// �������ɹ��ر�
    {
        message[0] = 'L';
        message[1] = 'a';
        message[2] = 's';
        message[3] = 'e';
        message[4] = 'r';
        message[5] = ' ';
        message[6] = 'O';
        message[7] = 'F';
        message[8] = 'F';
        message[9] = 0;
    }
    return message;
}

u16 Message_len(u8 *message) // �ַ�������
{
  u16 len = 0;
  while (message[len] != 0)
  {
    len++;
    if (len >= 128)
    {
      break;
    }
  }
  return len;
}

/*******************************************************************************
 * Function Name  : EP3_OUT_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP3_OUT_Callback(void)
{
  uint16_t USB_Rx_Cnt;             // �������ݸ���
  uint8_t Message_send[128] = {0}; // ���ͻ�����

  /* Get the received data buffer and update the counter */
  USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);

  if (USB_Rx_Cnt != 0)
  {
    Receive_Flag = 1;         // ���ձ�־
    Receive_Num = USB_Rx_Cnt; // �������ݴ�С
  }
  else
    Receive_Flag = 0;
  /* USB data will be immediately processed, this allow next USB traffic being
  NAKed till the end of the USART Xfer */
  if (Receive_Flag)
  {
    switch (Instruction_Decoding())
    {
    case 1: // ������� -- 01
      if (R_index < 298)
      {
        R_index++;
        Message_compile(R_index, Message_send);
      }
      else
      {
          Message_error(Message_send);
      }
      break;

    case 0: // ��С���� -- 00
      if (R_index > 0)
      {
        R_index--;
        Message_compile(R_index, Message_send);
      }
      else
      {
          Message_error(Message_send);
      }
      break;
    
    case 10:// ��С���ư�
        if (Laser_Power > 0)
        {
            Laser_Power--;
            Message_compile(Laser_Power, Message_send);
        }
        else
        {
            Message_error(Message_send);
        }
        break;

    case 11:// ������ư�
        if (Laser_Power < 255)
        {
            Laser_Power++;
            Message_compile(Laser_Power, Message_send);
        }
        else
        {
            Message_error(Message_send);
        }
        break;

    case 12:                         // �򿪼�����
        PA_ON(9);                      // �ȴ�18��
        Delay_1ms(8);                  // ����˵����Ҫ�����ӳ�5ms
        PA_ON(10);                     // �ٴ�19��
        Message_Info(1, Message_send); // ���ɹ�����������
        break;

    case 13:                         // �رռ�����
        PA_OFF(10);                    // �ȹر�19��
        Delay_1ms(8);                  // ��߰˳�ҲҪ������ʱ5ms
        PA_OFF(9);                     // �ٹر�18��
        Message_Info(2, Message_send); // ���ɹ��رռ�����
        break;

    default: // �����������
      Message_error(Message_send);
    }
    Resistance_Change(R_index);// �ı���ֵ
    Laser_Power_Change(Laser_Power);// �ı���ư�
    USB_Send_string(Message_send, Message_len(Message_send)); // ������Ϣ����λ��
  }

  /* Enable the receive of data on EP3 */
  SetEPRxValid(ENDP3);
}

/*******************************************************************************
 * Function Name  : SOF_Callback / INTR_SOFINTR_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void SOF_Callback(void)
{
  static uint32_t FrameCount = 0;

  if (bDeviceState == CONFIGURED)
  {
    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
    {
      /* Reset the frame counter */
      FrameCount = 0;

      /* Check the data to be sent through IN pipe */
      Handle_USBAsynchXfer();
    }
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
