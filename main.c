/*
* main.c
*
* author: MUHAMMET FATIH KESKIN - PROJE 2*
*/

void clearSSD(void);
void setSSD(int);

void set_Rows_Keypad(void);
void clear_Rows_Keypad(void);

int main(void)
{
	init_variable();
	keypad_enable();
	school_id();

	while (1)
	{
	}

	return 0;
}
