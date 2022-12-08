#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Customer{
    char* name;
    struct DateOfBirth{
        int day;
        int month;
        int year;
    } dob;
    int passportNumber;
    char* address;
    long long phoneNumber;
    char* accountType;
    double balance;
};


int menu(struct Customer* i);
void new_acc(struct Customer* i);
void view_list();
void edit();
void transact();
void erase(struct Customer* i);
void see_account();

void main(){
    struct Customer customerInfo;
    struct Customer* custInfo = &customerInfo;

    custInfo->name = (char*)malloc(50*sizeof(char));
    custInfo->address = (char*)malloc(50*sizeof(char));
    custInfo->accountType = (char*)malloc(20*sizeof(char));

    if(!custInfo->name || !custInfo->address || !custInfo->accountType){
        perror("Unable to allocate memory to either name, address, account type, or a mix of them.");
        free(custInfo->name);
        free(custInfo->address);
        free(custInfo->accountType);
        exit(1);
    }

    system("cls");
    menu(custInfo);

    free(custInfo->name);
    free(custInfo->address);
    free(custInfo->accountType);
    
    return;
}


int menu(struct Customer* i){ //This function displays the menu or welcome screen to perform different banking activities mentioned below.
    printf("Customer Account Management System\n");
    printf("Welcome to the main menu.\n");
    printf("1. Create New Account\n2. Update Information on Existing Account\n3. Make a Transaction\n4. Check Existing Account Details\n5. Remove Existing Account\n6. View Customer List\n7. Exit\n");

    int choice;
    printf("Enter choice number: ");
    scanf("%d", &choice);
    
    system("cls");
    fflush(stdin);

    switch (choice){
        case 1: //!done
            new_acc(i);
            system("cls");
            menu(i);

        case 2: //!done
            edit();
            system("cls");
            menu(i); 
        
        case 3:
            transact(); //!done
            system("cls");
            menu(i);

        case 4: //!done
            see_account();
            system("cls");
            menu(i);

        case 5:
            erase(i); //!done
            system("cls");
            menu(i);

        case 6:
            view_list(); //!done
            system("cls");
            menu(i);

        default:
            exit(0);
            
    }

}


void new_acc(struct Customer* i){
    printf("Enter your full name: ");
    fgets(i->name, 100, stdin);
    i->name[strlen(i->name) - 1] = '\0'; fflush(stdin);

    printf("Enter your date of birth in the following format: dd/mm/yyyy: ");
    scanf("%d/%d/%d", &i->dob.day, &i->dob.month, &i->dob.year); fflush(stdin);
    
    printf("Enter your passport number: ");
    scanf("%d", &i->passportNumber); fflush(stdin);

    char address[50];
    printf("Enter your address: ");
    fgets(i->address, 50, stdin);
    i->address[strlen(i->address) - 1] = '\0'; fflush(stdin);

    printf("Enter your phone number: ");
    scanf(" %lld", &i->phoneNumber); fflush(stdin);
    
    printf("Do you want a savings or current account (savings/current)?: ");
    scanf("%s", i->accountType); fflush(stdin);

    i->balance=0.0;


    FILE* fp;
    fp = fopen("accounts.txt", "a");
    
    char* accountInfo = (char*)malloc(100*sizeof(char));
    if(!accountInfo){
        perror("Failed to allocate memory for account info.");
        free(accountInfo);
        exit(1);
    }

    fprintf(fp, "%s,%d/%d/%d,%s,%d,%lld,%s,%f\n", i->name, i->dob.day, i->dob.month, i->dob.year, i->address, i->passportNumber, i->phoneNumber, i->accountType, i->balance); // writes all
    
    printf("Account added.\n");
    printf("Press Any Key to Continue\n");
    getchar();
    
    fclose(fp);
    return;
}


void see_account(){
    fflush(stdin);

    char accountHolderName[50];
    printf("Enter full name: ");
    fgets(accountHolderName, 50, stdin); fflush(stdin); // by this point, user's name has been collected
    accountHolderName[strlen(accountHolderName) - 1] = '\0';


    FILE *fp;
    fp = fopen("accounts.txt", "r");
    if(!fp){
        perror("Failed to open file.");
        fclose(fp);
        exit(1);
    } // file has been opened

    char chunk[128];
    size_t len = sizeof(chunk); 
    char* line = malloc(len);
    if(!line){
        perror("Unable to allocate memory for line buffer.");
        free(line);
        exit(1);
    }
    line[0] = '\0'; // line buffer has been set up

    int match=0;
    while(fgets(chunk, sizeof(chunk), fp) != NULL){
        if(len - strlen(line) < sizeof(chunk)){
            len *= 2;
            if(!(line = realloc(line, len))){
                perror("Unable to reallocate memory for line buffer.");
                free(line);
                exit(1);
            } // if length of line is smaller than chunk size, then line buffer must be enlarged

        }
        //write what you want to do with each line below
        strcat(line, chunk);
        char lineBuffer[len];

        if(line[strlen(line) - 1] == '\n'){
            strncpy(lineBuffer, line, len);
            char* token = strtok(line, ",");
            if(strcmp(token, accountHolderName) == 0){
                match = 1;
                printf("Name, Account, DOB, Address, Passport Number, Phone Number, Account Type, Balance\n");
                printf("%s", lineBuffer);
            }
            line[0] = '\0';
        }
    }

    if(match==0) printf("Account not found. Try again.\n");

    printf("Press Any Key to Continue\n");
    getchar();

    fclose(fp);
    free(line); 

    return;
}


void edit(){
    fflush(stdin);

    char accountHolderName[50];
    printf("Enter full name: ");
    fgets(accountHolderName, 50, stdin);
    accountHolderName[strlen(accountHolderName)-1]='\0'; fflush(stdin);
    int match=0;

    FILE* fp, *fp2;
    fp = fopen("accounts.txt", "r+");
    if(!fp){
        perror("Unable to open file for account editing.");
        fclose(fp);
        exit(1);
    }

    fp2 = fopen("clone.txt", "w+");
    if(!fp2){
        perror("Unable to open file to clone account names into.");
        fclose(fp2);
        exit(1);
    }

    char chunk[128];
    size_t len = sizeof(chunk);
    char* line = malloc(len);
    if(!line){
        perror("Unable to allocate memory for line buffer.");
        free(line);
        exit(1);
    }
    line[0] = '\0';

    while(fgets(chunk, sizeof(chunk), fp) != NULL){

        if(len - strlen(line) < sizeof(chunk)){
            len *= 2;
            if(!(line = realloc(line, len))){
                perror("Unable to reallocate memory for line buffer.");
                free(line);
                exit(1);
            }
        }
    
        strncpy(line, chunk, len);
        char lineBuffer[len];

        if(line[strlen(line) - 1] == '\n'){
            strncpy(lineBuffer, line, len);
            char* token = strtok(line, ",");

            if(strcmp(token, accountHolderName) == 0){
                match = 1;
                int choice;
                printf("Enter the option you want to change - Name(1), DOB(2), Address(3), Passport Number(4), Phone Number(5), Account Type(6): ");
                scanf("%d", &choice); fflush(stdin);
                
                char* newDetailsLine = malloc(len); newDetailsLine[0] = '\0';
                // printf("Token = %s\n", token);

                for(int i=1; token != NULL && i<choice; i++){
                    strncat(newDetailsLine, token, strlen(token)); strcat(newDetailsLine, ",");
                    token = strtok(NULL, ",");
                }

                char newDetail[50];
                switch(choice){
                    case 1:
                        printf("Enter new name: ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;

                    case 2:
                        printf("Enter new DOB (dd/mm/yyyy): ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;

                    case 3:
                        printf("Enter new address: ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;

                    case 4:
                        printf("Enter new passport number : ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;

                    case 5:
                        printf("Enter new phone number: ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;

                    case 6:
                        printf("Enter new account type: ");
                        fgets(newDetail, 50, stdin); newDetail[strlen(newDetail) - 1] = '\0'; fflush(stdin);
                        strncat(newDetailsLine, newDetail, strlen(newDetail));
                        break;
                }
                strcat(newDetailsLine, ",");

                token = strtok(NULL, ",");
                while(token != NULL){
                    strncat(newDetailsLine, token, strlen(token)); strcat(newDetailsLine, ",");
                    token = strtok(NULL, ",");
                    // if(token != NULL) break;
                }

                newDetailsLine[strlen(newDetailsLine) - 1] = '\0';

                // printf("Token = %s\n", token);
                // printf("Account details updated: %s", newDetailsLine);
                fprintf(fp2, newDetailsLine);

            }

            else fprintf(fp2, lineBuffer);
            line[0]='\0';
            
        }
    }

    if(match == 0){
        printf("Account not found.\n");
        edit();
    }

    printf("Account updated.\n");
    printf("Press Any Key to Continue\n");
    getchar();

    fclose(fp);fclose(fp2);
    if(remove("accounts.txt") != 0) perror("Unable to delete original file");
    rename("clone.txt", "accounts.txt");

    free(line);
}


void transact(){
    char accountHolderName[50];
    printf("Enter full name: ");
    fgets(accountHolderName, 50, stdin);
    accountHolderName[strlen(accountHolderName) -1 ] = '\0';
    fflush(stdin);

    FILE* fp = fopen("accounts.txt", "r+");
    if(!fp){
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    FILE *fp2 = fopen("clone.txt", "w+");
    if(!fp2){
        perror("Unable to open file for transaction updating");
        fclose(fp2);
        exit(1);
    }

    char chunk[128];
    size_t len = sizeof(chunk);
    char* line = malloc(len);
    if(!line){
        perror("Unable to allocate memory for line buffer");
        free(line);
        exit(1);
    }
    line[0] = '\0';

    int match = 0;
    while(fgets(chunk, sizeof(chunk), fp) != NULL){
        if(len - strlen(line) < sizeof(chunk)){
            len *= 2;
            if(!(line = realloc(line, len))){
                perror("Unable to reallocate memory for line buffer");
                free(line);
                exit(1);
            }
        }

        strncat(line, chunk, len);

        if(line[strlen(line) - 1] == '\n'){
            char lineBuffer[len];
            lineBuffer[0] = '\0';
            strncat(lineBuffer, line, len);

            char* token = strtok(line, ",");

            if(strcmp(token, accountHolderName) == 0){
                match = 1;
                char updatedLine[len];
                updatedLine[0] = '\0';

                while(token[strlen(token) - 1] != '\n'){
                    strncat(updatedLine, token, strlen(token));
                    strcat(updatedLine, ",");
                    token = strtok(NULL, ",");
                }

                token[strlen(token) - 1] = '\0';
                float accountBalance = (float) atoi(token);

                int transactionChoice;
                printf("Do you want to withdraw (1) or deposit (2) money?: ");
                scanf("%d", &transactionChoice);
                fflush(stdin);

                while(transactionChoice != 1 && transactionChoice != 2){
                    printf("Invalid Input. Would you like to withdraw (1) or deposit (2) money?: ");
                    scanf("%d", &transactionChoice);
                    fflush(stdin);
                }

                if(transactionChoice == 1){
                    if(accountBalance == 0){
                        printf("Insufficient funds.\n");
                        break;
                    }

                    float amountToWithdraw;
                    printf("Enter amount you would like to withdraw: ");
                    scanf("%f", &amountToWithdraw);
                    fflush(stdin);

                    while(accountBalance - amountToWithdraw < 0){
                        printf("Insufficient Funds. Enter a valid amount to withraw: ");
                        scanf("%f", amountToWithdraw);
                        fflush(stdin);
                    }

                    accountBalance -= amountToWithdraw;
                    printf("Money wihtdrawn. Remaining balance: %f\n", accountBalance);

                }

                else if(transactionChoice == 2){
                    float amountToDeposit;
                    printf("Enter amount you would like to deposit: ");
                    scanf("%f", &amountToDeposit);
                    fflush(stdin);

                    accountBalance += amountToDeposit;
                    printf("Money deposited. Balance: %f\n", accountBalance);
                }

                char accountBalanceString[20];
                sprintf(accountBalanceString, "%f", accountBalance);
                accountBalanceString[strlen(accountBalanceString) - 1] = '\n';
                strncat(updatedLine, accountBalanceString, strlen(accountBalanceString));

                fprintf(fp2, updatedLine);

            }

            else fprintf(fp2, lineBuffer);

            line[0] = '\0';
        }

    }

    if(match == 0){
        printf("Account not found. Try again.\n");
        transact();
    }

    printf("Press Any Key to Continue\n");
    getchar(); 
    

    fclose(fp);fclose(fp2);
    if(remove("accounts.txt") != 0) perror("Unable to delete original file");
    rename("clone.txt", "accounts.txt");
    free(line);
}


void erase(struct Customer* i){
    char accountHolderName[50];
    printf("Enter full name: ");
    fgets(accountHolderName, 50, stdin);
    accountHolderName[strlen(accountHolderName) -1 ] = '\0';
    fflush(stdin);

    FILE* fp = fopen("accounts.txt", "r+");
    if(!fp){
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    FILE *fp2 = fopen("clone.txt", "w+");
    if(!fp2){
        perror("Unable to open file for transaction updating");
        fclose(fp2);
        exit(1);
    }

    char chunk[128];
    size_t len = sizeof(chunk);
    char* line = malloc(len);
    if(!line){
        perror("Unable to allocate memory for line buffer");
        free(line);
        exit(1);
    }
    line[0] = '\0';

    int match = 0;
    while(fgets(chunk, sizeof(chunk), fp) != NULL){
        if(len - strlen(line) < sizeof(chunk)){
            len *= 2;
            if(!(line = realloc(line, len))){
                perror("Unable to reallocate memory for line buffer");
                free(line);
                exit(1);
            }
        }

        strncat(line, chunk, len);

        if(line[strlen(line) - 1] = '\n'){
            char lineBuffer[len];
            lineBuffer[0] = '\0';
            strncat(lineBuffer, line, strlen(line));

            char* token = strtok(line, ",");
            if(strcmp(token, accountHolderName) == 0){
                match = 1;

                while(token[strlen(token) - 1] != '\n') token = strtok(NULL, ",");

                float accountBalance = (float) atoi(token);
                if(accountBalance > 0){
                    printf("Withdraw all money from your account before proceeding.\n");
                    printf("Press Any Key to Continue\n");
                    getchar();

                    fclose(fp);fclose(fp2);
                    if(remove("clone.txt") != 0) perror("Unable to delete clone file");
                    free(line);

                    menu(i);
                }

                char confirmation;
                printf("This cannot be undone. Would you still like to proceed (y/n)? ");
                scanf(" %c", &confirmation);
                fflush(stdin);

                while(confirmation != 'y' && confirmation != 'n'){
                    printf("Invalid input. Try again (y/n): ");
                    scanf(" %c", &confirmation);
                }

                if(confirmation == 'n') menu(i);
            }

            else fprintf(fp2, lineBuffer);
            line[0] = '\0';
        }

    }

    if(match == 0){
        printf("Account not found. Try again.\n");
        erase(i);
    }

    printf("Account deleted.\n");
    printf("Press Any Key to Continue\n");
    getchar();

    match = 0;

    fclose(fp);fclose(fp2);
    if(remove("accounts.txt") != 0) perror("Unable to delete original file");
    rename("clone.txt", "accounts.txt");
    free(line);
}


void view_list(){
    FILE* fp = fopen("accounts.txt", "r");
    if(!fp){
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char chunk[128];
    size_t len = sizeof(chunk);
    char* line = malloc(len);
    if(!line){
        perror("Unable to allocate memory for line buffer");
        free(line);
        exit(1);
    }
    line[0] = '\0';

    while(fgets(chunk, sizeof(chunk), fp) != NULL){
        if(len - strlen(line) < sizeof(chunk)){
            len *= 2;
            if(!(line = realloc(line, len))){
                perror("Unable to reallocate memory for line buffer");
                free(line);
                exit(1);
            }
        }

        strncat(line, chunk, len);

        if(line[strlen(line) - 1] == '\n'){
            printf("%s", line);
            line[0] = '\0';
        }

    }

    printf("Press Any Key to Continue\n");
    getchar();

    free(line);
    fclose(fp);

}
