#include <io.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MAX_LINE_LENGTH 2048
#define BASE_ADDRESS    1
#define PERIHERAL_NAME  2
#define OFFSET          3
#define REGISTER_NAME   4
#define DESCRIPTION     5
#define WIDTH           6
#define ACCESS          7

/* Menu */
void Display_Menu()
{
  printf("--------------------------------\n");
  printf("Menu:\n");
  printf("1. Choose location option\n");
  printf("   1.1 Set input file location\n");
  printf("   1.2 Set output file location\n");
  printf("2. Generate header files\n");
  printf("3. Exit\n");
  printf("--------------------------------\n");
}

/* Heximal to Decimal */
int Convert_HextoDec(char hex_dec[])
{
  int decNumber = 0;
  int length = strlen(hex_dec);
  int power = 0;
    
  int start = 0;
  if (hex_dec[0] == '0' && (hex_dec[1] == 'x' || hex_dec[1] == 'X'))
    start = 2;
    
  for (int i = length - 1; i >= start; i--)
  {
    char hexDigit = hex_dec[i];
    int decValue;
        
    if (hexDigit >= '0' && hexDigit <= '9') 
      decValue = hexDigit - '0';
    else if (hexDigit >= 'A' && hexDigit <= 'F') 
      decValue = hexDigit - 'A' + 10;
    else if (hexDigit >= 'a' && hexDigit <= 'f') 
      decValue = hexDigit - 'a' + 10;
    else 
    {
      printf("Invalid hexadecimal digit: %c\n", hexDigit);
      return -1;
    }
        
    decNumber += decValue * pow(16, power);
    power++;
  }
    
  return decNumber;
}

/* Replace backslashes from the entered input */
void Replace_Backslashes(char *path, char *modified_path)
{
  int j = 0;
  for (int i = 0; path[i] != '\0'; i++)
  {
    if (path[i] != '"')
      modified_path[j++] = path[i];
  }
  modified_path[j] = '\0';
}

/* Replace words in Tempate */
void Replace_Placeholder(char *output, const char *placeholder, const char *value)
{
  while ((output = strstr(output, placeholder)) != NULL)
  {
    int len_placeholder = strlen(placeholder);
    char temp[MAX_LINE_LENGTH * 10];
    strcpy(temp, output + len_placeholder);
    strcpy(output, value);
    strcat(output, temp);
  }
}

/* Substring */
void Get_String(int pos, const char *string, char *check_type, int last_string)
{
  char substring[100];
  int len = strlen(string) - last_string;
  int counter = 0;
  while ((pos - 1 + counter) < len)
  {
    substring[counter] = string[pos - 1 + counter];
    counter++;
  }
  substring[counter] = '\0';
  strcpy(check_type, substring);
}

typedef struct Register
{
  char base_address[10];
  char peripheral_name[10];
  int offset;
  char register_name[50];
  char description[50];
  int width;
  char access[10];
  char check_type[10];
  struct Register *next;
} Register;

/* Linked List of Register Information */
Register *Create_Register(char *base_address, char *peripheral_name, int offset, char *register_name, char *description, int width, char *access, char *check_type)
{
  Register *newRegister = (Register*) malloc(sizeof(Register));
  strcpy(newRegister->base_address, base_address);
  strcpy(newRegister->peripheral_name, peripheral_name);
  newRegister->offset = offset;
  strcpy(newRegister->register_name, register_name);
  strcpy(newRegister->description, description);
  newRegister->width = width;
  strcpy(newRegister->access, access);
  strcpy(newRegister->check_type, check_type);
  newRegister->next = NULL;

  return newRegister;
}

/* Linked List of Register Appending */
void Append_Register(Register **head, char *base_address, char *peripheral_name, int offset, char *register_name, char *description, int width, char *access, char *check_type)
{
  Register *newRegister = Create_Register(base_address, peripheral_name, offset, register_name, description, width, access, check_type);
  if (*head == NULL)
  {
    *head = newRegister;
    return;
  }

  Register *temp = *head;
  while (temp->next != NULL)
    temp = temp->next;

  temp->next = newRegister;
}

/* Read CSV file */
void Parse_CSVLine(char *line, char *base_address, char *peripheral_name, int *offset, char *register_name, char *description, int *width, char *access)
{
  char current_field[MAX_LINE_LENGTH];
  int column_number = 1;
  int j = 0;

  for (int i = 0; line[i] != '\0'; i++)
  {
    if (line[i] == ',' || line[i+1] == '\0' || line[i+1] == '\n')
    {
      if (line[i] == ',')
        current_field[j] = '\0';
      else
      {
        current_field[j] = line[i];
        current_field[j+1] = '\0';
      }

      switch (column_number)
      {
      case BASE_ADDRESS:
        if (strlen(current_field) > 0)
          strcpy(base_address, current_field);
        else
          strcpy(base_address, " ");
        break;
      case PERIHERAL_NAME:
        if (strlen(current_field) > 0)
          strcpy(peripheral_name, current_field);
        break;
      case OFFSET:
        if (strlen(current_field) > 0)
          *offset = Convert_HextoDec(current_field);
        break;
      case REGISTER_NAME:
        if (strlen(current_field) > 0)
          strcpy(register_name, current_field);
        break;
      case DESCRIPTION:
        if (strlen(current_field) > 0)
          strcpy(description, current_field);
        break;
      case WIDTH:
        if (strlen(current_field) > 0)
          *width = atoi(current_field);
        break;
      case ACCESS:
        if (strlen(current_field) > 0)
          strcpy(access, current_field);
      default:
        break;
      }

      column_number++;
      j = 0;
    }
    else
      current_field[j++] = line[i];
  }
}

/* Write header file output */
void Write_HeaderFile(const char *module_name, Register *register_list, const char *template_content, const char *output_directory)
{
  char output_content[MAX_LINE_LENGTH * 10] = "";
  strcpy(output_content, template_content);

  char check_type[10] = "";
  char register_type[10] = "";
  char check_type_null[10] = "";
  Register *current = register_list;

  if (strcmp(current->check_type, check_type_null) != 0)
  {
    Get_String(1, module_name, check_type, 1);
    Replace_Placeholder(output_content, "<MSN>", check_type);
    strcpy(register_type, check_type);
  }
  else
  {
    Replace_Placeholder(output_content, "<MSN>", module_name);
    strcpy(register_type, module_name);
  }

  char header_file_name[MAX_LINE_LENGTH];
  snprintf(header_file_name, MAX_LINE_LENGTH, "%s/%s_Register.h", output_directory, register_type);

  FILE *output_file = fopen(header_file_name, "w");
  if (!output_file)
  {
    printf("Error creating file %s.\n", header_file_name);
    return;
  }

  printf("Output file %s generated successfully.\n", header_file_name);

  time_t now = time(NULL);
  char time_generated[100];
  struct tm *t = localtime(&now);
  strftime(time_generated, sizeof(time_generated) - 1, "%Y-%m-%d %H:%M:%S", t);
  Replace_Placeholder(output_content, "<NOW>", time_generated);
  Replace_Placeholder(output_content, "<NAME_OF_STUDENT>", "Nguyen Quang Thien");

  char defined_registers[MAX_LINE_LENGTH * 10] = "";
  char register_entries[MAX_LINE_LENGTH * 10] = "";
  int current_offset = 0;
  int dummy_count = 0;
  
  while (current != NULL)
  {
    if (!strstr(defined_registers, current->register_name))
    {
      if (current->offset > current_offset)
      {
        int dummy_size = (current->offset - current_offset) / 4;
        if (dummy_size > 0)
        {
          char dummy_entry[MAX_LINE_LENGTH];
          snprintf(dummy_entry, MAX_LINE_LENGTH, "  unsigned int dummy%d[%d];\n", dummy_count++, dummy_size);
          strcat(register_entries, dummy_entry);
        }
      }

      char entry_line[MAX_LINE_LENGTH];
      snprintf(entry_line, MAX_LINE_LENGTH, "  /* %s */\n  %s unsigned int %s;\n",
              current->description,
              strcmp(current->access, "RO") == 0 ? "volatile const" : "volatile",
              current->register_name);

      strcat(register_entries, entry_line);
      strcat(defined_registers, current->register_name);
      strcat(defined_registers, " ");
    }

    current_offset = current->offset + 4;
    current = current->next;
  }

  char final_output[MAX_LINE_LENGTH * 10] = "";
  char *pos = strstr(output_content, "typedef struct {");

  if (pos != NULL)
  {
    strncpy(final_output, output_content, pos - output_content);
    final_output[pos - output_content] = '\0';
    strcat(final_output, "typedef struct\n{\n");
    strcat(final_output, register_entries);
    strcat(final_output, "} ");
    strcat(final_output, register_type);
    strcat(final_output, "_Type;\n\n");
    
    char after_typedef[MAX_LINE_LENGTH * 10];
    strcpy(after_typedef, pos + strlen("typedef struct {"));
    char *end_of_struct = strstr(after_typedef, "};");
    if (end_of_struct != NULL)
      strcat(final_output, after_typedef + (end_of_struct - after_typedef));
  }

  current = register_list;
  char base_addr_define[MAX_LINE_LENGTH * 10] = "";
  char defined_peripherals[MAX_LINE_LENGTH * 10] = "";

  while (current != NULL)
  {
    if (strcmp(current->base_address, " ") != 0)
    {
      char single_base_addr[MAX_LINE_LENGTH];
      char base_addr[10] = "";
      strcpy(base_addr, current->base_address);
      int cut_str = strlen(base_addr) - 10;
      Get_String(1, base_addr, base_addr, cut_str);
      
      snprintf(single_base_addr, MAX_LINE_LENGTH,
              "#define %s_BASE_ADDRS (%s)\n#define %s ((%s_Type*)%s_BASE_ADDRS)\n\n",
              current->peripheral_name, base_addr,
              current->peripheral_name, register_type,
              current->peripheral_name);

      strcat(base_addr_define, single_base_addr);
      strcat(defined_peripherals, current->peripheral_name);
      strcat(defined_peripherals, " ");
    }
    current = current->next;
  }

  strcat(final_output, base_addr_define);
  strcat(final_output, "#endif  /* #if !defined(");
  strcat(final_output, register_type);
  strcat(final_output, "_REGISTER_H_) */\n");

  fputs(final_output, output_file);
  fclose(output_file);
}

/* Check valid path*/
int Check_Path_Exists(const char *path)
{
  int path_length = strlen(path);
  if (path_length < 2)
      return 0;

  char *check_path = (char *)malloc((path_length - 2) * sizeof(char));
  if (check_path == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    return 0;
  }

  for(int i = 0; i < path_length - 2; i++)
    check_path[i] = path[i+1];

  check_path[path_length-2] = '\0';
  int result = (_access(check_path, 0) != -1);
  free(check_path);

  return result;
}

int main()
{
  char csv_file_path[MAX_LINE_LENGTH] = "";
  char template_file_path[MAX_LINE_LENGTH] = "";
  char output_directory[MAX_LINE_LENGTH] = "";
  char path[MAX_LINE_LENGTH] = "";
  int choice, sub_choice;

  while (1)
  {
    Display_Menu();
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice)
    {
      case 1:
        printf("\n1. Set input file location\n");
        printf("2. Set output file location\n");
        printf("Enter your sub-choice: ");
        scanf("%d", &sub_choice);
        getchar();

        if (sub_choice == 1)
        {
          do
          {
            printf("Enter the path for the CSV file: ");
            fgets(path, MAX_LINE_LENGTH, stdin);
            path[strcspn(path, "\n")] = '\0';

            if (!Check_Path_Exists(path))
              printf("Invalid path for the CSV file. Please try again.\n");
          } while (!Check_Path_Exists(path));
          
          Replace_Backslashes(path, csv_file_path);
          printf("CSV file path set successfully: %s\n", csv_file_path);

          do
          {
            printf("Enter the path for the template file: ");
            fgets(path, MAX_LINE_LENGTH, stdin);
            path[strcspn(path, "\n")] = '\0';

            if (!Check_Path_Exists(path))
              printf("Invalid path for the template file. Please try again.\n");
          } while (!Check_Path_Exists(path));

          Replace_Backslashes(path, template_file_path);
          printf("Template file path set successfully: %s\n", template_file_path);
        }
        else if (sub_choice == 2)
        {
          do
          {
            printf("Enter the output directory path: ");
            fgets(path, MAX_LINE_LENGTH, stdin);
            path[strcspn(path, "\n")] = '\0';

            if (!Check_Path_Exists(path))
              printf("Invalid path for the output directory. Please try again.\n");
          } while (!Check_Path_Exists(path));

          Replace_Backslashes(path, output_directory);
          printf("Output directory path set successfully: %s\n", output_directory);
        }
        else
          printf("Invalid sub-choice.\n");
        break;

      case 2:
        if (csv_file_path[0] == '\0' || template_file_path[0] == '\0' || output_directory[0] == '\0')
          printf("\nPlease set all file locations before generating header files.\n");
        else
        {
          FILE *csv_file = fopen(csv_file_path, "r");
          FILE *template_file = fopen(template_file_path, "r");

          if (!csv_file || !template_file)
          {
            printf("Error opening file(s).\n");
            return 1;
          }

          char template_content[MAX_LINE_LENGTH * 10] = "";
          char *template_ptr = template_content;
          while (fgets(template_ptr, MAX_LINE_LENGTH, template_file))
            template_ptr += strlen(template_ptr);
          fclose(template_file);

          char line[MAX_LINE_LENGTH];
          fgets(line, MAX_LINE_LENGTH, csv_file);

          struct ModuleRegisters
          {
            char module_name[10];
            Register *register_list;
            struct ModuleRegisters *next;
          } *modules = NULL;

          char base_address[10] = "";
          char peripheral_name[10] = "";
          int offset = 0;
          char register_name[50] = "";
          char description[50] = "";
          int width = 0;  
          char access[10] = "";
          char check_type_module[10] = "";
          char check_type_periheral[10] = "";
          char check_type_null[10] = "";

          while (fgets(line, MAX_LINE_LENGTH, csv_file))
          {
            Parse_CSVLine(line, base_address, peripheral_name, &offset, register_name, description, &width, access);
            struct ModuleRegisters *mod = modules;
            struct ModuleRegisters *lastMod = NULL;

            while (mod != NULL)
            {
              lastMod = mod;
              mod = mod->next;
            }
            mod = lastMod;
            
            if(mod->module_name != NULL)
            {
              Get_String(1, mod->module_name, check_type_module, 1);
              Get_String(1, peripheral_name, check_type_periheral, 1);
            }
            
            while (mod != NULL && strcmp(mod->module_name, peripheral_name) != 0)
            {
              if(strcmp(check_type_module, check_type_periheral)  == 0)
              {
                strcpy(mod->register_list->check_type, check_type_module);
                break;
              }
              else
              {
                mod = mod->next;
                strcpy(check_type_module, check_type_null);
                strcpy(check_type_periheral, check_type_null);
              }
            }

            if (mod == NULL)
            {
              mod = (struct ModuleRegisters *)malloc(sizeof(struct ModuleRegisters));
              strcpy(mod->module_name, peripheral_name);
              mod->register_list = NULL;
              mod->next = NULL;

              if (lastMod == NULL)
                modules = mod;
              else
                lastMod->next = mod;
            }

            Append_Register(&mod->register_list, base_address, peripheral_name, offset, register_name, description, width, access, check_type_module);
          }
          fclose(csv_file);

          struct ModuleRegisters *mod = modules;
          while (mod != NULL)
          {
            Write_HeaderFile(mod->module_name, mod->register_list, template_content, output_directory);
            mod = mod->next;
          }

          while (modules != NULL)
          {
            struct ModuleRegisters *temp_mod = modules;
            modules = modules->next;
            Register *reg_list = temp_mod->register_list;
            while (reg_list != NULL)
            {
              Register *temp_reg = reg_list;
              reg_list = reg_list->next;
              free(temp_reg);
            }
            free(temp_mod);
          }
        }
        break;

      case 3:
        {
          printf("\nExiting...\n");
          return 0;
        }

      default:
        printf("\nInvalid choice. Please try again.\n");
    }
  }
  return 0;
}
