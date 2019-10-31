#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>

#define PORT 5000

#define N 10
#define MAX_WORD_SIZE 80
#define MAX_TEL_SIZE 12

int new_socket;
char buffer[1024] = {0}; 

typedef struct telephone{
    char telefone[MAX_TEL_SIZE];
} Telephone ;

typedef struct telephones{
    Telephone teleph[N];
}Telephones ;

typedef struct contact{
    char id[N];
    char name[80];
    char email[80];
    char address[80];
    Telephones *tel;

}Contact;


int removeContact(char *id){

}
int updateContact(char *id){

}
int listContact(char *id){
    
}

int agendaExists(){
    FILE *fp = fopen("agenda.csv","r");
    if(fp == NULL){
        return 0;
    }else{
        return 1;
    }
}

FILE * getAgenda(){
    return fopen("agenda.csv","a");
}

FILE * createAgenda(){
    FILE* f = fopen("agenda.csv","a");
    fprintf(f,"ID;NAME;EMAIL;TELEPHONES;ADDRESS\n");
    fclose(f);
    return getAgenda();
}

int addNewContact(Contact * contact){    
    return saveContact(contact);
}

int saveContact(Contact *con){
    FILE * agenda = getAgenda();
    if(agenda!=NULL){
        char *tel = malloc(sizeof(char)*N*MAX_TEL_SIZE);
        int i = 0;
        int j = 0;
        int k = 0;
        for (i = 0; i < 2; i++){
            for (j = 0; j < strlen(con->tel->teleph[i].telefone) ; j++){
                    tel[k] = con->tel->teleph[i].telefone[j];
                    k++;
            }
            tel[k++] = ',';
        }

        tel[strlen(tel)-1] = '\0';

        fprintf(agenda,"%s,%s;%s;[%s];%s\n",con->id,con->name,con->email, tel, con->address);

        fclose(agenda);
        return 1;
    }else{
        printf("**ERROR** : There was some error while saving the data. Has agenda been created?");
        return -1;
    }
}

char *addToWord(char *word,char *message, int pos){
    int size = strlen(word);
    if(size < MAX_WORD_SIZE)
        word[size] = message[pos];
        
    return word;    
}

Contact *normalizeMessageIntoParams(char *message){
    
    if(message[0] == ";") return -1;
    
    int i = 1, DELIMITER_COUNTER = 0;
    int CSV_TEL_DELIMITER_LIMIT = 3;
    int TEL_COUNT = 0;
    char *word = NULL;
    Telephones *telephones = malloc(sizeof(Telephones));
    Telephone *telephone = malloc(sizeof(Telephone));
    Contact *contact = malloc(sizeof(Contact));
    
    while(message[i] != '\0'){
        if(word == NULL){
            word = malloc(sizeof(char) * MAX_WORD_SIZE);
        }

        if(DELIMITER_COUNTER == CSV_TEL_DELIMITER_LIMIT){
                // Start to build telephones 
                if(message[i] != '[' && message[i] != ']' && message[i] != ','){
                    word = addToWord(word, message, i);
                    //strcat(word,message[i]);
                }else{
                    if(message[i] == '['){
                        word = malloc(sizeof(char) * MAX_WORD_SIZE);
                    }else if(message[i] == ']'){
                        strcpy(telephone->telefone,word);
                        telephones->teleph[TEL_COUNT++] = *telephone;
                        free(word);
                        i++;
                    }else if(message[i] == ','){
                        strcpy(telephone->telefone,word);
                        telephones->teleph[TEL_COUNT++] = *telephone;
                        word = malloc(sizeof(char) * MAX_WORD_SIZE);
                    }
                }
    
        }

        if(message[i] == ';'){
            if(DELIMITER_COUNTER == 0){
                // printf("ID : %s \n ", word);
                strcpy(contact->id,word);
                word = malloc(sizeof(char));
                
            }else if(DELIMITER_COUNTER == 1){
                // printf("Nome : %s \n ", word);
                strcpy(contact->name,word);
                word = malloc(sizeof(char));
            }else if(DELIMITER_COUNTER == 2){
                //  printf("Email : %s \n ", word);
                strcpy(contact->email,word);
                word = malloc(sizeof(char));

            }else if(DELIMITER_COUNTER == 3){
                // int j;
                // for (j = 0; j < N ; j++){
                //     Telephone tel = telephones->teleph[j];
                //     if(!strlen(tel.telefone) == 0){
                //         printf("Tel %d : %s \n",j+1,tel.telefone);
                //     }                
                // }
                contact->tel = telephones;
            }else if(DELIMITER_COUNTER == 4){
                strcpy(contact->address, word);
                // printf("Endereco : %s",contact->address);
            }

            DELIMITER_COUNTER++;
                
        }else{
        
            if(DELIMITER_COUNTER != CSV_TEL_DELIMITER_LIMIT)
                word = addToWord(word, message, i);
        }
        
         i++;  
     }
    
    return contact;
}


void init(char *message){
    char c = message[0];

    Contact *contact;
    switch (c)
    {
    case '+':
        // Adiciona novo contato
        //printf("%s",message);
        contact = normalizeMessageIntoParams(message);
        if(addNewContact(contact)){
            // mandar mensagem de successo para o cliente por uma das threads e usando o socket.
        }else{
            // mandar mensagem de erro para o cliente por uma das threads e usando o socket.
        }

        break;
    case 2:
        // remove contato
        if(removeContact("gerar_id_para_busca_aqui")){
            // mandar mensagem de successo para o cliente por uma das threads.
        }else{
            // mandar mensagem de erro para o cliente por uma das threads;
        }

        break;    
    case 3:
        // Atualiza contato
        if(updateContact("gerar_id_para_busca_aqui")){
            // mandar mensagem de successo para o cliente por uma das threads.
        }else{
            // mandar mensagem de erro para o cliente por uma das threads;
        }

        break;

    case 4:
        // Lista contato
        if(listContact("gerar_id_para_buscar_aqui")){
            // mandar mensagem de successo para o cliente por uma das threads.
        }else{
            // mandar mensagem de erro para o cliente por uma das threads;
        }

        break;    

    default:
        break;
    }
    

    return;
}

void initSocket(){

    int server_fd , valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    
    //send(new_socket , hello , strlen(hello) , 0 );
}

int getMessageFromClient(){
    
    int valread = read( new_socket , buffer, 1024);
    init(buffer);
    
}

int main(){

    
    printf("Loading Agenda...\n");
    if(!agendaExists()){


        FILE * agenda = createAgenda();
        if(agenda == NULL){
            printf("**ERROR** - Agenda couldn't be created \n\n");
            return -1;
        }else{
            printf("Agenda initiated successfuly \n\n");
            initSocket();
        }


    }else{
        printf("Load Complete...\n");
        initSocket();
        
        
        // This could be inside a thread 
        getMessageFromClient();

    }

   
    


}