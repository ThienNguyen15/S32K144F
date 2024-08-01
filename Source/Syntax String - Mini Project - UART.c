#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int Check_Integer(char str[])
{
  int i = 0;

  if (str[i] == '\0')
    return 0;

  if (str[i] == '-')
  {
    i++;
    if (str[i] == '\0')
      return 0;
  }

  while (str[i] != '\0')
  {
  if (!isdigit((unsigned char)str[i]))
        return 0;
    i++;
  }
  return 1;
}

// int Check_Syntax(char input[], char output[])
// {
//   char Operand1[100] = "";
//   char Operand2[100] = "";

//   char* plus_pos = strchr(input, '+');
//   char* equal_pos = strchr(input, '=');

//   if ( (plus_pos - input) <= 0 || (equal_pos - input) <= 2)
//   {
//     strcpy(output, "Wrong format\r\n");
//     return 0;
//   }

//   strncpy(Operand1, input, (unsigned int)(plus_pos - input));
//   Operand1[plus_pos - input] = '\0';

//   strncpy(Operand2, plus_pos + 1, (unsigned int)(equal_pos - plus_pos - 1));
//   Operand2[equal_pos - plus_pos - 1] = '\0';

//   if (!Check_Integer(Operand1) || !Check_Integer(Operand2))
//   {
//     strcpy(output, "Wrong format\r\n");
//     return 0;
//   }

//   int result = atoi(Operand1) + atoi(Operand2);
//   sprintf(output, "%d", result);
//   return 1;
// }

int Check_Syntax(char input[], char output[])
{
  char Operand1[100] = "";
  char Operand2[100] = "";
  int i = 0, j = 0;
  int plus_pos = -1, equal_pos = -1;

  for (i = 0; input[i] != '\0'; i++)
  {
    if (input[i] == '+')
    {
      plus_pos = i;
    }
    else if (input[i] == '=')
    {
      equal_pos = i;
      break;
    }
  }

  if (plus_pos <= 0 || equal_pos <= plus_pos + 1)
  {
    strcpy(output, "Wrong format\r\n");
    return 0;
  }

  for (i = 0; i < plus_pos; i++)
  {
    Operand1[i] = input[i];
  }
  Operand1[i] = '\0';

  for (i = plus_pos + 1, j = 0; i < equal_pos; i++, j++)
  {
    Operand2[j] = input[i];
  }
  Operand2[j] = '\0';

  if (!Check_Integer(Operand1) || !Check_Integer(Operand2))
  {
    strcpy(output, "Wrong format\r\n");
    return 0;
  }

  int result = atoi(Operand1) + atoi(Operand2);
  sprintf(output, "%d", result);
  return 1;
}

int main() {
    char input[100] = "";
    char output[100] = "";
    char print_result[200] = "";

  while (1)
  {
    printf("Enter the input string: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = 0;

    Check_Syntax(input, output);

    if(Check_Integer(output) != 0)
    {
      strcpy(print_result, input);
      strcat(print_result, output);
      strcat(print_result, "\r\n");
      printf("%s", print_result);
    }
    else
      printf("%s", output);
  }
  

    return 0;
}
