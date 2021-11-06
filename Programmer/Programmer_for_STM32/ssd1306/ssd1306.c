
#include "ssd1306.h"
#include "FreeRTOS.h"
#include "task.h"



// Screenbuffer
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Screen object
static SSD1306_t SSD1306;


//
//  Send a byte to the command register
//
static SSD1306_Status ssd1306_WriteCommand(uint8_t command)
{
	if (HAL_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR,0x00,1,&command,1,10)== HAL_OK){
	return SSD1306_OK;
	}
	else{
	return SSD1306_ERROR;
	}
}


//
//	Initialize the oled screen
//
uint8_t ssd1306_Init(void)
{	
	SSD1306_Status init_Status = SSD1306_OK;
	// Wait for the screen to boot
	HAL_Delay(100);
	
	/* Init LCD */
	init_Status = ssd1306_WriteCommand(0xAE); //display off
	init_Status = ssd1306_WriteCommand(0x20); //Set Memory Addressing Mode   
	init_Status = ssd1306_WriteCommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	init_Status = ssd1306_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	init_Status = ssd1306_WriteCommand(0xC8); //Set COM Output Scan Direction
	init_Status = ssd1306_WriteCommand(0x00); //---set low column address
	init_Status = ssd1306_WriteCommand(0x10); //---set high column address
	init_Status = ssd1306_WriteCommand(0x40); //--set start line address
	init_Status = ssd1306_WriteCommand(0x81); //--set contrast control register
	init_Status = ssd1306_WriteCommand(0xFF);
	init_Status = ssd1306_WriteCommand(0xA1); //--set segment re-map 0 to 127
	init_Status = ssd1306_WriteCommand(0xA6); //--set normal display
	init_Status = ssd1306_WriteCommand(0xA8); //--set multiplex ratio(1 to 64)
	init_Status = ssd1306_WriteCommand(0x3F); //
	init_Status = ssd1306_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	init_Status = ssd1306_WriteCommand(0xD3); //-set display offset
	init_Status = ssd1306_WriteCommand(0x00); //-not offset
	init_Status = ssd1306_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
	init_Status = ssd1306_WriteCommand(0xF0); //--set divide ratio
	init_Status = ssd1306_WriteCommand(0xD9); //--set pre-charge period
	init_Status = ssd1306_WriteCommand(0x22); //
	init_Status = ssd1306_WriteCommand(0xDA); //--set com pins hardware configuration
	init_Status = ssd1306_WriteCommand(0x12);
	init_Status = ssd1306_WriteCommand(0xDB); //--set vcomh
	init_Status = ssd1306_WriteCommand(0x20); //0x20,0.77xVcc
	init_Status = ssd1306_WriteCommand(0x8D); //--set DC-DC enable
	init_Status = ssd1306_WriteCommand(0x14); //
	init_Status = ssd1306_WriteCommand(0xAF); //--turn on SSD1306 panel
	
	// Clear screen
	ssd1306_Fill(Black);
	
	// Flush buffer to screen
	ssd1306_UpdateScreen();
	
	// Set default values for screen object
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	SSD1306.Initialized = 1;
	
	if (init_Status){
	return SSD1306_OK;
	}
	else{
	return SSD1306_ERROR;
	}
}

void ssd1306_on(void)
	{
ssd1306_WriteCommand(0xAF);
}
	
void ssd1306_off(void)
	{
ssd1306_WriteCommand(0xAE); 
}


//
//  Fill the whole screen with the given color
//
void ssd1306_Fill(SSD1306_COLOR color) 
{
	/* Set memory */
	uint32_t i;

	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}



//void ssd1306_Fill1(SSD1306_COLOR color) 
//{
//	/* Set memory */
//	uint32_t i;

//	for(i = 777; i < 1024; i++)
//	{
//		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
//	}
//}

//
//  Write the screenbuffer with changed to the screen
//
SSD1306_Status ssd1306_UpdateScreen(void) 
{
	
	taskENTER_CRITICAL();
	SSD1306_Status UpdateScreen_Status = SSD1306_OK;
	uint8_t i;
	
	for (i = 0; i < 8; i++) {
		UpdateScreen_Status = ssd1306_WriteCommand(0xB0 + i);
		UpdateScreen_Status = ssd1306_WriteCommand(0x00);
		UpdateScreen_Status = ssd1306_WriteCommand(0x10);

		if (HAL_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x40, 1, &SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, 2000) == HAL_OK){//!!! 25.03.2021
		UpdateScreen_Status = SSD1306_OK;
		}
		else {
		UpdateScreen_Status = SSD1306_ERROR;
		}
	}
	taskEXIT_CRITICAL();
	if (UpdateScreen_Status){
	return SSD1306_OK;
	}
	else{
	return SSD1306_ERROR;
	}
}

//
//	Draw one pixel in the screenbuffer
//	X => X Coordinate
//	Y => Y Coordinate
//	color => Pixel color
//
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		// Don't write outside the buffer
		return;
	}
	
	// Check if pixel should be inverted
	if (SSD1306.Inverted) 
	{
		color = (SSD1306_COLOR)!color;
	}
	
	// Draw in the right color
	if (color == White)
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} 
	else 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

//
//  Draw 1 char to the screen buffer
//	ch 		=> char om weg te schrijven
//	Font 	=> Font waarmee we gaan schrijven
//	color 	=> Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
	uint32_t i, b, j;
	
	// Check remaining space on current line
	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}
	
	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			} 
			else 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}
	
	// The current space is now taken
	SSD1306.CurrentX += Font.FontWidth;
	
	// Return written char for validation
	return ch;
}



char ssd1306_WriteCharInverted(char ch, FontDef Font, SSD1306_COLOR color)
{
	uint32_t i, b, j;
	
	// Check remaining space on current line
	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}
	
	
	
	for(uint32_t h=0; h < Font.FontHeight; h++)
  {
		for(uint32_t w=0; w < Font.FontWidth; w++)
  {
		
	ssd1306_DrawPixel(SSD1306.CurrentX + w, (SSD1306.CurrentY + h), (SSD1306_COLOR)!color);
		
  }		
  }
	
	
	
	
	
	
	
	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000) 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			} 
			else 
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}
	
	// The current space is now taken
	SSD1306.CurrentX += Font.FontWidth;
	
	// Return written char for validation
	return ch;
}



//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
	// Write until null-byte
	while (*str) 
	{
		if (ssd1306_WriteChar(*str, Font, color) != *str)
		{
			// Char could not be written
			return *str;
		}
		
		// Next char
		str++;
	}
	
	// Everything ok
	return *str;
}

//
//	Position the cursor
//
void ssd1306_SetCursor(uint8_t x, uint8_t y) 
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}
