#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#define PORT 5000

// Characters styles
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"

// Information regarding the telephone size.
#define MAX_TEL_SIZE 12
#define N 10

int sock = 0;
char bufferServerMessage[1024] = {0}; 

typedef struct telephone{
    char telefone[MAX_TEL_SIZE];
} Telephone ;

typedef struct telephones{
    Telephone teleph[N];
}Telephones ;

int insertContact(){  

    printf("=========================== INSTRUCTIONS OF USAGE ======================\n\n ");
    printf("* Person's name is required.\n ");
    printf("* Person's email is not required, if you want to leave it blank just hit SPACE\n ");
    printf("* Person's telephones are required.\n ");
    printf("* Person's address is not required, if you want to leave it blank just hit SPACE\n ");
    printf("========================================================================\n\n ");

    char *name = malloc(sizeof(char)*80); 
    char *email = malloc(sizeof(char)*80);
    char *address = malloc(sizeof(char)*80);
    char *telephone = malloc(sizeof(char)*80);

    fflush(stdin);
    do{
        printf("Name (*) : ");
        scanf("%[^\n]", name);
        scanf("%*[^\n]"); scanf("%*c");
    }while(strlen(name) <= 0);
    
    printf("Email : ");
    scanf("%[^\n]", email);
    scanf("%*[^\n]"); scanf("%*c");

    int number;

    do{
        printf("%s How many telephones do you want to insert? : ",KNRM);
        scanf("%d", &number);
        scanf("%*[^\n]"); scanf("%*c");
        if(number <= 0){
            printf("%s You must inform at least one telephone number.\n\n",KRED);
        }else if(number > N){
            printf("%s The number of telephones cannot be higher than the limit (%d).\n\n",KRED,N);

        }
    }while(number <= 0 || number > N);
    
    // Inserting telephones somewhere.
    int counter = 0; 

    char* telString = malloc(sizeof(char) * number * MAX_TEL_SIZE);
    while(counter < number){
        Telephone *tel = malloc(sizeof(Telephone));
        printf("Insert telephone number (%d)",counter+1);
        // scanf("%s",tel->telefone);
        scanf("%[^\n]", tel->telefone);
        scanf("%*[^\n]"); scanf("%*c");
        if(counter + 1 == number)
            strcat(telString,tel->telefone);
        else{
            strcat(telString,tel->telefone);    
            strcat(telString,",");
        }
        counter++;
        free(tel);
    }
    
    printf("What's your address: ");
    scanf("%[^\n]", address);
    scanf("%*[^\n]"); scanf("%*c");

    char* message = malloc(sizeof(char) * 1024);
    strcat(message,"+23131;");
    strcat(message,name);
    strcat(message,";");
    strcat(message,email);
    strcat(message,";");
    strcat(message,"[");
    strcat(message,telString);
    strcat(message,"]");
    strcat(message,";");
    strcat(message,address);
    strcat(message,";");

    return send(sock , message , strlen(message) , 0 );

}

void menu(){

    int op;
    printf("=========================== SOCKETS AGENDA ======================\n\n ");
    printf("1 - INSERT NEW CONTACT \n ");
    printf("2 - LIST EXISTING CONTACT \n ");
    printf("3 - DELETE A CONTACT \n ");
    printf("4 - UPDATE AN EXISTING CONTACT \n ");
    printf("=================================================================\n\n ");

    scanf("%d",&op);
    switch (op)
    {
    case 1:
        if(insertContact()){
            // if(read( sock , bufferServerMessage, 1024)){
            //     printf("Server says : %s", bufferServerMessage);
            // }
        }
        break;
    
    default:
        break;
    }

}

int createSocket(){
    int serverMsg; 
    struct sockaddr_in serv_addr; 
    

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n Erro na criação do socket \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nEndereço Inválido/ Endereços não são suportados. \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\n Conexao falhou \n"); 
        return -1; 
    }

    return 1;
}

int main(int argc, char **argv)
{
    if(createSocket()){
        printf("Socket initialized sucessfully...");
        printf("Listening to port %d...\n\n",PORT);
        menu();
    }
    
    

    // serverMsg = read( sock , bufferServerMessage, 1024); 



}