#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define __USE_XOPEN
#include <time.h>

// Function to print error message if the user enter any wrong input or we can't find any logs
void print_error_mess()
{
   printf("No data found. The reason would be one of the following:\n");
   printf("1) Usage: QUERY IP cpu_id time_start time_end\n");
   printf("2) Range of IP: 192.168.0.0 to 192.168.0.255, 192.168.1.0 to 192.168.1.255, 192.168.2.0 to 192.168.2.255, 192.168.3.0 to 192.168.3.231\n");
   printf("3) cpu_id is either 0 or 1\n");
   printf("4) start_time and end_time are in YYYY-MM-DD HH:MM format\n");
   printf("5) Range of time: 2014-10-31 00:00 to 2014-10-31 23:59\n\n");
}

// Function to get unix timestamp from YYYY-MM-DD HH:MM format
int get_timestamp(char *date, char *time)
{
   // Combine YYYY-MM-DD and HH:MM into one array 
   char date_time[17];
   strcpy(date_time, date);
   strcat(date_time, " ");
   strcat(date_time, time);

   struct tm timestamp;

   // Function to set all the values to 0 of struct tm
   memset(&timestamp, 0, sizeof(struct tm));

   // convert to unix timestamp
   strptime(date_time, "%Y-%m-%d %H:%M", &timestamp);

   return timegm(&timestamp);
}

// Function to split the user input
char** split_input(char *str, int words)
{
   char delimiter[] = " \n";
   char **input = (char **)malloc(sizeof(char *) * words);

   // Strtok is used to point the pointer to delimeter character
   char *token = strtok(str, delimiter);

   int i = 0;
   while(token != NULL)
   {
      input[i++] = token;
      token = strtok(NULL, delimiter);
   }
   return input;
}

// Function to Calculate the number of digits in the timestamp
int get_int_len(int timestamp){
    int length = 0;
    while(timestamp){
        timestamp /= 10;
        length++;
    }
    return length;
}

// Function to convert (timestamp) int to string data type
char* int_to_string(int timestamp)
{
   // Calculating the number of digits in the timestamp
   int n = get_int_len(timestamp);
   char *time_in_string = calloc(n, sizeof(char));

   for (int i = n-1; i >= 0; --i, timestamp /= 10)
   {
      time_in_string[i] = (timestamp % 10) + '0';
   }
   return time_in_string;
}

// Function to check if the user input is valid or not
bool is_input_valid(char input[])
{
   int count = 0;
   for(int i = 0; input[i] != '\0'; ++i)
   {
      if(input[i] == ' ' && input[i+1] != ' ')
         count++;
   }

   // Checking if the count of space is 6 or not
   return (count == 6);
}

int main(int argc, char *argv[])
{
   //Opening the log in read mode
   FILE *f;
   if ((f = fopen(argv[1], "r")) == NULL) {
      printf("Error in opening %s file!\n", argv[1]);   
      return 0;
   }
   
   // While loop to continue query the logs and exit from the loop
   // if user enter EXIT
   while(1){
      // Character array for storing input
      char str[100];
      printf(">");
      scanf("%[^\n]%*c", str);

      // If the input is EXIT or exit, break the loop
      if(strcasecmp(str, "EXIT") == 0)  return 0;

      // Checking if the input is valid or not
      if(!is_input_valid(str))
      {
         print_error_mess();
         continue;
      }

      // Spliting the user input in array for performing operations
      // Second argument is the number of words in the input
      char **input = split_input(str, 7);

      // If the command is not QUERY then print the error message and continue the loop
      if(strcasecmp(input[0], "QUERY") != 0){
         printf("Valid Commands are QUERY or EXIT\n\n");
         continue;
      }

      // Convert the YYYY-MM-DD HH:MM time in unix_timestamp
      int time_start = get_timestamp(input[3], input[4]);
      int time_end = get_timestamp(input[5], input[6]);

      // Flag for printing this line: CPU1 usage on 192.168.1.10:
      // If we find any query, then only print above line
      // For that, this flag is used
      int result = 1;

      for(int i=time_start; i<time_end; i+=60)
      {
         // Log array for storing timestamp, IP Address, and CPU ID
         char *logs[3];
         logs[0] = int_to_string(i);
         logs[1] = input[1];
         logs[2] = input[2];

         // For reading every line of log file
         char buffer[44];
         while (fgets(buffer, sizeof(buffer), f))
         {
            // Flag for checking if we find any logs
            int flag = 1;

            // Spliting the readline input from log file into array 
            char **query = split_input(buffer, 4);
            for(int j=0; j<3; ++j){
               if(strcmp(query[j], logs[j]) != 0){
                  flag = 0;
               }
            }

            // If we find any query, then print that query
            if(flag){
               if(result){
                  printf("CPU%s usage on %s:\n", input[2], input[1]);
                  result = 0;
               }
               
               char res[18];
               time_t t = i;
               // Converting unix timestamp to YYYY-MM-DD HH:MM format
               struct tm* lt = gmtime(&t);
               strftime(res, sizeof(res), "%Y-%m-%d %H:%M", lt);

               printf("(%s, %s%%)", res, query[3]);
               
               // Don't print ',' (comma) if the query is last one
               if(i!=(time_end-60))  printf(", ");
               else printf("\n");

               // Break from the loop. No need to parse the remaining file
               // Because we won't get the result and check the next minute
               break;
            }
         }

         // If we didn't get the result after parsing the whole log file for one timestamp
         // That means that timestamp is not present and no need to check other timestamps
         if(result){
            break;
         }
      }
      if(result)  print_error_mess();

      // Set the file pointer to starting position of the file
      fseek(f, 0, SEEK_SET);
   }
   
   // Close the file
   fclose(f);
   return 0;
}