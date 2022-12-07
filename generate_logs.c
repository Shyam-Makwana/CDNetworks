#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_SERVERS 1000
#define TOTAL_MINUTES 1440

struct server
{
	int timestamp;		    //Timestamp in Unix system
	short ip_addr[4];		//Ip address
	short cpu_id[2];		//CPU ID for storing ID 0 and ID 1
	short cpu_usage[2];	    //CPU usage for both 0 and 1
}s;

void set_struct_values(struct server *s, int timestamp, int byte[])
{
    // Set the timestamp
    s->timestamp = timestamp;

    // The format used in IP Add = ip_addr[0].ip_addr[1].ip_addr[2].ip_addr[3]
    // Set the first two index of IP Address
    s->ip_addr[0] = 192;
    s->ip_addr[1] = 168;
    
    // If the last byte is 256 then make the last byte = 0 and increment 1st byte of byte array
    if (byte[1] == 256)
    {
        byte[0]++;
        byte[1] = 0;
    }
    // Assign the byte array to ip_addr array
    s->ip_addr[2] = byte[0];
    s->ip_addr[3] = byte[1]++;

    //Set the 2 CPU ID
    s->cpu_id[0] = 0;
    s->cpu_id[1] = 1;

    // Set any random usage for both the CPU
    s->cpu_usage[0] = rand() % 101;
    s->cpu_usage[1] = rand() % 101;
}

int get_unix_timestamp()
{
    // Declare a member of type struct tm
    struct tm log_time;

    // To generate logs of date 2014-10-31
    int year = 2014, month = 10, day = 31;

    // Update the values of struct members
    //The tm_year field is relative to 1900. So, subtracting 1900 from the year 
    log_time.tm_year = year - 1900;
    log_time.tm_mon = month - 1;
    log_time.tm_mday = day;
    log_time.tm_hour = 0;
    log_time.tm_min = 0;
    log_time.tm_sec = 0;

    // This function is used to convert struct to int data type
    int timestamp = timegm(&log_time);

    return timestamp;
}

void main(int argc, char *argv[])
{
    // Open the file in append mode to add logs in it
    FILE *fptr;
    if((fptr = fopen(argv[1],"a")) == NULL)
    {
        printf("Error in opening file!");   
        return;
    }
    fprintf(fptr, "Timestamp    IP            cpu_id    usage\n");

    // Get the timestamp in Unix format for date 2014-10-31
    int timestamp = get_unix_timestamp();

    // To generate random number every time
    srand(time(NULL));

    // Traverse through every minute
    for(int i=0; i<TOTAL_MINUTES; ++i, timestamp+=60){

        // For updating IP Address
        int byte[2] = {0};

        for(int j=0; j<TOTAL_SERVERS; ++j){
            
            // This set_struct_values will set the values of all struct members
            struct server s;
            set_struct_values(&s, timestamp, byte);

            // Add the logs in file using fprintf function
            fprintf(fptr,"%d   %d.%d.%d.%d    %d        %3d\n", s.timestamp, s.ip_addr[0], s.ip_addr[1], s.ip_addr[2], s.ip_addr[3], s.cpu_id[0], s.cpu_usage[0]);
            fprintf(fptr,"%d   %d.%d.%d.%d    %d        %3d\n", s.timestamp, s.ip_addr[0], s.ip_addr[1], s.ip_addr[2], s.ip_addr[3], s.cpu_id[1], s.cpu_usage[1]);
        }
    }
    //Close the file
    fclose(fptr);
}