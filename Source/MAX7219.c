#include "MAX7219.h"

const char TX_CHAR1[CHAR_NUMBER] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '-', '.'};
const unsigned int TX_CODE1[CHAR_NUMBER] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x01, 0x80};

unsigned int SPI_send_char(char char_sign, unsigned int char_position)
{
  unsigned int Result_Char = 0;

  for (int i = 0; i < CHAR_NUMBER; i++)
  {
    if (TX_CHAR1[i] == char_sign)
    {
      Result_Char |= (TX_CODE1[i] << 0);
      break;
    }
  }
  char_position += 1;
	Result_Char |= (char_position << 8);
	return Result_Char;
}

void SPI_send_date_time(char* data)
{
	unsigned int TX_Data;
	unsigned int char_sign = 0;
	unsigned int char_position = 0;

	while(data[char_sign] != '\0')
	{
		if(char_position > LED_POSITION)
			char_position = 0;

		while(!(LPSPI1->SR & (1<<0)));

		if(data[char_sign] != '.')
		{
			TX_Data = SPI_send_char(data[char_sign], LED_POSITION - char_position);
			LPSPI1->TDR = TX_Data;
			char_sign++;
			char_position++;
		}
		else
		{
      TX_Data = SPI_send_char(data[char_sign - 1], LED_POSITION - char_position + 1);
      TX_Data |= (1 << 7);
      LPSPI1->TDR = TX_Data;
      char_sign++;
		}
	}
}
