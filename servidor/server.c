#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 10


typedef struct telephone{
    char telefone[20];
} Telephone ;

typedef struct telephones{
    Telephone teleph[N];
}Telephones ;

typedef struct contact{
   
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
    return fopen("agenda.csv","a");
}

int addNewContact(char *name, char* email, Telephones *t, char* address){
    Contact *c = malloc(sizeof(Contact));
    Telephones *tel = malloc(sizeof(Telephones));

    strcpy(c->name,name);
    strcpy(c->address,address);
    strcpy(c->email,email);
    tel = t;
    c->tel = tel;

    
    return saveContact(c);
    
}

int saveContact(Contact *con){
    FILE * agenda = getAgenda();
    if(agenda!=NULL){
        fprintf(agenda,"%s;%s;%s\n",con->name,con->email, con->address);

        fclose(agenda);
        return 1;
    }else{
        printf("**ERROR** : There was some error while saving the data. Has agenda been created.");
        return -1;
    }
}

void init(){
    int id = 1;
    switch (id)
    {
    case 1:
        // Adiciona novo contato
        if(addNewContact("ayrton","ayrton@gmail.com",NULL,"frei vidal da penha")){
            // mandar mensagem de successo para o cliente por uma das threads.
        }else{
            // mandar mensagem de erro para o cliente por uma das threads;
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


int main(){

    if(!agendaExists()){


        FILE * agenda = createAgenda();
        if(agenda == NULL){
            printf("**ERROR** - Agenda couldn't be created \n\n");
            return -1;
        }else{
            printf("Agenda initiated successfuly \n\n");
            init();
        }


    }else{
        printf("Carregando Agenda...\n");
        init();
    }

   
    


}