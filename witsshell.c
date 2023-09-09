#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>



//fuction prints error message
void errormessage(){
	
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));

}

//function return array of path with size at index 0
char** path(char **pieces){
	char** directories;
	directories = malloc(sizeof(int*) * 50);
	for(int i=0;i<50;i++){
		directories[i]=malloc(sizeof(int*) * 50);
	}
	for(int i=2;i<atoi(pieces[0])+1;i++){
		 
		strcpy(directories[i-1],pieces[i]);
		
	}
	char * temp;
	temp = malloc(sizeof(int*)*50);
	sprintf(temp,"%d", atoi(pieces[0])-1);
	strcpy(directories[0],temp);
	return directories;
}

//function reads input from file
char **readFromfile(char *input,char *argv[]){
    
	char **arr;
    arr = malloc(sizeof(int*) * 100);
     
    for(int i = 0; i < 100; i++) {
        arr[i] = malloc(sizeof(int*) * 100);
    }
	
	//open file
    FILE *in_file=fopen(argv[1],"r");
    
    //if file not found
    if(in_file==NULL){
    
    	errormessage();
    	exit(1);
	}
	
	//read strings in a file
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

//function splits string into pieces according to spaces tabs and new lines
char **splitIntoPieces(char *input){
	
  	char **pieces;
  	char *chunk;
    pieces = malloc(sizeof(int*) * 50);
     
    for(int i = 0; i < 50; i++) {
        pieces[i] = malloc(sizeof(int*) * 20);
    }
	
	//splitting string	
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

//fuction splits input according to redirection sign >
char *splitIntoRedirection(char *input){
	
  	char **pieces;
  	char *chunk;
    pieces = malloc(sizeof(int*) * 50);
     
    for(int i = 0; i < 50; i++) {
        pieces[i] = malloc(sizeof(int*) * 20);
    }
	
	//splitting string	
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

//function splits string according to ampersand
char ** splitAmpersand(char *input){
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

//function to change directory
void changedir(char* path){	

if(	chdir(path)!=0){

	errormessage();
	}											
}

//function to check if redirection signs are placed properly
bool checkFormat(char* input){
		char* tmp;
		tmp = malloc(sizeof(int*)* 50);
			
		strcpy(tmp,input);
		char** pieces = splitIntoPieces(tmp);
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


//check presence of redirection sign
bool checkRedirection(char* cpy){
	
	for(int k=0;k<strlen(cpy);k++){
					if(cpy[k]=='>'){
						return true;
					}
					
				}
				return false;
}

//main function that return array of paths and responsible for executing the commands
char ** run(char* buffer, char** pathArr){
	

			//set to true if ampersand next to each other
	    	int temp=0;
	    	
	    	//allocate space to store array of commands
	    	char** parrParts;
	    	parrParts = malloc(sizeof(int*)* 50);
	    	for(int i=0;i<50;i++){
	    		parrParts[i]=malloc(sizeof(int*)* 50);
			}
			
			//alloacate array to store componets that have to be executed in parallel
	    	char** parrPartsOutCommands;
	    	parrPartsOutCommands = malloc(sizeof(int*)* 50);
	    	for(int i=0;i<50;i++){
	    		parrPartsOutCommands[i]=malloc(sizeof(int*)* 50);
			}
	    	
	    	
	    	//check for error in input, if everything is good put component in array of commands
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
			//set index for array of commands you want to run in parallel
			int indexOutCommands = 0;
			
			//if input is okay
			if(temp == 0){
				
				//split the input using ampersand
				parrParts = splitAmpersand(buffer);
				
			
				
			
			//Process each parallel component in this loop
			for(int i=1;i<atoi(parrParts[0])+1; i++){
				
				
				//allocate and copy each command part into cpy
				char *cpy=malloc(sizeof(int*)* 50);
				strcpy(cpy,parrParts[i]);
				
				//split the copy of the part and store in pieces array 
				char **pieces=splitIntoPieces(cpy);
				
				
				//check if the command is empty and if its not proceed
				if(strcmp(pieces[0], "0") != 0){
					//printf("%d\n",atoi(pieces[0]));
				
				
				//check if cammand is built in and if it is proceed to process the built in command if its not add it to the array of non built in commands
				if(strcmp(pieces[1], "exit")==0 || strcmp(pieces[1], "cd")==0  || strcmp(pieces[1], "path")==0){
					
					
					//exit command
					if(strcmp(pieces[1], "exit") == 0){			
						if(atoi(pieces[0])==1){
							exit(0);
						}else{
						
							errormessage();
						}
		    		
		    		
					}
					//path command empties the pathArray if the command is just path or adds the path to array if given any path
					else if(strcmp(pieces[1], "path") == 0){
					
						if(strcmp(pieces[0], "1") == 0){
						
								char** pathArr2;
								pathArr2 = malloc(sizeof(int*) * 50);
								for(int i=0;i<50;i++){
									pathArr2[i]=malloc(sizeof(int*) * 50);
								}
								sprintf(pathArr2[0],"%d",0);
							pathArr = pathArr2;
																		
						
						}
						
						else{
							
							pathArr =path(pieces);
						}
				}
				//cd command 
				else if(strcmp(pieces[1], "cd") == 0){
					
					if(strcmp(pieces[0], "2") == 0){							
						
						changedir(pieces[2]);									
					
					}
					else{	
																
						errormessage();
					}
				}
					
			}
				
		    	
				//if it is not a built-in command exit,cd or path,print error message
			else if(strcmp(pathArr[0],"0")!=0){
				
					
					if(checkFormat(buffer)){
						
						errormessage();
					}else{
						//add commands to array of non parallel commands
						parrPartsOutCommands[indexOutCommands]=parrParts[i];
						indexOutCommands++;
				
					}

					
				}
				else{
					
					errormessage();
				}
				
					
					
				}
				
				
			}
				
			}else{
				
				errormessage();
				
			}	
		
			
		//initilis list to store process id of command running in parallel	
		int pidList[indexOutCommands];
			int trackChildren=0;
		for(int i=0;i<indexOutCommands;i++){
			
				
				//split the paralle commands 
		
				char* cpy=malloc(sizeof(int*) * 50);
				strcpy(cpy,parrPartsOutCommands[i]);
				
				
				
				
				bool found=false;
				
					char** pieces = splitIntoPieces(parrPartsOutCommands[i]);
					for(int j=1; j<atoi(pathArr[0])+1; j++){
						if(!found){
								
								//for everuthing in array of paths intilise a new path copy path array of paths concatenate / and the command to be executued
								char* newPath=malloc(sizeof(int*)* 50);
								strcpy(newPath, pathArr[j]);
								if(newPath[strlen(newPath)-1]!='/'){
									strcat(newPath, "/");
								}
								
								strcat(newPath,pieces[1]);
								
								//check if the command path/cmd exists and is okay
								if(access(newPath,X_OK)==0){
									found=true;
									
									//create child process to run command and store the process id in the of process id's
									int pid = fork();
									if(pid==0){
										trackChildren++;
										char** args;
										
										
										if(checkRedirection(cpy)){
											
											
											
											char** pieces2 = splitIntoPieces(splitIntoRedirection(cpy));
											
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
								
								errormessage();
							}
				
		
				
				
			}
			
			
			pid_t wpid;
			int status=0;
			while((wpid=wait(&status))>0);
		
			return pathArr;

}





int main(int argc, char *argv[]){

	
	
	//allocate space for array to store the different paths you need to find an executable										
	char** pathArr;
	pathArr = malloc(sizeof(int*) * 50);
	for(int i=0;i<50;i++){
		pathArr[i]=malloc(sizeof(int*) * 50);
	}
	
	
	//initilise the path Array with bin
	sprintf(pathArr[0],"%s", "1");
	sprintf(pathArr[1],"%s", "/bin/");
	
	
	//allocate space input line
	char* buffer;
	size_t bufsize = 20;
	buffer = malloc(sizeof(int*)* bufsize);
	
	//check if we in Iterarive mode if True
	if(argc<=1){														
	
	
		//strart iterative mode by taking input
	    printf("%s", "witsshell> ");
	    
	    
	    //EOF marker
		int i=getline (&buffer, &bufsize, stdin);
		
		
		
		//Enter loop to take and process commands
	    while(1){
	    	
	    	if(i==-1){
			exit(0);
			}
			if(strcmp(buffer," ")!=0 && strcmp(buffer,"\n")){
				pathArr=run(buffer, pathArr);
			
			}
	    	
	    	
			printf("%s", "witsshell> ");
			i=getline (&buffer, &bufsize, stdin);		
			}
		
			
	    	
		}				
		//	if we are in batch mode
		else{
			if(argc!=2){
				
				errormessage();
				exit(1);
			}
		//read input from file
		char **arr=readFromfile(buffer,argv);
		
		//printf("%s\n",arr[0]);
		
		//split each string in the file into pieces
		char **pieces;
		for(int i=1;i<atoi(arr[0])+1;i++){
			//printf("%s\n",arr[i]);
			pathArr=run(arr[i],pathArr);
			
		}
		
	}
	
	
	
	return 0;
}
