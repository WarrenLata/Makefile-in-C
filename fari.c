#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <langinfo.h>
#include <jansson.h>
//#include <../json-c/json.h>

//separateur
typedef struct separateur { 
    char space[2];
    char soteLigne[3]; 
    
} Delim;

Delim separateur;

//definition des donnes extraitent apartir du fichier de description
typedef struct data{
    
    char   f_desc_name[12];
    char*  FILE_E;
    char** FILE_C;
    char** FILE_H;
    char** FILE_J;  
    char** LIBS;
    char** FLAGS;
    int file_c_size;
    int file_h_size;
    int file_b_size;
    int file_f_size;
    int file_j_size;
    time_t *file_c_mtimes;
    time_t *file_h_mtimes;
    
} F_DESC_DATA;

//store description's data 
F_DESC_DATA f_desc_data;


//------------ Les fonction utilitaire ------------


void init(){ 
    
    strcpy(separateur.soteLigne,"\n"); 
    strcpy(separateur.space," ");
    
    f_desc_data.FILE_C = (char **)malloc(sizeof(char*));
    f_desc_data.file_c_size=0;
    f_desc_data.file_c_mtimes = (time_t*)malloc(sizeof(time_t));
    
    f_desc_data.FILE_H = (char **)malloc(sizeof(char*));
    f_desc_data.file_h_size=0;
    f_desc_data.file_h_mtimes = (time_t*)malloc(sizeof(time_t));
    
    f_desc_data.LIBS = (char **)malloc(sizeof(char*));
    f_desc_data.file_b_size=0;
    
    f_desc_data.FLAGS = (char **)malloc(sizeof(char*));
    f_desc_data.file_f_size=0;
    
}
 
 
int verifierExistanceFichier(char*nomfichier){
    
	FILE *f;
    if ((f = fopen(nomfichier, "r")))
    {
        fclose(f);
        return 1;
    }
    return 0;
}


char* getNameFile(char** argument){
    
    if(strlen(argument[1])>0) return argument[1];
    else return strcat(argument[0],"Make.txt");
}

// void afficheMots(char**mots,int n,char*title){
//     printf("\n\nfiles c :\n");
//     for(int i=0;i<n;i++)printf("%s " ,mots[i]);
// }

int createOFile(char*nameFile){
    
    puts(nameFile);
   
    pid_t pid;
    
     char **argvopt;

   int p;
   
    if((pid=fork())==0){
        p=0;
      
  //   char **argvopt;//etap fiha compilation --> reservation 
      

        int size=6+f_desc_data.file_f_size;

       argvopt=(char**) malloc((size)*sizeof(char*));

       argvopt[0]=(char*) malloc(4*sizeof(char));
       strcpy( argvopt[0],"gcc");

        for(int i=0;i<f_desc_data.file_f_size;i++) {

        argvopt[i+1]=(char*) malloc(strlen(f_desc_data.FLAGS[i])*sizeof(char));
         strcpy(argvopt[i+1],f_desc_data.FLAGS[i]);
        }

       argvopt[size-5]=(char*) malloc(3*sizeof(char));
       strcpy( argvopt[size-5],"-c");

       argvopt[size-4]=(char*) malloc(strlen(nameFile)*sizeof(char));
       strcpy( argvopt[size-4], nameFile);

       argvopt[size-3]=(char*) malloc(3*sizeof(char));
       strcpy( argvopt[size-3],"-o");

       argvopt[size-2]=(char*) malloc(strlen(nameFile)*sizeof(char));
       strcpy( argvopt[size-2], strcat(strtok(nameFile,"c"),"o"));

    

     


         argvopt[size-1]=(char*) malloc(1*sizeof(char));

        argvopt[size-1]=NULL;


       //      for(int i=0;i<size;i++){

       // printf("%d\n",argvopt[i] );
       //  }

           
            p =  execvp("gcc",argvopt);

           if(p==-1) return 0;
           else return 1;
      
        exit(0);
    


 

   } 
   
    else{
            waitpid(pid,0,0);
            return 1;
    }

    
}



// int editlink(char*nameFile){
    
//     puts(nameFile);
   
//     pid_t pid;
    
//      char **argvopt;

//    int p;
   
//     if((pid=fork())==0){
//         p=0;
      
//   //   char **argvopt;//etap fiha compilation --> reservation 
      

//         int size=7+f_desc_data.file_f_size;

//        argvopt=(char**) malloc((size)*sizeof(char*));

//        argvopt[0]=(char*) malloc(4*sizeof(char));
//        strcpy( argvopt[0],"gcc");

//         for(int i=0;i<f_desc_data.file_f_size;i++) {

//         argvopt[i+1]=(char*) malloc(strlen(f_desc_data.FLAGS[i])*sizeof(char));
//          strcpy(argvopt[i+1],f_desc_data.FLAGS[i]);
//         }

//        argvopt[1+f_desc_data.file_f_size]=(char*) malloc(3*sizeof(char));
//        strcpy(argvopt[1+f_desc_data.file_f_size],"-o");

//        argvopt[2+f_desc_data.file_f_size]=(char*) malloc(strlen(f_desc_data.FILE_E)*sizeof(char));
//        strcpy( argvopt[2+f_desc_data.file_f_size],f_desc_data.FILE_E);

//        argvopt[3+f_desc_data.file_f_size]=(char*) malloc(strlen(nameFile)*sizeof(char));
//        strcpy( argvopt[3+f_desc_data.file_f_size],nameFile);



//         for(int i=0;i<f_desc_data.file_c_size;i++) {

//         argvopt[i+4+f_desc_data.file_f_size]=(char*) malloc(strlen(f_desc_data.FILE_C[i])*sizeof(char));
//          strcpy(argvopt[i+4+f_desc_data.file_f_size],strcat(strtok(f_desc_data.FILE_C[i],"c"),"o");
//         }


      

    

     


//          argvopt[size-1]=(char*) malloc(1*sizeof(char));

//         argvopt[size-1]=NULL;


//        //      for(int i=0;i<size;i++){

//        // printf("%d\n",argvopt[i] );
//        //  }

           
//             p =  execvp("gcc",argvopt);

//            if(p==-1) return 0;
//            else return 1;
      
//         exit(0);
    


 

//    } 
   
//     else{
//             waitpid(pid,0,0);
//             return 1;
//     }

    
// }






void generateExecFile(){
    
   
    pid_t pid;
  
    char **argvopt;

   
    if((pid=fork())==0){
      
  //   char **argvopt;//etap fiha compilation --> reservation 
      
   // gcc [flags] -o executable_name [.o files] [librairies]

        int size=4+f_desc_data.file_f_size+f_desc_data.file_c_size+f_desc_data.file_b_size;

       argvopt=(char**) malloc((size)*sizeof(char*));

       argvopt[0]=(char*) malloc(4*sizeof(char));
       strcpy( argvopt[0],"gcc");

        for(int i=0;i<f_desc_data.file_f_size;i++) {

        argvopt[i+1]=(char*) malloc(strlen(f_desc_data.FLAGS[i])*sizeof(char));
         strcpy(argvopt[i+1],f_desc_data.FLAGS[i]);
        }

       argvopt[f_desc_data.file_f_size+1]=(char*) malloc(3*sizeof(char));
       strcpy( argvopt[1+f_desc_data.file_f_size],"-o");

       argvopt[f_desc_data.file_f_size+2]=(char*) malloc(strlen(f_desc_data.FILE_E)*sizeof(char));
       strcpy( argvopt[f_desc_data.file_f_size+2], f_desc_data.FILE_E);

       
       for(int i=0;i<f_desc_data.file_c_size;i++) {

        argvopt[i+f_desc_data.file_f_size+3]=(char*) malloc(strlen(f_desc_data.FILE_C[i])*sizeof(char));
         strcpy(argvopt[i+f_desc_data.file_f_size+3],strcat(strtok(f_desc_data.FILE_C[i],"c"),"o"));
        }

        for(int i=0;i<f_desc_data.file_b_size;i++) {

        argvopt[i+f_desc_data.file_f_size+3+f_desc_data.file_c_size]=(char*) malloc(strlen(f_desc_data.LIBS[i])*sizeof(char));
         strcpy(argvopt[i+f_desc_data.file_f_size+3+f_desc_data.file_c_size],f_desc_data.LIBS[i]);
        }
     

      

    

     


         argvopt[size-1]=(char*) malloc(1*sizeof(char));

        argvopt[size-1]=NULL;


      
             execvp("gcc",argvopt);
        
        exit(0);
          for(int i=0;i<size;i++){

       printf("%s\n",argvopt[i] );
         }




   } 
   
    else{
        waitpid(pid,0,0);
    }

    
}




int oFiles(char* srcFile,time_t t){
    
    int h_file_exit=0;
    //if the file dosent exist 
    struct stat for_o,for_h;
    int error_compil=-1;


    //printf("%s",srcFile);
    char *file=(char *) malloc(strlen(srcFile)*sizeof(char));
    strcpy(file,srcFile);
    char *src_file_name = strtok(file,"c");
    if (stat(strcat(src_file_name,"o"), &for_o) == -1 ){
        

       // printf("%s kjhekha\n",strcat(strtok(srcFile,"o"),"c"));
    error_compil=createOFile(srcFile);
  //  printf("%s whyata %s \n ",srcFile,src_file_name);
    }
    else{
        
        int plusAncien_C = t > for_o.st_mtime;
        //sâ€™il est plus ancien que le .c 
       
        if(plusAncien_C){
            //createOFile
            error_compil= createOFile(srcFile);
        }
        else{
        
            //ou quâ€™un fichier dâ€™entË†ete quelconque
            for(int i=0;i<f_desc_data.file_h_size;i++){
                
                if(stat(f_desc_data.FILE_H[i], &for_h) != -1)
                {
                    int plusAncien_H = for_h.st_mtime > for_o.st_mtime;
                    if(plusAncien_H) h_file_exit=1;
                    
                }
            }
            //ou quâ€™un fichier dâ€™entË†ete quelconque        
            if(h_file_exit==1){
                //createOFile
                error_compil=createOFile(srcFile);
            }
        }
        
    
        
    }
return error_compil;

}


int genrateOReal(){

    for(int i=0;i<f_desc_data.file_c_size;i++) 
        if(!oFiles(f_desc_data.FILE_C[i],f_desc_data.file_c_mtimes[i])) 
            return i;
           

  generateExecFile();

  return 0;

}



int jcrJFile(char*nameFile){//creatJfile
    
  

    pid_t pid;
   
     char **argvopt;

   int p;
   
    if((pid=fork())==0){
        p=0;
      
  //   char **argvopt;//etap fiha compilation --> reservation 
      

        int size=3+f_desc_data.file_j_size;

       argvopt=(char**) malloc((size)*sizeof(char*));

       argvopt[0]=(char*) malloc(6*sizeof(char));
       strcpy( argvopt[0],"javac");

      

        for(int i=0;i<f_desc_data.file_j_size;i++) {

        argvopt[i+1]=(char*) malloc(strlen(f_desc_data.FILE_J[i])*sizeof(char));
         strcpy(argvopt[i+1],f_desc_data.FILE_J[i]);
        }



         argvopt[size-1]=(char*) malloc(1*sizeof(char));

        argvopt[size-1]=NULL;


       //      for(int i=0;i<size;i++){

       // printf("%d\n",argvopt[i] );
       //  }

           
            p =  execvp("javac",argvopt);

           if(p==-1) return 0;
           else return 1;
      
        exit(0);
    


 

   } 
   
    else{
            waitpid(pid,0,0);
            return 1;
    }

    
}

int  generateJFiles(char *srcfile){


   // int h_file_exit=0;
    //if the file dosent exist 
   // struct stat for_j;
    int error_compil=-1;


    //printf("%s",srcFile);
    char *file=(char *) malloc(strlen(srcfile)*sizeof(char));
    strcpy(file,srcfile);
   // char *src_file_name = strtok(file,"java");
  
       // printf("%s kjhekha\n",strcat(strtok(srcFile,"o"),"c"));
    error_compil=jcrJFile(srcfile);
  //  printf("%s whyata %s \n ",srcFile,src_file_name);
  
    
        
    
return error_compil;

}




void genrateJReal(){

    for(int i=0;i<f_desc_data.file_j_size;i++) 
        generateJFiles(f_desc_data.FILE_J[i]);
           
           
}


//------------- Les fonctions fonctionnel -------------
void ExtractFileDescData(){
   

    
    char* line = NULL;
  
    size_t len = 100;
    ssize_t read;
    FILE *fp = fopen (f_desc_data.f_desc_name, "r");
    //int i = 0;
    init();
    int if_line_exist[5]={0,0,0,0,0};
    int if_line_java=0;
    int error_not_foundJ=0;
   
    
    while ((read = getline (&line, &len, fp)) != -1)

    { 
      if (line != NULL){
          
            //extraire les mots de la ligne 

            char *token = strtok (line, separateur.soteLigne);//separer par \n
            char* firstWord = strtok (line, separateur.space);//separer par espace
            char* file_name = strtok(NULL,separateur.space);//mot suivant

            if(strcmp(firstWord,"E")==0){
                if_line_exist[0]=1;
                // check for errors
                if(file_name==NULL) printf("E_err_fn_notFound");
                else{
                    
                    //check for othersFileE error
                    char* other_e_name = strtok(NULL,separateur.space);
                    if(other_e_name!=NULL) printf("E_err_fn_toMany");
                    else
                    {
                        f_desc_data.FILE_E = (char*)malloc(strlen(file_name)*sizeof(char));
                        strcpy(f_desc_data.FILE_E,file_name);  
                        printf(" le nom du fichier executable : %s\n",f_desc_data.FILE_E);
                    }
                    
                }
            }
            else if(strcmp(firstWord,"C")==0){

                if_line_exist[1]=1;
                 // check for errors
                if(file_name==NULL) printf("C_err_fn_notFound");
                 else{
                    
                        struct stat buf;
                      
                        
                            do{
                                if (stat(file_name, &buf) == -1) {
                                   perror("fichier source .c not found");
                                   exit(EXIT_FAILURE);
                                }
        
                                int size = ++f_desc_data.file_c_size;
                                
                                f_desc_data.FILE_C = (char**)realloc(f_desc_data.FILE_C,size*sizeof(char*));
                                f_desc_data.FILE_C[size - 1] = (char*)malloc(strlen(file_name)*sizeof(char));
                                
                                strcpy(f_desc_data.FILE_C[size - 1],file_name);

                                    f_desc_data.file_c_mtimes = (time_t*)realloc(f_desc_data.file_c_mtimes,size*sizeof(time_t));
                                 f_desc_data.file_c_mtimes[size - 1]=buf.st_mtime;
                                

                               
                               
                                
                                file_name = strtok(NULL,separateur.space);
                                //oFiles(f_desc_data.FILE_C[size - 1],f_desc_data.file_c_mtimes[size-1]);
                               

                                
                            }while(file_name!=NULL);
                        }
                
                }
            
            else if(strcmp(firstWord,"H")==0){
               
                if_line_exist[2]=1;
                
                 // check for errors
                if(file_name==NULL) printf("H_err_fn_notFound");
                else{
                     struct stat buf;
                    
                    do{
                        if (stat(file_name, &buf) == -1) {
                            perror("stat: fichier header .h not found");
                            exit(EXIT_FAILURE);
                        }
                        
                        int size = ++f_desc_data.file_h_size;
                        
                        f_desc_data.FILE_H = (char**)realloc(f_desc_data.FILE_H,size*sizeof(char*));
                        f_desc_data.FILE_H[size - 1] = (char*)malloc(strlen(file_name)*sizeof(char));
                        
                        strcpy(f_desc_data.FILE_H[size - 1],file_name);
                        
                      //  f_desc_data.file_h_mtimes[size - 1]=buf.st_mtime;
                    //    f_desc_data.file_h_mtimes = (time_t*)realloc(f_desc_data.file_h_mtimes,size*sizeof(time_t));
                        
                        file_name = strtok(NULL,separateur.space);
                        
                    }while(file_name!=NULL);
                }
            }
            else if(strcmp(firstWord,"B")==0){
               
                if_line_exist[3]=1;
                
                 // check for errors
                if(file_name==NULL) printf("B_err_libs_notFound");
                else{
                    
                    do{
                        
                        int size = ++f_desc_data.file_b_size;
                        f_desc_data.LIBS = (char**)realloc(f_desc_data.LIBS,size*sizeof(char*));
                        f_desc_data.LIBS[size - 1] = (char*)malloc(strlen(file_name)*sizeof(char));
                        strcpy(f_desc_data.LIBS[size - 1],file_name);
                        file_name = strtok(NULL,separateur.space);
                        
                    }while(file_name!=NULL);
                }
            }
            else if(strcmp(firstWord,"F")==0){
              
                if_line_exist[4]=1;
                
                 // check for errors
                if(file_name==NULL) printf("F_err_flags_notFound");
                else{
                    
                    do{
                        
                        int size = ++f_desc_data.file_f_size;
                        f_desc_data.FLAGS = (char**)realloc(f_desc_data.FLAGS,size*sizeof(char*));
                        f_desc_data.FLAGS[size - 1] = (char*)malloc(strlen(file_name)*sizeof(char));
                        strcpy(f_desc_data.FLAGS[size - 1],file_name);
                        file_name = strtok(NULL,separateur.space);
                        
                    }while(file_name!=NULL);
                }
            }
 
            else if(strcmp(firstWord,"J")==0 && if_line_exist[2]==0 && if_line_exist[1]==0){
              
                if_line_java=1;
                
                 // check for errors
                if(file_name==NULL) error_not_foundJ=1;
                else{
                    
                    do{
                        
                        int size = ++f_desc_data.file_j_size;
                        f_desc_data.FILE_J = (char**)realloc(f_desc_data.FILE_J,size*sizeof(char*));
                        f_desc_data.FILE_J[size - 1] = (char*)malloc(strlen(file_name)*sizeof(char));
                        strcpy(f_desc_data.FILE_J[size - 1],file_name);
                        file_name = strtok(NULL,separateur.space);
                        
                    }while(file_name!=NULL);
                }
            }





      }


    }

    if(if_line_exist[1]==0 && if_line_exist[2]==0 && if_line_java==1)
        if(error_not_foundJ==1)
            printf("J_err_line_notFound");

            else    genrateJReal();
    else


     genrateOReal();
    
    


    //check for existance of files 
  
    
    if(if_line_exist[0]==0){printf(" E_err_line_notFound\n");}
    if(if_line_exist[1]==0){printf(" C_err_line_notFound\n");}
    if(if_line_exist[2]==0){printf(" H_err_line_notFound\n");}
    if(if_line_exist[3]==0){printf(" B_err_line_notFound\n");}
    if(if_line_exist[4]==0){printf(" F_err_line_notFound\n");}

    //test section
    // afficheMots(f_desc_data.FILE_C,f_desc_data.file_c_size,"c");
     //afficheMots(f_desc_data.FILE_H,f_desc_data.file_h_size,"h");
    // afficheMots(f_desc_data.LIBS,f_desc_data.file_b_size,"lib");
    // afficheMots(f_desc_data.FLAGS,f_desc_data.file_f_size,"flags");
    




                            
                        
                         
                
}


// void jsonFile(){


// char* s = NULL;



//        json_t *root= json_object();
//          json_t *root_compilation=json_object();
//          json_t *root_linking=json_object();

//          json_t *json_arrTabc=json_array();
//          // json_t *json_arrTabH=json_array();
//          // json_t *json_arrTabF=json_array();
//          // json_t *json_arrTabB=json_array();

//         // json_t*json_arrCommands=json_array();
  
//          // struct stat buf;

//         for(int i =0;i<f_desc_data.file_c_size;i++){
//         json_array_append(json_arrTabc,json_string(f_desc_data.FILE_C[i]));
//     }




//         json_object_set_new(root,"sources",json_arrTabc);

  




  
 
  
 
  
  
// json_dump_file(root,"logs.json",JSON_INDENT(1));



// }




// void jsonFile(){


//   /*Creating a json object*/
//   json_object * jobj = json_object_new_object();

//   /*Creating a json string*/
//   json_object *executable = json_object_new_string("E");
//    json_object *Fari_error = json_object_new_string("Firari error");
//     json_object *fari_msg = json_object_new_string("wahyata ya fari ");
  


//   json_object *file_c = json_object_new_array();
//   json_object *file_h = json_object_new_array();
//   json_object *file_b = json_object_new_array();
//   json_object *file_f = json_object_new_array();
//   /*Creating json strings*/
//   json_object *s1 = json_object_new_string("f.c");
//   json_object *s2 = json_object_new_string("f1.c");
//   json_object *s3 = json_object_new_string("f2.c");

//   /*Adding the above created json strings to the array*/
//   json_object_array_add(file_c,s1);
//   json_object_array_add(file_c,s2);
//   json_object_array_add(file_c,s3);

//    json_object *h = json_object_new_string("f.h");

//    json_object_array_add(file_h,h);

// json_object *b = json_object_new_string("-lm");

//    json_object_array_add(file_b,b);

//    json_object *f = json_object_new_string("-g");

//    json_object_array_add(file_f,f);

//   /*Form the json object*/
//   /*Each of these is like a key value pair*/
//   json_object_object_add(jobj,"sources", file_c);

//   json_object *n = json_object_new_string("\n");

//   json_object_object_add(jobj,"",n);

//   json_object_object_add(jobj,"headers", file_h);
//   json_object_object_add(jobj,"libraries", file_b);
//   json_object_object_add(jobj,"flags", file_f);
//   json_object_object_add(jobj,"executable_name", executable);
//    json_object_object_add(jobj,"fari_error_msg", Fari_error);
//     //json_object_object_add(jobj,"commands", jstring);
//     //json_object_object_add(jobj,"error_msg", jstring);

// //    json_object_object_add(jobj,"linking", jstring);


//     json_object_object_add(jobj,"far_msg", fari_msg);


// //    json_object_to_file("log.json",jobj) ;





  

// }
































int main (int argc, char *argv[])
{
    
    
    //get nameFile description
    // strcpy(f_desc_data.f_desc_name,getNameFile(argv);
    //Pour le test :p 
    //strcpy(f_desc_data.f_desc_name,argv[1]);

 if(argc>1){
          if(argc>2){
            if(argv[1][0]=='-'){
              strcpy(f_desc_data.f_desc_name,argv[2]);
            }
            else{
               strcpy(f_desc_data.f_desc_name,argv[1]);
            }
          }
          else{
            if(argv[1][0]=='-'){
              strcpy(f_desc_data.f_desc_name,"farifile");
            }
            else{
               strcpy(f_desc_data.f_desc_name,argv[1]);
            }
          }
        }
        if(argc==1){
           strcpy(f_desc_data.f_desc_name,"farifile");
        }



    
    //check for file description existance nameFile=argv(0)Make or argv(1)
    if(verifierExistanceFichier(f_desc_data.f_desc_name)){


   
    
        //step 1 : Extract data from file description & check existence of the src and  header files 
        ExtractFileDescData();
        
         //jsonFile();
    }
    
    else printf("fichier non existant ");



    

}



