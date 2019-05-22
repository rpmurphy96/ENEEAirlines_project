//
// Created by Ryan Murphy on 3/30/19.
//

/*

  The required function prototypes and several variable names for this project were provided by Donald Yeung.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NO 0
#define YES 1
#define INVALID -23
#define VALID -54

// Function prototypes (ideally these would be moved to a different .h file)
int string_to_int(char string[]);
int check_flight_number(int flight_number_entered);
int check_airport_code(char airport_entered[]);


struct airports_data {
    char airport_code[4];
    char airport_location[41]; // The location is a char array no longer than 40 characters
} airports[100];

struct routes_data {
    char routeID_string[3];
    int routeID;
    char flight1[4]; // flight1 = departure airport servicing the associated route ID (the airport_code)
    char flight2[4]; // flight2 = arrival airport servicing the associated route ID (the airport_code)
} routes[500];

struct flights_data {
    int flight_number;
    int routeID;
    char time1[7];
    char time2[7];
    long time1number; // time1 = departure time
    long time2number; // time2 = arrival time
    char schedule[8]; // The number of days that a flight flies
} flights[3000];

int main() {

    // Local Variables
    int days_flight_runs_on[7];
    char days_of_week[7][3] = {{'S','\0',},
                               {'M','\0'},
                               {'T','\0'},
                               {'W','\0'},
                               {'T','h','\0'},
                               {'F','\0'},
                               {'S','a','\0'}};

    // Open the files
    FILE *flights_file = fopen("flights.txt", "r");
    FILE *airports_file = fopen("airports.txt", "r");
    FILE *routes_file = fopen("routes.txt", "r");


    if (flights_file == NULL || airports_file == NULL || routes_file == NULL) {
        printf ("Could not open database files\n"); // Error correction if any of the files can't be opened
        exit (0);
    }
    // Parsing the data
    int i, j, k, p, f, g, r, l, t, e;
    for (i = 0; i < 100; i++){
        fscanf(airports_file, "%s %[^\n]\n", airports[i].airport_code, airports[i].airport_location);
        airports[i].airport_code[3] = '\0';
        airports[i].airport_location[40] = '\0';
    }
    for (i = 0; i < 500; i++){
        fscanf(routes_file, "%[^.]. %[^ ] %[^\n]\n", routes[i].routeID_string, routes[i].flight1, routes[i].flight2);
        routes[i].routeID = atoi(routes[i].routeID_string);
        routes[i].flight1[3] = '\0';
        routes[i].flight2[3] = '\0';
    }
    char flight_number_string[5]; // Temporary, "dummy" array used to help me parse data
    char routeID_string[5]; // Temporary, "dummy" array used to help me parse data
    for (i = 0; i < 3000; i++){

        fscanf(flights_file, "%[^ ] %[^ ] %[^ ] %[^ ] %[^\n]\n", flight_number_string, routeID_string,
                flights[i].time1, flights[i].time2, flights[i].schedule);

        flights[i].time1[6] = '\0';
        flights[i].time2[6] = '\0';

        if (flights[i].time1[4] == 'p' || flights[i].time1[5] == 'p') {
            flights[i].time1number = string_to_int(flights[i].time1) + 1200;
        } else flights[i].time1number = string_to_int(flights[i].time1);

        if (flights[i].time2[4] == 'p' || flights[i].time2[5] == 'p') {
            flights[i].time2number = string_to_int(flights[i].time2) + 1200;
        } else flights[i].time2number = string_to_int(flights[i].time2);


        flights[i].flight_number = atoi(flight_number_string);
        flights[i].routeID = atoi(routeID_string);
        flights[i].schedule[7] = '\0';

    }

    // ************************************************ USER INTERFACE ************************************************
    printf("Welcome to the ENEE Airlines Flight Database\n");
    int print_menu = YES;
    while (1){
        int option;
        if (print_menu == YES){
            printf("Choose an option:\n1.  Print a flight\n"
                   "2.  Print an airport\n3.  Find non-stop flights\n4.  Find 1-stop flights\n5.  Exit\n");
        }
        printf("Enter option:  ");
        scanf("%d", &option);


        if (option > 5 || option < 1){
            printf("Invalid option.  Try again.\n");
            print_menu = NO;
            continue;
        }

        if (option == 1){
            // Do everything associated with the user entering 1
            int flight_number_entered;
            printf("Enter a flight number:  ");
            scanf("%d", &flight_number_entered);

            if (check_flight_number(flight_number_entered) == INVALID){
                printf("Flight %d doesn't exist\n", flight_number_entered);
                print_menu = YES;
                continue;
            }

            // Set the days the flight runs on to all days by default
            for (i=1;i<=7;i++){
                days_flight_runs_on[i-1] = i;
            }

            if (flights[check_flight_number(flight_number_entered)].schedule[0] == 'X'){
                for (i=1; i<=7 && flights[check_flight_number(flight_number_entered)].schedule[i] != '\0'; i++){
                    days_flight_runs_on[(flights[check_flight_number(flight_number_entered)].schedule[i] - '0') - 1] = -1;
                }
            }
            else if  (flights[check_flight_number(flight_number_entered)].schedule[0] == '1' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '2' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '3' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '4' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '5' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '6' ||
                    flights[check_flight_number(flight_number_entered)].schedule[0] == '7'){
                for (k = 0; k<7; k++){
                    days_flight_runs_on[k] = 0;
                }
                for (i=0; i<7 && flights[check_flight_number(flight_number_entered)].schedule[i] != '\0'; i++){
                    days_flight_runs_on[flights[check_flight_number(flight_number_entered)].schedule[i] - '0'] =
                            (flights[check_flight_number(flight_number_entered)].schedule[i] - '0') + 1;
                }
            }

            printf("\nFlight %d (", flight_number_entered);
            for (i=0; i<7; i++){
                if (days_flight_runs_on[i] > 0)
                    printf("%s", days_of_week[days_flight_runs_on[i]-1]);
            }
            printf(")\n    ");

            // Print the time of the first flight
            if (flights[check_flight_number(flight_number_entered)].time1number < 1000 ||
                (flights[check_flight_number(flight_number_entered)].time1number >= 1300 &&
                 flights[check_flight_number(flight_number_entered)].time1number < 2200)){
                printf("0");
            }
            printf("%s.m. ", flights[check_flight_number(flight_number_entered)].time1);
            // Print the location of the first flight
            // But first, find the element in routes associated with the routeID
            int routeID_entered = flights[ check_flight_number(flight_number_entered) ].routeID;
            char first_airport[4];
            strcpy(first_airport, routes[routeID_entered].flight1);


            for (i=0; i<100; i++){
                if (strcmp(airports[i].airport_code, first_airport)==0){
                    printf("%s (%s)\n    ", airports[i].airport_location, airports[i].airport_code);
                }
            }

            // Print the time of the second flight
            if (flights[check_flight_number(flight_number_entered)].time2number < 1000 ||
                (flights[check_flight_number(flight_number_entered)].time2number >= 1300 &&
                 flights[check_flight_number(flight_number_entered)].time2number < 2200)){
                printf("0");
            }
            printf("%s.m. ", flights[check_flight_number(flight_number_entered)].time2);
            // Print the location of the second flight
            char second_airport[4];
            strcpy(second_airport, routes[routeID_entered].flight2);

            for (i=0; i<100; i++){
                if (strcmp(airports[i].airport_code, second_airport)==0){
                    printf("%s (%s)\n\n", airports[i].airport_location, airports[i].airport_code);
                }
            }

            // Exit from option 1
            print_menu = YES;
            continue;
        }

        if (option == 2){
            // Do everything associated with the user entering 2
            char airport_entered[4];
            printf("Enter airport code:  ");
            scanf("%s", &airport_entered);
            airport_entered[3] = '\0';

            if (check_airport_code(airport_entered) == INVALID){
                printf("Airport %s doesn't exist\n", airport_entered);
                print_menu = YES;
                continue;
            }

            printf("\n%s (%s)\n\nDepartures to:", airports[check_airport_code(airport_entered)].airport_location,
                    airports[check_airport_code(airport_entered)].airport_code);

            // Print all airport locations and codes that are second in the list where the airport_entered is the first
            for (i=0; i<500; i++){
                if (strcmp(routes[i].flight1, airport_entered) == 0){
                    for (k=0; k<100; k++){
                        if (strcmp(routes[i].flight2, airports[k].airport_code) == 0){
                            printf("\n   %s (%s)", airports[k].airport_location, airports[k].airport_code);
                        }
                    }
                }
            }
            printf("\n\nArrivals from:");
            // Print all airport locations and codes that are first in the list where the airport_entered is the second
            for (i=0; i<500; i++){
                if (strcmp(routes[i].flight2, airport_entered) == 0){
                    for (k=0; k<100; k++){
                        if (strcmp(routes[i].flight1, airports[k].airport_code) == 0){
                            printf("\n   %s (%s)", airports[k].airport_location, airports[k].airport_code);
                        }
                    }
                }
            }
            printf("\n\n");
            print_menu = YES;
            continue;
        }

        if (option == 3){
            // Do everything associated with the user entering 3
            int run = 0;
            char airport_1[4];
            char airport_2[4];
            printf("Enter departure and arrival airport codes:  ");
            scanf("%s %s", airport_1, airport_2);
            airport_1[3] = '\0';
            airport_2[3] = '\0';
            // Check to see if either airport_1 or airport_2 doesn't exist
            if (check_airport_code(airport_1) == INVALID || check_airport_code(airport_2) == INVALID){
                printf("Either airport %s or %s doesn't exist\n", airport_1, airport_2);
                print_menu = YES;
                continue;
            }
            // Find the location of each airport
            char airport_location_1[40];
            char airport_location_2[40];
            for (j=0; j<100; j++) {
                if (strcmp(airports[j].airport_code, airport_1) == 0)
                    strcpy(airport_location_1, airports[j].airport_location);
            }
            for (j=0; j<100; j++) {
                if (strcmp(airports[j].airport_code, airport_2) == 0)
                    strcpy(airport_location_2, airports[j].airport_location);
            }

            int day;
            int flies = NO;

            printf("Enter a day of the week (1-7, or 0 for all days):  ");
            scanf("%d", &day);

            // First, check to see if this combo of flights even exists in the routes file. If it doesn't, print the
            // error message
            for(i=0;i<500;i++){
                if (strcmp(routes[i].flight1, airport_1)==0 && strcmp(routes[i].flight2, airport_2)==0){
                    break;
                }
                if (i>=499){ // If all the elements in the routes file have been checked and the combo hasn't been found
                    // that means the combo doesn't exist, so print the error message
                    printf("Route %s to %s has no non-stop flights on the specified days", airport_1,
                           airport_2);
                }
            }

            // Now, check to see, from the routes file, what routeID's correspond to a flight with those airport codes
            for (i=0; i<500; i++){
                if (strcmp(routes[i].flight1, airport_1)==0 && strcmp(routes[i].flight2, airport_2)==0){
                    // Second, check to see, from the flights file, if the routeID just found flies on the day entered
                    for (k=0;k<3000;k++){
                        if (flights[k].routeID == routes[i].routeID){
                            // Check all the scenarios where the flight wouldn't fly on the day entered
                            if (flights[k].schedule[0] == 'X'){
                                if ((flights[k].schedule[1] - '0' == day) || (flights[k].schedule[2] - '0' == day)
                                || (flights[k].schedule[3] - '0' == day) || (flights[k].schedule[4] - '0' == day)
                                || (flights[k].schedule[5] - '0' == day) || (flights[k].schedule[6] - '0' == day)
                                || (flights[k].schedule[7] - '0' == day)){
                                    // Meaning the flight doesn't fly on the day entered
                                    if (k == 2999){
                                        printf("Route %s to %s has no non-stop flights on the specified days", airport_1,
                                               airport_2);
                                    }
                                    continue;
                                }
                            }
                            if (flights[k].schedule[0] != 'X' && flights[k].schedule[0] != 'D'){
                                if ((flights[k].schedule[0] - '0' != day) && (flights[k].schedule[1] - '0' != day)
                                    && (flights[k].schedule[2] - '0' != day) && (flights[k].schedule[3] - '0' != day)
                                    && (flights[k].schedule[4] - '0' != day) && (flights[k].schedule[5] - '0' != day)
                                    && (flights[k].schedule[6] - '0' != day) && (flights[k].schedule[7] - '0' != day)){
                                    if (k == 2999){
                                        printf("Route %s to %s has no non-stop flights on the specified days\n", airport_1,
                                               airport_2);
                                    }
                                    continue;
                                }
                            }
                            run++;
                            // Print the header only right before the we print the first matching flight
                            if (run == 1){
                                printf("\nNon-stop flights from %s (%s) to %s (%s):\n", airport_location_1, airport_1,
                                       airport_location_2, airport_2);
                            }

                            // At this point, the flight with the day entered passes all the tests, so we know it must
                            // fly on the day entered

                            // Print the necessary flight info

                            // First, convert the days the flight flies on to something we can print

                            // Set the days the flight runs on to all days by default
                            for (j=1;j<=7;j++){
                                days_flight_runs_on[j-1] = j;
                            }
                            if (flights[k].schedule[0] == 'X'){
                                for (j=1; j<=7 && flights[k].schedule[j] != '\0'; j++){
                                    days_flight_runs_on[(flights[k].schedule[j] - '0') - 1] = -1;
                                }
                            }
                            else if  (flights[k].schedule[0] == '1' ||
                            flights[k].schedule[0] == '2' ||
                            flights[k].schedule[0] == '3' ||
                            flights[k].schedule[0] == '4' ||
                            flights[k].schedule[0] == '5' ||
                            flights[k].schedule[0] == '6' ||
                            flights[k].schedule[0] == '7'){
                                for (j = 0; j<7; j++){
                                    days_flight_runs_on[j] = 0;
                                }
                                for (j=0; j<7 && flights[k].schedule[j] != '\0'; j++){
                                    days_flight_runs_on[flights[k].schedule[i] - '0'] =
                                            (flights[k].schedule[j] - '0') + 1;
                                }
                            }

                            // Print the flight number
                            printf("\nFlight %d (", flights[k].flight_number);
                            for (j=0; j<7; j++){
                                if (days_flight_runs_on[j] > 0)
                                    printf("%s", days_of_week[days_flight_runs_on[j]-1]);
                                if (j==6)
                                    printf(")\n");
                            }

                            // Print the time of the first flight
                            printf("    ");
                            if (flights[k].time1number < 1000 || (flights[k].time1number >= 1300 &&
                                                                        flights[k].time1number < 2200)){
                                printf("0");
                            }
                            printf("%s.m. ", flights[k].time1);

                            // Print the location of the first flight
                            printf("%s (%s)\n    ", airport_location_1, airport_1);

                            // Print the time of the second flight
                            if (flights[k].time2number < 1000 || (flights[k].time2number >= 1300 &&
                                                                        flights[k].time2number < 2200)){
                                printf("0");
                            }
                            printf("%s.m. ", flights[k].time2);

                            // Print the location of the second flight
                            printf("%s (%s)\n", airport_location_2, airport_2);
                        }
                    }
                }
            }
            printf("\n");
            print_menu = YES;
            continue;
        }

        if (option == 4){
            // Do things associated with this
            int run = 0;
            char airport_1[4];
            char airport_1_location[40];
            char airport_rando[4];
            char airport_rando_location[40];
            char airport_2[4];
            char airport_2_location[40];
            int routeID_1;
            int routeID_2;
            int flight_num_a = 0;
            int flight_a_index = 0;
            int days_flight_a_runs_on[7];
            int flight_num_b = 0;
            int flight_b_index = 0;
            int days_flight_b_runs_on[7];
            int flight_a_fly_status = NO;
            int flight_b_fly_status = NO;
            printf("Enter departure and arrival airport codes:  ");
            scanf("%s %s", airport_1, airport_2);
            airport_1[3] = '\0';
            airport_2[3] = '\0';

            // Check to see if either airport_1 or airport_2 doesn't exist
            if (check_airport_code(airport_1) == INVALID || check_airport_code(airport_2) == INVALID){
                printf("Either airport %s or %s doesn't exist\n", airport_1, airport_2);
                print_menu = YES;
                continue;
            }

            // Find the location of each airport
            for (j=0; j<100; j++) {
                if (strcmp(airports[j].airport_code, airport_1) == 0)
                    strcpy(airport_1_location, airports[j].airport_location);
            }

            for (j=0; j<100; j++) {
                if (strcmp(airports[j].airport_code, airport_2) == 0)
                    strcpy(airport_2_location, airports[j].airport_location);
            }

            int day;
            int flies = NO;

            printf("Enter a day of the week (1-7, or 0 for all days):  ");
            scanf("%d", &day);


            // First, find the routeID that starts from airport_1 and ends at ANY random airport
            for (i=0; i<500; i++){
                if (strcmp(airport_1, routes[i].flight1)==0){
                    strcpy(airport_rando, routes[i].flight2);
                    routeID_1 = routes[i].routeID;
                    // Find the routeID that starts from the random airport and ends at airport_2
                    for (j=0; j<500; j++){
                        if (strcmp(airport_rando, routes[j].flight1)==0 && strcmp(airport_2, routes[j].flight2)==0){
                            routeID_2 = routes[j].routeID;

                            // Find a flight corresponding to the first routeID
                            for (k=0; k<3000; k++){
                                if (routeID_1 == flights[k].routeID){
                                    flight_num_a = flights[k].flight_number;
                                    // Check to see if the first flight flies on the specified day
                                    flight_a_fly_status = NO;
                                    for (g=1;g<=7;g++){
                                        days_flight_a_runs_on[g-1] = g;
                                    }


                                    flight_a_index = check_flight_number(flight_num_a);


                                    if (flights[flight_a_index].schedule[0] == 'X'){
                                        for (g=1; g<=7 && flights[flight_a_index].schedule[g] != '\0'; g++){
                                            days_flight_a_runs_on[(flights[flight_a_index].schedule[g] - '0') - 1] = -1;
                                        }
                                    }
                                    else if  (flights[flight_a_index].schedule[0] == '1' ||
                                              flights[flight_a_index].schedule[0] == '2' ||
                                              flights[flight_a_index].schedule[0] == '3' ||
                                              flights[flight_a_index].schedule[0] == '4' ||
                                              flights[flight_a_index].schedule[0] == '5' ||
                                              flights[flight_a_index].schedule[0] == '6' ||
                                              flights[flight_a_index].schedule[0] == '7'){
                                        for (g = 0; g<7; g++){
                                            days_flight_a_runs_on[g] = 0;
                                        }
                                        for (g=0; g<7 && flights[flight_a_index].schedule[g] != '\0'; g++){
                                            days_flight_a_runs_on[flights[flight_a_index].schedule[g] - '0'] =
                                                    (flights[flight_a_index].schedule[g] - '0') + 1;
                                        }
                                    }
                                    else if (flights[flight_a_index].schedule[0] == 'D'){
                                        for (g=0; g<7; g++){
                                            days_flight_a_runs_on[g] = g+1;
                                        }
                                    }
                                    for (g=0; g<7;g++){
                                        if (days_flight_a_runs_on[g] == day || day == 0)
                                            flight_a_fly_status = YES;
                                    }

                                    // Find a flight corresponding to the second routeID
                                    for (g=0; g<3000; g++){

                                        if (routeID_2 == flights[g].routeID && flight_a_fly_status == YES){
                                            flight_num_b = flights[g].flight_number;
                                            // Check to see if the second flight flies on the specified day
                                            flight_b_fly_status = NO;
                                            for (r=1;r<=7;r++){
                                                days_flight_b_runs_on[r-1] = r;
                                            }

                                            flight_b_index = check_flight_number(flight_num_b);


                                            if (flights[flight_b_index].schedule[0] == 'X'){
                                                for (r=1; r<=7 && flights[flight_b_index].schedule[r] != '\0'; r++){
                                                    days_flight_b_runs_on[(flights[flight_b_index].schedule[r] - '0') - 1] = -1;
                                                }
                                            }
                                            else if  (flights[flight_b_index].schedule[0] == '1' ||
                                                      flights[flight_b_index].schedule[0] == '2' ||
                                                      flights[flight_b_index].schedule[0] == '3' ||
                                                      flights[flight_b_index].schedule[0] == '4' ||
                                                      flights[flight_b_index].schedule[0] == '5' ||
                                                      flights[flight_b_index].schedule[0] == '6' ||
                                                      flights[flight_b_index].schedule[0] == '7'){
                                                for (r = 0; r<7; r++){
                                                    days_flight_b_runs_on[r] = 0;
                                                }
                                                for (r=0; r<7 && flights[flight_b_index].schedule[r] != '\0'; r++){
                                                    days_flight_b_runs_on[flights[flight_b_index].schedule[r] - '0'] =
                                                            (flights[flight_b_index].schedule[r] - '0') + 1;
                                                }
                                            }
                                            else if (flights[flight_b_index].schedule[0] == 'D'){
                                                for (r=0; r<7; r++){
                                                    days_flight_b_runs_on[r] = r+1;
                                                }
                                            }
                                            for (r=0; r<7;r++){
                                                if (days_flight_b_runs_on[r] == day || day == 0)
                                                    flight_b_fly_status = YES;
                                            }

                                            if (flight_a_fly_status == YES && flight_b_fly_status == YES){
                                                //Check to see if both of these flights meet the layover criteria
                                                if ((flights[flight_b_index].time1number -
                                                     flights[flight_a_index].time2number < 200) &&
                                                    (flights[flight_b_index].time1number - flights[flight_a_index].time2number
                                                     > 100)){
                                                    flies = YES;
                                                    run++;


                                                    //Print the flight info

                                                    //Now print the flight information
                                                    if (run == 1){
                                                        printf("\n1-stop flights from %s (%s) to %s (%s):\n\n", airport_1_location, airport_1, airport_2_location,
                                                               airport_2);
                                                    }

                                                    //find the location of the random airport
                                                    for (f=0; f<100; f++) {
                                                        if (strcmp(airports[f].airport_code, airport_rando) == 0)
                                                            strcpy(airport_rando_location, airports[f].airport_location);
                                                    }


                                                    //Print the flight number of flight_a
                                                    printf("Flight %d (", flights[flight_a_index].flight_number);
                                                    for (f=0; f<7; f++){
                                                        if (days_flight_a_runs_on[f] > 0)
                                                            printf("%s", days_of_week[days_flight_a_runs_on[f]-1]);
                                                        if (f==6)
                                                            printf(")\n");
                                                    }

                                                    //Print the time of flight_a's departure
                                                    printf("    ");
                                                    if (flights[flight_a_index].time1number < 1000 ||
                                                        (flights[flight_a_index].time1number >= 1300 &&
                                                         flights[flight_a_index].time1number < 2200)){
                                                        printf("0");
                                                    }
                                                    printf("%s.m. ", flights[flight_a_index].time1);

                                                    //Print the location of the first airport
                                                    printf("%s (%s)\n    ", airport_1_location, airport_1);

                                                    //Print the time of flight_a's arrival
                                                    if (flights[flight_a_index].time2number < 1000 ||
                                                        (flights[flight_a_index].time2number >= 1300 &&
                                                         flights[flight_a_index].time2number < 2200)){
                                                        printf("0");
                                                    }
                                                    printf("%s.m. ", flights[flight_a_index].time2);

                                                    //Print the location of the random airport
                                                    printf("%s (%s)\n", airport_rando_location, airport_rando);


                                                    //Print the flight number of flight_b
                                                    printf("Flight %d (", flights[flight_b_index].flight_number);
                                                    for (f=0; f<7; f++){
                                                        if (days_flight_b_runs_on[f] > 0)
                                                            printf("%s", days_of_week[days_flight_b_runs_on[f]-1]);
                                                        if (f==6)
                                                            printf(")\n");
                                                    }

                                                    //Print the time of flight_b's departure
                                                    printf("    ");
                                                    if (flights[flight_b_index].time1number < 1000 ||
                                                        (flights[flight_b_index].time1number >= 1300 &&
                                                         flights[flight_b_index].time1number < 2200)){
                                                        printf("0");
                                                    }
                                                    printf("%s.m. ", flights[flight_b_index].time1);

                                                    //Print the location of the random airport
                                                    printf("%s (%s)\n    ", airport_rando_location, airport_rando);

                                                    //Print the time of flight_b's arrival
                                                    if (flights[flight_b_index].time2number < 1000 ||
                                                        (flights[flight_b_index].time2number >= 1300 &&
                                                         flights[flight_b_index].time2number < 2200)){
                                                        printf("0");
                                                    }
                                                    printf("%s.m. ", flights[flight_b_index].time2);

                                                    //Print the location of the second airport
                                                    printf("%s (%s)\n\n", airport_2_location, airport_2);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (i>=499 && flies == NO){
                    printf("Route %s to %s has no 1-stop flights on the specified days\n", airport_1, airport_2);
                }
            }
            print_menu = YES;
            continue;
        }



        if (option == 5){
            exit (0);
        }
    }
    return 0;
}

//Function definitions (Ideally these would be placed into a separate .c file)
int string_to_int(char string[]){
    int i;
    int n = 0;
    for (i = 0; i < strlen(string); i++){

        if (string[i] >= '0' && string[i] <= '9')
            n = (10 * n) + (string[i] - '0');
        else continue;

    }
    return n;
}

int check_flight_number(int flight_number_entered){
    int i;
    for (i = 0; i < 3000; i++){
        if (flights[i].flight_number == flight_number_entered)
            return i; //returns the index in flights[] where the flight number entered was found
    }
    return INVALID;
}


int check_airport_code(char airport_entered[]){
    int i;
    for (i=0; i<100; i++){
        if (strcmp(airports[i].airport_code, airport_entered) == 0)
            return i; //returns the index in airports[] whee the airport entered was found
    }
    return INVALID;
}
