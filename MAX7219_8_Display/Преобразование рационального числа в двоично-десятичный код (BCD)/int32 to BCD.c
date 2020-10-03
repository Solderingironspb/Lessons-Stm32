void int32_t_to_BCD(int32_t value) {
	uint8_t BCD_Arr[8] = { 0, };
	while (value >= 10000000) {
		value -= 10000000;
		BCD_Arr[7]++;
	}

	while (value >= 1000000) {
		value -= 1000000;
		BCD_Arr[6]++;
	}
	while (value >= 100000) {
		value -= 100000;
		BCD_Arr[5]++;
	}
	while (value >= 10000) {
		value -= 10000;
		BCD_Arr[4]++;
	}
	while (value >= 1000) {
		value -= 1000;
		BCD_Arr[3]++;
	}
	while (value >= 100) {
		value -= 100;
		BCD_Arr[2]++;
	}
	while (value >= 10) {
		value -= 10;
		BCD_Arr[1]++;
	}
	BCD_Arr[0] = (uint8_t) (value);

	printf("BCD_Arr[7] = %d\r\n", BCD_Arr[7]);
	printf("BCD_Arr[6] = %d\r\n", BCD_Arr[6]);
	printf("BCD_Arr[5] = %d\r\n", BCD_Arr[5]);
	printf("BCD_Arr[4] = %d\r\n", BCD_Arr[4]);
	printf("BCD_Arr[3] = %d\r\n", BCD_Arr[3]);
	printf("BCD_Arr[2] = %d\r\n", BCD_Arr[2]);
	printf("BCD_Arr[1] = %d\r\n", BCD_Arr[1]);
	printf("BCD_Arr[0] = %d\r\n\r\n", BCD_Arr[0]);
}