int str_count(char* str, char c)
{
	int i;
	for (i=0; str[i]; str[i]==c ? i++ : *str++);
	return i;
}
