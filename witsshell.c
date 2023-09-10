#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>




void handleError(){
	
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));

}


void changedir(char* path){	

if(	chdir(path)!=0){

	handleError();
	}											
}





char **readFromfile(char *input,char *argv[]){
    
	char **arr;
    arr = malloc(sizeof(int*) * 100);
     
    for(int i = 0; i < 100; i++) {
        arr[i] = malloc(sizeof(int*) * 100);
    }
	
	
    FILE *in_file=fopen(argv[1],"r");
    
    
    if(in_file==NULL){
    
    	handleError();
    	exit(1);
	}
	
	
	int index=1;
	while(fgets(input, 50, in_file) != NULL){					
		strcpy(arr[index],input);
		
		index++;
		
	}
	
	index--;

	sprintf(arr[0], "%d", index);
	
	fclose(in_file);
		
	return arr;
}

char **splitChunks(char *input){
	
  	char **pieces;
  	char *chunk;
    pieces = malloc(sizeof(int*) * 50);
     
    for(int i = 0; i < 50; i++) {
        pieces[i] = malloc(sizeof(int*) * 20);
    }
	

  	int index=1;
	while((chunk=strsep(&input,"\t \n")) !=NULL){ 				
		
	 		if(strcmp(chunk, "") != 0){
	 			strcpy(pieces[index],chunk);
	    		index++;
			}
	 		
	}
	index--;
	sprintf(pieces[0], "%d", index);					
	
  	return pieces;
}


char** path(char **chunks){
	char** dir;
	dir = malloc(sizeof(int*) * 50);
	for(int i=0;i<50;i++){
		dir[i]=malloc(sizeof(int*) * 50);
	}
	for(int i=2;i<atoi(chunks[0])+1;i++){
		 
		strcpy(dir[i-1],chunks[i]);
		
	}
	char * temp;
	temp = malloc(sizeof(int*)*50);
	sprintf(temp,"%d", atoi(chunks[0])-1);
	strcpy(dir[0],temp);
	return dir;
}


char *redirectionSplit(char *input){
	
  	char **pieces;
  	char *chunk;
    pieces = malloc(sizeof(int*) * 50);
     
    for(int i = 0; i < 50; i++) {
        pieces[i] = malloc(sizeof(int*) * 20);
    }
	

  	int index=1;
	while((chunk=strsep(&input,">")) !=NULL){ 				
		
	 		if(strcmp(chunk, "") != 0){
	 			strcpy(pieces[index],chunk);
	    		index++;
			}
	 		
	}
	index--;
	sprintf(pieces[0], "%d", index);					
	char* cpy =  malloc(sizeof(int*) * 50);
	for(int i=1;i<atoi(pieces[0])+1;i++){
		if(i==2){
			strcat(cpy, " ");
		}
		strcat(cpy,pieces[i]);
	}
	
  	return cpy;
  	
}
bool checkRedirection(char* cpy){
	
	for(int k=0;k<strlen(cpy);k++){
					if(cpy[k]=='>'){
						return true;
					}
					
				}
				return false;
}

char ** ampersandSplitting(char *input){
	char ** pieces;
	char* parrComponent;
	pieces = malloc (sizeof(int*)* 50);
	
	for(int i = 0; i < 50; i++) {
        pieces[i] = malloc(sizeof(int*) * 20);
    }
    int index = 1;
    while((parrComponent=strsep(&input,"&"))!=NULL){
    	if(strcmp(parrComponent,"")!=0){
    		strcpy(pieces[index],parrComponent);
    		index++;
    		
		}
		
	}
		index--;
		sprintf(pieces[0], "%d", index);
		return pieces;
    	
	
}



bool checkFormat(char* input){
		char* tmp;
		tmp = malloc(sizeof(int*)* 50);
			
		strcpy(tmp,input);
		char** pieces = splitChunks(tmp);
		int count=0;
			
		for(int i=0;i<strlen(input);i++){
			if(input[i]=='>'  ){
				count++;
				
				if(count>1){
					return true;
				}
				if(i==0 || i+1==strlen(input)-1){
					return true;
				}
				if(i>1){
					if(input[i-1]=='>'){
						return true;
					}
				}
			}
		}
		
		for(int i=1;i<atoi(pieces[0])+1;i++){
			
			if(strcmp(pieces[i],">")==0){
			
				if(atoi(pieces[0])-i!=1){
					return true;
				}
			}
		}
		return false;
					
				}






char ** run(char* buffer, char** arrayPath){
	

			
	    	int temp=0;
	    	
	    	
	    	char** parrParts;
	    	parrParts = malloc(sizeof(int*)* 50);
	    	for(int i=0;i<50;i++){
	    		parrParts[i]=malloc(sizeof(int*)* 50);
			}
			
			
	    	char** parrPartsOutCommands;
	    	parrPartsOutCommands = malloc(sizeof(int*)* 50);
	    	for(int i=0;i<50;i++){
	    		parrPartsOutCommands[i]=malloc(sizeof(int*)* 50);
			}
	    	
	    	
	    	
	    	if(buffer[0]=='&' && strlen(buffer)!=2){
	    		temp = 1;
	    		
	    	
			}
			else if(strlen(buffer)>1){
				
				 
				
					for(int i=0;i<strlen(buffer);i++){
					
				
	    		if(buffer[i]=='&'){
	    			if(buffer[i+1]=='&'){
	    				
	    				temp = 1;
	    				break;
					}
					continue;	
				}
	    		
				}	
				
				
			
		}
			
			int indexOutCommands = 0;
			
			
			if(temp == 0){
				
				
				parrParts = ampersandSplitting(buffer);
				
			
				
			
			
			for(int i=1;i<atoi(parrParts[0])+1; i++){
				
				
				
				char *cpy=malloc(sizeof(int*)* 50);
				strcpy(cpy,parrParts[i]);
				
				
				char **pieces=splitChunks(cpy);
				
				
				
				if(strcmp(pieces[0], "0") != 0){
					
				
				
				
				if(strcmp(pieces[1], "exit")==0 || strcmp(pieces[1], "cd")==0  || strcmp(pieces[1], "path")==0){
					
					
					
					if(strcmp(pieces[1], "exit") == 0){			
						if(atoi(pieces[0])==1){
							exit(0);
						}else{
						
							handleError();
						}
		    		
		    		
					}
					
					else if(strcmp(pieces[1], "path") == 0){
					
						if(strcmp(pieces[0], "1") == 0){
						
								char** pathArr2;
								pathArr2 = malloc(sizeof(int*) * 50);
								for(int i=0;i<50;i++){
									pathArr2[i]=malloc(sizeof(int*) * 50);
								}
								sprintf(pathArr2[0],"%d",0);
							arrayPath = pathArr2;
																		
						
						}
						
						else{
							
							arrayPath =path(pieces);
						}
				}
				
				else if(strcmp(pieces[1], "cd") == 0){
					
					if(strcmp(pieces[0], "2") == 0){							
						
						changedir(pieces[2]);									
					
					}
					else{	
																
						handleError();
					}
				}
					
			}
				
		    	
				
			else if(strcmp(arrayPath[0],"0")!=0){
				
					
					if(checkFormat(buffer)){
						
						handleError();
					}else{
						
						parrPartsOutCommands[indexOutCommands]=parrParts[i];
						indexOutCommands++;
				
					}

					
				}
				else{
					
					handleError();
				}
				
					
					
				}
				
				
			}
				
			}else{
				
				handleError();
				
			}	
		
			
		int pidList[indexOutCommands];
			int trackChildren=0;
		for(int i=0;i<indexOutCommands;i++){
			
				
				
		
				char* cpy=malloc(sizeof(int*) * 50);
				strcpy(cpy,parrPartsOutCommands[i]);
				
				
				
				
				bool found=false;
				
					char** pieces = splitChunks(parrPartsOutCommands[i]);
					for(int j=1; j<atoi(arrayPath[0])+1; j++){
						if(!found){
								
								
								char* newPath=malloc(sizeof(int*)* 50);
								strcpy(newPath, arrayPath[j]);
								if(newPath[strlen(newPath)-1]!='/'){
									strcat(newPath, "/");
								}
								
								strcat(newPath,pieces[1]);
								
								
								if(access(newPath,X_OK)==0){
									found=true;
									
									
									int pid = fork();
									if(pid==0){
										trackChildren++;
										char** args;
										
										
										if(checkRedirection(cpy)){
											
											
											
											char** pieces2 = splitChunks(redirectionSplit(cpy));
											
											args = malloc(sizeof(int*)* atoi(pieces2[0])-1);
											
											for(int i=0; i<atoi(pieces2[0])-1; i++){
												args[i]=malloc(sizeof(int*)* 20);
											}
												sprintf(args[0], "%s", pieces2[1]);
													
												for(int i = 2; i<atoi(pieces2[0]);i++){
													sprintf(args[i-1], "%s", pieces2[i]);
													printf("%s\n",pieces2[i]);
													}
													
											int file = open(pieces2[atoi(pieces2[0])], O_WRONLY | O_CREAT | O_TRUNC, 0777);
											
												if(file ==-1){
												
												}
												if(dup2(file,1)!=1){
											
													close(file);
												}
													
											}
											else{
												
												
												args = malloc(sizeof(int*)* atoi(pieces[0]));
											for(int i=0; i<atoi(pieces[0]); i++){
												args[i]=malloc(sizeof(int*)* 20);
											}
												
												sprintf(args[0], "%s", pieces[1]);
												for(int i = 2; i<atoi(pieces[0])+1;i++){
													sprintf(args[i-1], "%s", pieces[i]);
											}
												
											}
										
										pidList[i]=pid;  

										execv(newPath,args);
										
										exit(0);
									}
								}
						
						}
					
								
							}
							if(!found){
								
								handleError();
							}
				
		
				
				
			}
			
			
			pid_t wpid;
			int status=0;
			while((wpid=wait(&status))>0);
		
			return arrayPath;

}





int main(int argc, char *argv[]){

	
											
	char** arrayPath;
	arrayPath = malloc(sizeof(int*) * 50);
	for(int i=0;i<50;i++){
		arrayPath[i]=malloc(sizeof(int*) * 50);
	}
	
	
	sprintf(arrayPath[0],"%s", "1");
	sprintf(arrayPath[1],"%s", "/bin/");
	
	
	
	char* buffer;
	size_t bufsize = 20;
	buffer = malloc(sizeof(int*)* bufsize);
	
	
	if(argc<=1){														
	
	
		
	    printf("%s", "witsshell> ");
	    
	    
	    
		int k=getline (&buffer, &bufsize, stdin);
		
		
		
		
	    while(1){
	    	
	    	if(k==-1){
			exit(0);
			}
			if(strcmp(buffer," ")!=0 && strcmp(buffer,"\n")){
				arrayPath=run(buffer, arrayPath);
			
			}
	    	
	    	
			printf("%s", "witsshell> ");
			k=getline (&buffer, &bufsize, stdin);		
			}
		
			
	    	
		}				
		
		else{
			if(argc!=2){
				
				handleError();
				exit(1);
			}
		
		char **arr=readFromfile(buffer,argv);
	
		char **pieces;
		for(int i=1;i<atoi(arr[0])+1;i++){
			
			arrayPath=run(arr[i],arrayPath);
			
		}
		
	}
	
	
	
	return 0;
}
